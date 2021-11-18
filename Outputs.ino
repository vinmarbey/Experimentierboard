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
