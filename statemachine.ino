bool select_input() {
  Serial.println("Auswahl Input: 1");
  measurement_start = wait_for_input();
  switch (measurement_start) {
    case 49:
      Serial.println("Input 1");
      return false;
      break;
    default:
      Serial.println("falsche Eingabe");
      return true;
      break;
  }
}

bool start_measurement() {
  Serial.println("Start Messung? j/n");
  measurement_start = wait_for_input();
  switch (measurement_start) {
    case 74:
    case 106:
      Serial.println("j - Start Messung");
      measure_state |= (1 << measurement_start_true);
      return false;
      break;
    case 78:
    case 110:
      Serial.println("n - kein Start Messung");
      return false;
      break;
    default:
      Serial.println("falsche Eingabe");
      return true;
      break;
  }
}

void measurement() {
  unsigned long time_since_start = millis();
  int counter = 0;
  Serial.print("vor start");
  Serial.println(analogRead(pin_A0));
  digitalWrite(pin_D0, HIGH);
  while ((millis() - time_since_start) < 15000) {
    Serial.println(analogRead(pin_A0));
    counter++;
    delay(10);
  }
  digitalWrite(pin_D0, LOW);
}

void statemachine_step_response() {
  //1. auswählen Glieder
  //2. starten der Messung
  //3. durchführen der Messung
  //4. anzeigen der Verläufe/Ergebnisse


  while (select_input()) {}
  while (start_measurement()) {} //menu for start measurement
  if (measure_state &= 0b00000001) {
    measurement();
    measure_state &= (1 << measurement_start_true);
    measure_state |= (1 << measurement_finish_true);
  }
}

void statemachine_control() {

}
