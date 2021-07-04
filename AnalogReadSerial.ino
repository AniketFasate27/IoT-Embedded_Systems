int val, timer1, timer2;
const int potPin = 0;
const int potpin2 = 2;

void setup() {
  Serial.begin(115200);
  pinMode(0,INPUT);
  pinMode(2,INPUT);
}

void loop() {
  timer1 = micros();
  analogRead(0);
  //analogRead(2);
  timer2 = micros();
  //val = (timer2 - timer1);
  Serial.println(timer2 - timer1);
}
