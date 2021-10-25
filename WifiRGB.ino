#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#include "names.h"
#include "web_admin.h"
#include "web_interface.h"
#include "web_iro_js.h"
#include "web_jquery_js.h"
#include "web_popper_js.h"
#include "web_bootstrap_js.h"
#include "web_data.h"
#include "web_jsxgraph_js.h"
#include "web_jsxgraph_css.h"

const char* ssid = "Lord LANdemort";
const char* password = "79351652776124235782";
const char* deviceName = "wifi-rgb";

//char Messdaten[];

#define BUILTIN_LED 2 // internal ESP-12 LED on GPIO2

#define REDPIN 12
#define GREENPIN 14
#define BLUEPIN 5

ESP8266WebServer server(80);
IPAddress clientIP(192, 168, 2, 250); //the IP of the device
IPAddress gateway(192, 168, 2, 1); //the gateway
IPAddress subnet(255, 255, 255, 0); //the subnet mask

//const char* ap_ssid = "ESP8266"; //Access Point SSID
//const char* ap_password= "12345678";
//uint8_t max_connections=8;




void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266 wifi rgb!");
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void Messung(char modus, char messobjekt, int timerange){
  Serial.println(modus,DEC);
    Serial.println(messobjekt,DEC);
    Serial.println(timerange,DEC);
}

void handleApiRequest() {

  Serial.println("### API Request:");
    /* 
    // DEBUG CODE
    String headers;
    headers += "HTTP Headers:\n";
    headers += server.headers();
    headers += "\n";
    for (uint8_t i = 0; i < server.headers(); i++) {
      headers += " " + server.headerName(i) + ": " + server.header(i) + "\n";
    }
    Serial.println(headers);
    */
    if (server.hasArg("plain") == false) { //Check if body received
      server.send(200, "text/plain", "Body not received");
      return;
    }

    /*
    // DEBUG CODE
    String message;
    headers += "HTTP args:\n";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    Serial.println(message);
    Serial.println(server.arg("plain"));
    */
    
    const size_t bufferSize = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + 70;
    DynamicJsonDocument jsonDocument(bufferSize);
    deserializeJson(jsonDocument, server.arg("plain"));

    Serial.println("JSON Body: ");
    serializeJson(jsonDocument, Serial);
    Serial.println();

    JsonObject root = jsonDocument.as<JsonObject>();
    char modus = root["Modus"];
    char messobjekt = root["Objekt"];
    int timerange = root["Zeit"];
    Serial.println(modus,DEC);
    Serial.println(messobjekt,DEC);
    Serial.println(timerange,DEC);
    
    server.send(200, "application/json", server.arg("plain"));

    Messung(modus, messobjekt, timerange);
}

void handleData()
{
  DynamicJsonDocument doc(1024);
  
  double gas = 0, distance = 0, VALUE=1;
  char measure_data[100] = {1,2,3};
 
  doc["distance"]=distance;
  doc["gas"]=gas;
  char lenght = sizeof(measure_data);
  doc["lenght"]=lenght;
  
  for(int i = 0; i<lenght;i++){
    doc["data"][i]=measure_data[i];
    Serial.println(measure_data[i],DEC);
  }
  
  serializeJson(doc,Serial);
  // Prepare the data for serving it over HTTP
  String output = "distance: " + String(distance) + "\n";
  output += "CO level: " + String(gas);
  // Serve the data as plain text, for example
  //server.send(200,"text/plain",output);
  //server.send(200,"application/json",server.arg(doc));
  String buf;
  serializeJson(doc,buf);
  server.send(200, F("application/json"), buf);
}



void setup(void) {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName);
  WiFi.config(clientIP, gateway, subnet); // Remove for DHCP

  WiFi.begin(ssid, password);
//Serial.print("start");
//WiFi.softAP(ap_ssid,ap_password);
//    Serial.print("Access Point is Created with SSID: ");
//    Serial.println(ap_ssid);
//    Serial.print("Max Connections Allowed: ");
//    Serial.println(max_connections);
//    Serial.print("Access Point IP: ");
//    Serial.println(WiFi.softAPIP());

  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, LOW); // Turn the LED ON by making the voltage LOW after wifi is connected

  // adjust the PWM range
  // see https://esp8266.github.io/Arduino/versions/2.0.0/doc/reference.html#analog-output
  analogWriteRange(255);

  // Root and 404
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  // iro.js User Interface and Javascript
  server.on("/ui", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBINTERFACE);
  });
  server.on("/admin", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBADMIN);
  });
  server.on("/iro.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", IRO_JS);
  });
  server.on("/jquery-3.3.1.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", JQUERY_JS);
  });
  server.on("/popper.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", POPPER_JS);
  });
  server.on("/bootstrap.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", BOOTSTRAP_JS);
  });
  server.on("/rawdata",handleData);
  server.on("/data", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBDATA);
  });
//  server.on("/jsxgraphcore.js", HTTP_GET, []() {
//    server.send_P(200, "application/javascript", JSXGRAPH_JS);
//  });
//  server.on("/jsxgraph.css", HTTP_GET, []() {
//    server.send_P(200, "text/css", JSXGRAPH_CSS);
//  });
   
  // REST-API
  server.on("/api/v1/state", HTTP_POST, handleApiRequest);
  server.on("/api/v1/data", HTTP_POST, handleData);
  //server.on("/api/v1/reset", HTTP_GET, resetOutputs);

  server.begin();
  Serial.println("WifiRGB HTTP server started");
}

void loop(void) {
  server.handleClient();
}
