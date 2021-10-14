char incomingByte; // for incoming serial data
byte measurement_mode = 0;
byte measurement_start = 0; //74 or 106 if start meassurement; 78 or 110 if no start measurement
byte menu_error=0; //0|0|0|0|0|0|0|0 //true if error
byte measure_state=0; //0|0|0|0|0|0|finish|start //
#define measurement_start_true 0
#define measurement_finish_true 1
#define pin_D0 16
#define pin_A0 A0
void setup() {
  Serial.begin(9600);
  pinMode(pin_D0, OUTPUT);
  Serial.println(analogRead(pin_A0));
  digitalWrite(pin_D0, LOW);
  Serial.println(analogRead(pin_A0));
  pinMode(pin_A0, INPUT);

}

void loop() {
  delay(200);
  while (menu_measure_mode()) //menu for choosing the mode
  {}
  if (measurement_mode == 1) {
    statemachine_step_response(); //measure an step response on discrete elements
  } else if (measurement_mode == 2) {
    statemachine_control(); //control voltage or current of discrete elements
  }
}
