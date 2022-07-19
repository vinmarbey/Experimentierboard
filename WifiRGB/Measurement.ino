void starte_Messung(char modus, int timerange) {
  if (modus == 1) {
    control_messung |= (1 << sprung);
  } else if (modus == 2) {
    control_messung |= (1 << regelung);
  } else {
    control_messung &= ~((1 << sprung) | (1 << regelung));
    return;
  }

  pinMode(MESSOBJEKT1, OUTPUT);
  analogWrite(MESSOBJEKT1, 255);  

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
  digitalWrite(Status_LED, HIGH); //LED Anzeige für Messung an
  
  start_zeitpunkt = millis();
  Serial.println("Starte Messung");
  
  return;
}

void stoppe_Messung() {
  analogWrite(MESSOBJEKT1, 0);
  digitalWrite(Status_LED, LOW); //LED Anzeige für Messung aus
  control_messung &= ~((1 << sprung) | (1 << regelung));
  control_messobjekt = 0;
  control_messzeitspanne = 0;
  control_messung &= ~(1 << start_messung);

}

void handleMessung() {

  if (!(control_messung & (1 << start_messung))) return; // soll überhaupt gerade gemessen werden
  
  akt_time = millis() - start_zeitpunkt;
  
  if (akt_time > control_messzeitspanne) { // Messzeit ist größer als Messzeitraum
    stoppe_Messung();
    Serial.println("Stoppe Messung");
    return;
  }

  delta_time = akt_time - last_time;
  if (delta_time < resolution) return; // die Zeit zur nächsten mesung ist noch nicht ran
  Serial.println("Messpunkt");
  last_time = akt_time;
  akt_value = analogRead(A0) / 4.0;
  if (akt_value>255&&akt_value<260) akt_value=255;
  //Serial.println(akt_value);
  Messdaten[zaehler_Messdaten] = (unsigned char) akt_value;
  zaehler_Messdaten ++;
  
  if (!(control_messung &  (1 << regelung))) return; //soll eine Regelung gemacht werden?

  error = int(((sollwert/1000.0)/3.3)*1024 - akt_value*4);
  
  if (reglerstruktur == 1){
    //P-Regler
    output_reg = gain/100.0 * error;
    if (output_reg > 255) output_reg=255;
    Stellgroesse[zaehler_Messdaten-1] = output_reg/4;
    
    
  }else if (reglerstruktur == 2){
    //PI-Regler
    integrate_temp += error * resolution/1000.0;
    output_reg = gain/100.0 * (error + (integrate_temp/(nachstellzeit/1000.0)));
//    Serial.println(error);
//    Serial.println(integrate_temp);
//    Serial.println(output_reg);
    //output_reg = gain/100.0 * output_reg;
    //output_reg = gain/100.0 * (integrate_temp/(nachstellzeit/1000.0));
    if (output_reg > 255) output_reg=255;
    Stellgroesse[zaehler_Messdaten-1] = output_reg/4;
    //output_reg = int(gain/100.0 * (error + (integrate_temp)));
  }
  analogWrite(MESSOBJEKT1, int(output_reg));
  
  
  return;
}
