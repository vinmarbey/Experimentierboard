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



void handleApiRequest() {

  //Serial.println("### API Request:");
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
  /*
          // DEBUG CODE
    Serial.println("JSON Body: ");
    serializeJson(jsonDocument, Serial);
    Serial.println();
  */
  
  JsonObject root = jsonDocument.as<JsonObject>();

  char start_measurement = root["Start"];
  char stop_measurement = root["Stop"];
  if (start_measurement == 1) {
    modus = root["Modus"];
    char messobjekt = root["Objekt"];
    if (modus == 1) {
      timerange = root["Zeit"];
      resolution = root["Aufloesung"];
      /*
        // DEBUG CODE
        Serial.println(modus, DEC);
        Serial.println(messobjekt, DEC);
        Serial.println(timerange, DEC);
        Serial.println(resolution, DEC);
      */
      server.send(200, "application/json", server.arg("plain"));

      starte_Messung(modus, messobjekt, timerange);
    } else if (modus == 2) {
      
      timerange = root["Zeit2"];
      resolution = root["Aufloesung2"];
      gain = root["Verstaerkung2"];
      /*
        // DEBUG CODE
        Serial.println(modus, DEC);
        Serial.println(messobjekt, DEC);
        Serial.println(gain, DEC);
        Serial.println(timerange, DEC);
        Serial.println(resolution, DEC);
      */

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

  doc["resolution"] = resolution;
  doc["modus"] = modus;
  doc["zeit"] = timerange;
  if (modus == 2) {
    doc["verstaerkung"] = gain;
  }
  char lenght = sizeof(Messdaten);
  doc["lenght"] = lenght;

  for (int i = 0; i < lenght; i++) {
    doc["data"][i] = Messdaten[i];
  }


  //serializeJson(doc, Serial);
  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);
}
