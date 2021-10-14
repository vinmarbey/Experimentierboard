byte wait_for_input() {
  byte input_byte_serial;

  while (Serial.available() <= 0) {}
  input_byte_serial = Serial.read();
  return input_byte_serial;
}

bool menu_measure_mode() {
  //Input:
  //Output: Error im Menue Messmodus-Auswahl
  //true: error liegt vor
  //false: kein error liegt vor, korrekte Auswahl nach Auswahlmöglichkeiten
  //variable: Messmodus in Variable measurement_mode zur Steuerung der Funktionen

  //byte measurement_mode;

  //Print Menu
  Serial.println("Menü - bitte auswählen:");
  Serial.println("1 - Sprungantwort");
  Serial.println("2 - Regelung");

  measurement_mode = wait_for_input();

  switch (measurement_mode) {
    case 49:
      Serial.println("Auswahl: 1 - Sprungantwort");
      measurement_mode = 1;
      return false;
      break;
    case 50:
      Serial.println("Auswahl: 2 - Regelung");
      measurement_mode = 2;
      return false;
      break;
    default:
      Serial.println("Auswahl: falsche Eingabe");
      return true;
      break;
  }

}
