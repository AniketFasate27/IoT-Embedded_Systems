

void setup()

{
  Serial.begin(115200);
  pinMode(A0, INPUT);               // Initialize Pin A0 as Input Pin for IR Sensor 1
  pinMode(A1, INPUT);               // Initialize Pin A1 as Input Pin for IR Sensor 2
  pinMode(8, OUTPUT);             // Initialize Pin 8 as Output Pin for Motor Output 2
  pinMode(9, OUTPUT);             // Initialize Pin 9 as Output Pin for Motor Output 2
  pinMode(10, OUTPUT);           // Initialize Pin 10 as Output Pin for Motor Output 1
  pinMode(11, OUTPUT);           // Initialize Pin 11 as Output Pin for Motor Output 1

}




void loop()

{

  // put your main code here, to run repeatedly:

  int a = analogRead(A0);        // Get the values from Pin A0

  int b = analogRead(A1);        // Get the values from Pin A1
  Serial.print("Sesnor a::"); Serial.println(a);
  Serial.print("Sesnor b::"); Serial.println(b);
  delay(500);

  /* The Two variables a and b are used for this particular circuit. When the IR Sensor detects white or any other color the range is less than 100, and when it detects black color, then the Range is above 1000. So, we have taken a Mid-Point Parameter with range of 100. */



  /* digitalWrite is used to write a HIGH or a LOW value to a specified digital pin (LOW and HIGH is the voltage level of the particular pin) */



  if (a < 200 && b < 200) {      // Forward Movement

    digitalWrite(8, HIGH);          // Pin 8 is High for Motor Output 2

    digitalWrite(9, LOW);           // Pin 9 is Low

    digitalWrite(10, HIGH);        // Pin 10 is High for Motor Output 1

    digitalWrite(11, LOW);         // Pin 11 is Low
    Serial.print("Forward Direction");
    
  }

  if (a > 500 && b < 200) {     // Right Movement

    digitalWrite(8, LOW);         // Pin 8 is Low

    digitalWrite(9, LOW);         // Pin 9 is Low

    digitalWrite(10, HIGH);      // Pin 10 is High for Motor Output 1

    digitalWrite(11, LOW);       // Pin 11 is Low
    Serial.print("Right Movement");
  }



  if (a < 200 && b > 500) {     // Left Movement

    digitalWrite(8, HIGH);         // Pin 8 is High for Motor Output 2

    digitalWrite(9, LOW);          // Pin 9 is Low

    digitalWrite(10, LOW);        // Pin 10 is Low

    digitalWrite(11, LOW);        // Pin 11 is Low
    Serial.print("Left Movement");
  }



  if (a > 500 && b > 500) {     // Stop

    digitalWrite(8, LOW);          // Pin 8 is Low

    digitalWrite(9, LOW);          // Pin 9 is Low

    digitalWrite(10, LOW);        // Pin 10 is Low

    digitalWrite(11, LOW);        // Pin 11 is Low
    Serial.print("Stop");
  }



}
