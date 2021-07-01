#include <Wire.h>

#include <Servo.h>
Servo myservo;

#include <LCD_I2C.h>
#define SCL A5
#define SDA A4
LCD_I2C lcd(0x27);


const int Tare = 2;
#include <HX711.h>

#define LOADCELL_DOUT_PIN  12

#define LOADCELL_SCK_PIN  11

HX711 scale;

float CalibrationFactor = 207500;  // Replace -12000 the calibration factor.

float units;
String value = "";
int weight_in_int;
String weight_in_str="";
int pos = 0;

void setup()

{

Serial.begin(9600); 
Serial.println("");

lcd.begin();
lcd.backlight();
pinMode(Tare, INPUT);
digitalWrite(Tare, HIGH);

lcd.setCursor(0, 0);
lcd.print("  Weight Scale");
lcd.setCursor(0, 1);
lcd.print("    Machine");
delay(2000);
lcd.print("Calibrartion stated");


scale.set_scale(CalibrationFactor);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
scale.tare();

myservo.attach(8);
myservo.write(0);
Serial.println("enter the weight");
lcd.print("Enter the weight");

}

void loop()
{ 
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Weight:");


  while(!Serial.available());
    if(Serial.available())
    {
        weight_in_str = Serial.readString();
        weight_in_int = weight_in_str.toInt();
        Serial.print("input value : ");
        Serial.println(weight_in_int);
        while(!(value == "complete"))
        {
            value = motor_control(weight_in_int);
           
        }
        value = "";
    }

}


int motor_control(int x)
{
    int pos;
    units = scale.get_units(), 5;
    if(units < x)
    {
        myservo.write(180);            
        delay(5);
        Serial.println("motor ON");
        lcd.print("motor On");
        
   }
   
   else if(units >= x)
   {
        myservo.write(0);
        delay(5);
        Serial.println("motor OFF");
        lcd.print(" motor OFF");
        return "complete";

   }

 Serial.print("Weight: ");
 Serial.print(units);
 Serial.println("grams");
 Serial.print("Weight That you Entered: ");
 Serial.print(weight_in_str);
 Serial.println("grams");


 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print(" Weights ");
 lcd.print(units);
 lcd.print("g");
 lcd.setCursor(0, 1);
 lcd.print("Enter");
 lcd.print(weight_in_str);
 lcd.print("g");
 delay(3000);

delay(200);





if (digitalRead(Tare) == LOW)

{

scale.tare();

lcd.clear();

lcd.setCursor(0, 0);

lcd.print("Tare ......");

lcd.setCursor(0, 1);

lcd.print("Setting to 0 Kg.");

delay(1000);

}

}
