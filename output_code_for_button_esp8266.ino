#define LED_BUILTIN D4
int count = 0;
void setup() {
  pinMode( D3, INPUT);
  pinMode(D4 ,OUTPUT);
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 int val = digitalRead(D3);

 if (val == 1){
  
  digitalWrite(D4, LOW);
  
  count = count+1;
  Serial.println(count);
  delay(150);
 }
 else{
  
  digitalWrite(D4, HIGH);
  delay(150);
  
 }
}
