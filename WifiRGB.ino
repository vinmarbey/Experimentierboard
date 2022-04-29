#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>


#include "web_admin.h"
#include "web_data.h"
#include "web_jsxgraph_js.h"
#include "web_jsxgraph_css.h"
#include "web_drucken_js.h"

#define BUILTIN_LED 2 // internal ESP-12 LED on GPIO2

#define MESSOBJEKT1 16 //PIN D0
#define Status_LED 5 //PIN D1, LED ON, wenn Messung läuft
#define sprung 0
#define regelung 1
#define start_messung 2

//confic Station-Mode
const char* ssid = "Lord LANdemort";
const char* password = "79351652776124235782";
const char* deviceName = "wifi-rgb";

//confic Access-Point-Mode
const char* ap_ssid = "ESP8266_1";
const char* ap_password = "12345678";
//uint8_t max_connections=8;

unsigned char Messdaten[254];
unsigned char Stellgroesse[254];

unsigned char zaehler_Messdaten = 0;
char control_messung = 0;

char control_messobjekt = 0, modus;
char reglerstruktur;
int control_messzeitspanne, timerange, resolution;
int gain=100,sollwert=2000,nachstellzeit=5000;
unsigned long start_zeitpunkt = 0;

unsigned long akt_time, delta_time, last_time = 0;
int akt_value;
int error, integrate_temp;
float output_reg;



ESP8266WebServer server(80);
IPAddress clientIP(192, 168, 2, 250); //the IP of the device
IPAddress gateway(192, 168, 2, 1); //the gateway
IPAddress subnet(255, 255, 255, 0); //the subnet mask


void resetOutputs();
void handleRoot();
void handleNotFound();
void handleApiRequest();
void handleData();
void starte_Messung(char, char, int);
void stoppe_Messung();
void handleMessung();

void setup(void) {
  resetOutputs();
  Serial.begin(115200);

  //WiFi.mode(WIFI_STA);
    //WiFi.mode(WIFI_AP_STA);
  WiFi.mode(WIFI_AP);
    //WiFi.hostname(deviceName);
    //WiFi.config(clientIP, gateway, subnet); // Remove for DHCP

    //WiFi.begin(ssid, password);
  //Serial.print("start");
  WiFi.softAP(ap_ssid, ap_password);
//  Serial.print("Access Point is Created with SSID: ");
//  Serial.println(ap_ssid);
//  Serial.print("Max Connections Allowed: ");
//  Serial.println(max_connections);
//  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("");

  // Wait for connection
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//  if (MDNS.begin("esp8266")) {
//    Serial.println("MDNS responder started");
//  }

  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, LOW); // Turn the LED ON after wifi is connected

  analogWriteRange(255);

  // Root and 404
  //server.on("/", handleRoot);
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBADMIN);
  });
  server.onNotFound(handleNotFound);

  // Subpages
  server.on("/admin", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBADMIN);
  });
  server.on("/rawdata", handleData);
  server.on("/data", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBDATA);
  });
  // API for scripts
  server.on("/drucken.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", DRUCKEN_JS);
  });
  server.on("/jsxgraphcore.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", JSXGRAPH_JS);
  });
  server.on("/jsxgraph.css", HTTP_GET, []() {
    server.send_P(200, "text/css", JSXGRAPH_CSS);
  });

  // REST-API
  server.on("/api/v1/state", HTTP_POST, handleApiRequest);
  server.on("/api/v1/data", HTTP_POST, handleData);
  //server.on("/api/v1/reset", HTTP_GET, resetOutputs);

  server.begin();
  Serial.println("RegTech HTTP server started");
}

void loop(void) {
  server.handleClient();
  handleMessung();
}
