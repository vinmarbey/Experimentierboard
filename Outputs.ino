void resetOutputs() {
  pinMode(MESSOBJEKT1, OUTPUT);
  analogWrite(MESSOBJEKT1, 255);
  analogWrite(MESSOBJEKT1, 0);
}
