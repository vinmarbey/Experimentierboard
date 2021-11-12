#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>


#include "web_admin.h"

#include "web_jquery_js.h"
#include "web_popper_js.h"
#include "web_bootstrap_js.h"
#include "web_data.h"
#include "web_jsxgraph_js.h"
#include "web_jsxgraph_css.h"

const char* ssid = "Lord LANdemort";
const char* password = "79351652776124235782";
const char* deviceName = "wifi-rgb";

unsigned char Messdaten[60];
unsigned char zaehler_Messdaten = 0;
char control_messung = 0;
#define sprung 0
#define regelung 1
#define start_messung 2
char control_messobjekt = 0;
int control_messzeitspanne;
unsigned long start_zeitpunkt = 0;
unsigned long resolution;
unsigned long akt_time, delta_time, last_time = 0;
int akt_value;

#define BUILTIN_LED 2 // internal ESP-12 LED on GPIO2

#define MESSOBJEKT1 16
#define MESSOBJEKT2 14
#define MESSOBJEKT3 5

ESP8266WebServer server(80);
IPAddress clientIP(192, 168, 2, 250); //the IP of the device
IPAddress gateway(192, 168, 2, 1); //the gateway
IPAddress subnet(255, 255, 255, 0); //the subnet mask

//const char* ap_ssid = "ESP8266"; //Access Point SSID
//const char* ap_password= "12345678";
//uint8_t max_connections=8;


void resetOutputs() {
  pinMode(MESSOBJEKT1, OUTPUT);
  pinMode(MESSOBJEKT2, OUTPUT);
  pinMode(MESSOBJEKT3, OUTPUT);

  analogWrite(MESSOBJEKT1, 255);
  analogWrite(MESSOBJEKT2, 255);
  analogWrite(MESSOBJEKT3, 255);

  analogWrite(MESSOBJEKT1, 0);
  analogWrite(MESSOBJEKT2, 0);
  analogWrite(MESSOBJEKT3, 0);
}

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

void starte_Messung(char modus, char messobjekt, int timerange) {
  if (modus == 1) {
    control_messung |= (1 << sprung);
  } else if (modus == 2) {
    control_messung |= (1 << regelung);
  } else {
    control_messung &= ~((1 << sprung) | (1 << regelung));
    return;
  }
  //Serial.println(control_messung, BIN);

  if (messobjekt < 0) { // das Messobjekt gibt es nicht
    control_messobjekt = 0;
    return;
  }
  control_messobjekt = messobjekt;
  switch (control_messobjekt) {
    case 1:
      pinMode(MESSOBJEKT1, OUTPUT);
      analogWrite(MESSOBJEKT1, 255);
      break;
    default:
      break;
  }
  //Serial.println(control_messobjekt, DEC);

  if (timerange <= 0) {
    control_messzeitspanne = 0;
    return;
  }
  control_messzeitspanne = timerange;
  zaehler_Messdaten = 0;
  for (int i = 0; i < sizeof(Messdaten); i++) {
    Messdaten[i] = 0;
  }
  control_messung |= (1 << start_messung);
  start_zeitpunkt = millis();
  Serial.println("Starte Messung");
  return;
}

void stoppe_Messung() {
  switch (control_messobjekt) {
    case 1:
      pinMode(MESSOBJEKT1, OUTPUT);
      analogWrite(MESSOBJEKT1, 0);
      break;
    default:
      break;
  }
  control_messung &= ~((1 << sprung) | (1 << regelung));
  control_messobjekt = 0;
  control_messzeitspanne = 0;
  control_messung &= ~(1 << start_messung);

}

void handleMessung() {

  if (!(control_messung & (1 << start_messung))) return; // soll überhaupt gerade gemessen werden
  akt_time = millis() - start_zeitpunkt;
  //Serial.println(akt_time, DEC);

  if (akt_time > control_messzeitspanne) { // Messzeit ist größer als Messzeitraum
    stoppe_Messung();
    Serial.println("Stoppe Messung");

    return;
  }
  delta_time = akt_time - last_time;


  if (delta_time < resolution) return; // die Zeit zur nächsten mesung ist noch nicht dran
  //Serial.println(delta_time, DEC);
  last_time = akt_time;
  akt_value = analogRead(A0);
  //Serial.println(akt_value, DEC);
  Messdaten[zaehler_Messdaten] = (unsigned char) akt_value;
  zaehler_Messdaten ++;

  //2 D Array füllen

  if (!(control_messung &  (1 << regelung))) return;
  //pin.value = (soll_value - akt_value);
  return;
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

  const size_t bufferSize = JSON_OBJECT_SIZE(9) + JSON_OBJECT_SIZE(4) + 70;
  DynamicJsonDocument jsonDocument(bufferSize);
  deserializeJson(jsonDocument, server.arg("plain"));

  Serial.println("JSON Body: ");
  serializeJson(jsonDocument, Serial);
  Serial.println();

  JsonObject root = jsonDocument.as<JsonObject>();
  int timerange, gain;
  char start_measurement = root["Start"];
  char stop_measurement = root["Stop"];
  if (start_measurement == 1) {
    char modus = root["Modus"];
    char messobjekt = root["Objekt"];
    if (modus == 1) {
      timerange = root["Zeit"];
      resolution = root["Aufloesung"];
      Serial.println(modus, DEC);
      Serial.println(messobjekt, DEC);
      Serial.println(timerange, DEC);
      Serial.println(resolution, DEC);
      server.send(200, "application/json", server.arg("plain"));

      starte_Messung(modus, messobjekt, timerange);
    } else if (modus == 2) {
      Serial.println("Regelung");
      timerange = root["Zeit2"];
      resolution = root["Aufloesung2"];
      gain = root["Verstaerkung2"];
      Serial.println(modus, DEC);
      Serial.println(messobjekt, DEC);
      Serial.println(gain, DEC);
      Serial.println(timerange, DEC);
      Serial.println(resolution, DEC);

      server.send(200, "application/json", server.arg("plain"));

      starte_Messung(modus, messobjekt, timerange);
    } else if (stop_measurement == 1) {
      server.send(200, "application/json", server.arg("plain"));
      stoppe_Messung();
    }
  }
}

void handleData()
{
  DynamicJsonDocument doc(1024);//6144 für 256 elemente
  //DynamicJsonDocument doc(2048);

  double gas = 0, distance = 0, VALUE = 1;
  char measure_data[100] = {1, 2, 3};

  doc["distance"] = distance;
  doc["gas"] = gas;
  char lenght = sizeof(measure_data);
  doc["lenght"] = lenght;

  for (int i = 0; i < lenght; i++) {
    //doc["data"][i] = measure_data[i];
    doc["data"][i] = Messdaten[i];
    //Serial.println(measure_data[i], DEC);
  }

  serializeJson(doc, Serial);
  // Prepare the data for serving it over HTTP
  String output = "distance: " + String(distance) + "\n";
  output += "CO level: " + String(gas);
  // Serve the data as plain text, for example
  //server.send(200,"text/plain",output);
  //server.send(200,"application/json",server.arg(doc));
  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);
}


void setup(void) {
  resetOutputs();
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
  //  server.on("/ui", HTTP_GET, []() {
  //    server.send_P(200, "text/html", WEBINTERFACE);
  //  });
  server.on("/admin", HTTP_GET, []() {
    server.send_P(200, "text/html", WEBADMIN);
  });
  //  server.on("/iro.min.js", HTTP_GET, []() {
  //    server.send_P(200, "application/javascript", IRO_JS);
  //  });
  server.on("/jquery-3.3.1.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", JQUERY_JS);
  });
  server.on("/popper.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", POPPER_JS);
  });
  server.on("/bootstrap.min.js", HTTP_GET, []() {
    server.send_P(200, "application/javascript", BOOTSTRAP_JS);
  });
  server.on("/rawdata", handleData);
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
  handleMessung();
}
