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
  

  if (akt_time > control_messzeitspanne) { // Messzeit ist größer als Messzeitraum
    stoppe_Messung();
    Serial.println("Stoppe Messung");
    return;
  }

  delta_time = akt_time - last_time;
  if (delta_time < resolution) return; // die Zeit zur nächsten mesung ist noch nicht dran
  //Serial.println(delta_time, DEC);
  last_time = akt_time;
  akt_value = analogRead(A0) / 4;
  //Serial.println(akt_value);
  Messdaten[zaehler_Messdaten] = (unsigned char) akt_value;
  zaehler_Messdaten ++;
  if (!(control_messung &  (1 << regelung))) return;
  error = int(((sollwert/1000)/3.3)*1024 - akt_value);
  
  if (reglerstruktur == 1){
    //P-Regler
    output_reg = int(gain/100 * error);
    //Serial.println(int(output_reg));
  }else if (reglerstruktur == 2){
    //PI-Regler
    integrate_temp += error * resolution;
    //    output_reg = int(gain/100 * (error + (integrate_temp/nachstellzeit)));
    output_reg = int(gain/100 * (error + (integrate_temp)));
  }
  analogWrite(MESSOBJEKT1, int(output_reg));
  return;
}
