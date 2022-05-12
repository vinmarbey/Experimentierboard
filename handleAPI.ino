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
  if (server.hasArg("plain") == false) { //Check if body received
    server.send(200, "text/plain", "Body not received");
    return;
  }

  const size_t bufferSize = JSON_OBJECT_SIZE(9) + JSON_OBJECT_SIZE(4) + 70;
  DynamicJsonDocument jsonDocument(bufferSize);
  deserializeJson(jsonDocument, server.arg("plain"));
//Serial.println("JSON Body: ");
    //serializeJson(jsonDocument, Serial);
    //Serial.println();
  JsonObject root = jsonDocument.as<JsonObject>();
  
  //char start_measurement = root["Start"];
  //char stop_measurement = root["Stop"];
  if (root["Start"] == 1) {
    modus = root["Modus"];
    if (modus == 1) {
      timerange = root["Zeit"];
      resolution = root["Aufloesung"];
      resolution = resolution /100.0;
      //Serial.println(resolution);
      server.send(200, "application/json", server.arg("plain"));

      starte_Messung(modus, timerange);
    } else if (modus == 2) {
      timerange = root["Zeit2"];
      resolution = root["Aufloesung2"];
      resolution = resolution /100.0;
      reglerstruktur = root["Regelstruktur"];
      gain = root["Verstaerkung"];
      sollwert = root["Sollwert"];
      nachstellzeit = root["Nachstellzeit"];
      //Serial.println(sollwert);
      //Serial.println(nachstellzeit);
      //Serial.println(gain);
      
      server.send(200, "application/json", server.arg("plain"));

      starte_Messung(modus, timerange);
    } else if (root["Stop"] == 1) {
      server.send(200, "application/json", server.arg("plain"));
      stoppe_Messung();
    }
  }
}

void handleData()
{
  //DynamicJsonDocument doc(1024);//6144 für 256 elemente
  //DynamicJsonDocument doc(2048);
  DynamicJsonDocument doc(6144);

  int number_of_elements = timerange / (resolution);
//  int number_of_schleifen = number_of_elements / 60;
//  //doc["schleifen"] = number_of_schleifen;
//  int rest_schleifen = number_of_elements % number_of_schleifen;
  doc["resolution"] = resolution*100;
  doc["modus"] = modus;
  doc["zeit"] = timerange;
  if (modus == 2) {
    doc["verstaerkung"] = gain;
    doc["Regelstruktur"] = reglerstruktur;
    doc["Sollwert"] = sollwert;
    doc["Nachstellzeit"] = nachstellzeit;
  }
  char lenght = sizeof(Messdaten);
  doc["lenght"] = lenght;
    doc["data"][0] = 0;
    doc["data1"][0] = 0;
    doc["data2"][0] = 0;
    doc["data3"][0] = 0;
    doc["data4"][0] = 0;
  
  
    if (number_of_elements >= 60) {
      for (int i = 0; i < 60; i++) {
        doc["data"][i] = Messdaten[i];
      }
      if (number_of_elements - 120 >= 0) {
        for (int i = 0; i < 60; i++) {
          doc["data1"][i] = Messdaten[i + 60];
        }
  
        if (number_of_elements - 180 >= 0) {
          for (int i = 0; i < 60; i++) {
            doc["data2"][i] = Messdaten[i + 120];
          }
          if (number_of_elements - 240 >= 0) {
            for (int i = 0; i < 60; i++) {
              doc["data3"][i] = Messdaten[i + 180];
            }
            if (number_of_elements - 300 >= 0) {
              for (int i = 0; i < 60; i++) {
                doc["data4"][i] = Messdaten[i + 240];
              }
            }
            else {
              for (int i = 0; i < number_of_elements % 60; i++) {
                doc["data4"][i] = Messdaten[i + 240];
              }
            }
          }
          else {
            for (int i = 0; i < number_of_elements % 60; i++) {
              doc["data3"][i] = Messdaten[i + 180];
            }
          }
        }
        else {
          for (int i = 0; i < number_of_elements % 60; i++) {
            doc["data2"][i] = Messdaten[i + 120];
          }
        }
      }
      else {
        for (int i = 0; i < number_of_elements % 60; i++) {
          doc["data1"][i] = Messdaten[i + 60];
        }
      }
    }
    else {
      for (int i = 0; i < number_of_elements % 60; i++) {
        doc["data"][i] = Messdaten[i];
      }
    }

    doc["data_stellg"][0] = 0;
    doc["data_stellg1"][0] = 0;
    doc["data_stellg2"][0] = 0;
    doc["data_stellg3"][0] = 0;
    doc["data_stellg4"][0] = 0;
  
    if (modus == 2){
    if (number_of_elements >= 60) {
      for (int i = 0; i < 60; i++) {
        doc["data_stellg"][i] = Stellgroesse[i];
      }
      if (number_of_elements - 120 >= 0) {
        for (int i = 0; i < 60; i++) {
          doc["data_stellg1"][i] = Stellgroesse[i + 60];
        }
  
        if (number_of_elements - 180 >= 0) {
          for (int i = 0; i < 60; i++) {
            doc["data_stellg2"][i] = Stellgroesse[i + 120];
          }
          if (number_of_elements - 240 >= 0) {
            for (int i = 0; i < 60; i++) {
              doc["data_stellg3"][i] = Stellgroesse[i + 180];
            }
            if (number_of_elements - 300 >= 0) {
              for (int i = 0; i < 60; i++) {
                doc["data_stellg4"][i] = Stellgroesse[i + 240];
              }
            }
            else {
              for (int i = 0; i < number_of_elements % 60; i++) {
                doc["data_stellg4"][i] = Stellgroesse[i + 240];
              }
            }
          }
          else {
            for (int i = 0; i < number_of_elements % 60; i++) {
              doc["data_stellg3"][i] = Stellgroesse[i + 180];
            }
          }
        }
        else {
          for (int i = 0; i < number_of_elements % 60; i++) {
            doc["data_stellg2"][i] = Stellgroesse[i + 120];
          }
        }
      }
      else {
        for (int i = 0; i < number_of_elements % 60; i++) {
          doc["data_stellg1"][i] = Stellgroesse[i + 60];
        }
      }
    }
    else {
      for (int i = 0; i < number_of_elements % 60; i++) {
        doc["data_stellg"][i] = Stellgroesse[i];
      }
    }
    }

  //serializeJson(doc, Serial);
  String buf;
  serializeJson(doc, buf);
  server.send(200, F("application/json"), buf);
}
