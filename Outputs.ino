void resetOutputs() {
  pinMode(MESSOBJEKT1, OUTPUT);
  analogWrite(MESSOBJEKT1, 255);
  analogWrite(MESSOBJEKT1, 0);
  pinMode(Status_LED, OUTPUT);
  digitalWrite(Status_LED, HIGH);
  digitalWrite(Status_LED, LOW);
}
