#include "HX711.h"
#include <Servo.h>
Servo myservo;
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
HX711 scale;
float calibration_factor = 213; // this calibration factor must be adjusted according to your load cell
float units;
String value1 = "";
String value2 = "";
int weight_in_int1;
int weight_in_int2;
int total_weight;
String weight_in_str1="";
String weight_in_str2="";
int pos = 0;


const int liquid = 5;

void setup() {
Serial.begin(9600);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
Serial.println("HX711 calibration sketch");
scale.set_scale(calibration_factor); //Adjust to this calibration factor
scale.tare(); //Reset the scale to 0
pinMode(liquid,OUTPUT);
digitalWrite(liquid,HIGH);
myservo.attach(8);
myservo.write(0);

//servo2.attach(9);
//servo2.write(0);

Serial.println("enter the weight for first container");

}


void loop()
{
while(!Serial.available());
    if(Serial.available())
    {
        weight_in_str1 = Serial.readString();
        weight_in_int1 = weight_in_str1.toInt();
        Serial.print("input value : ");
        Serial.println(weight_in_int1);
        while(!(value1 == "complete"))
        {
            value1 = motor_control1(weight_in_int1);
           
        }
        value1 = "";
    }
    
    Serial.println("enter the weight for first container");
    if(Serial.available())
    {
        weight_in_str2 = Serial.readString();
        weight_in_int2 = weight_in_str2.toInt();
        Serial.print("input value : ");
        Serial.println(weight_in_int2);
        while(!(value2 == "complete"))
        {
          value2 = motor_control2(weight_in_int2);
        }
        value2 = "";
    }
  
}


int motor_control1(int x)
{
    int pos;
    units = scale.get_units(), 5;
    if(units < x)
    {
        myservo.write(180);            
        delay(5);
        Serial.println("motor ON");
   }
   
   else if(units >= x)
   {
        myservo.write(0);
        delay(5);
        Serial.println("motor OFF");
        return "complete";

   }

 Serial.print("Weight: ");
 Serial.print(units);
 Serial.println("grams");
 Serial.print("Weight That you Entered: ");
 Serial.print(weight_in_str1);
 Serial.println("grams");
 delay(3000);
}
int motor_control2(int x)
{
    int pos;
    units = scale.get_units(), 5;
    if(units < x)
    {
        myservo.write(180);            
        delay(5);
        Serial.println("motor ON");
   }
   
   else if(units >= x)
   {
        myservo.write(0);
        delay(5);
        Serial.println("motor OFF");
        return "complete";

   }
 Serial.print("Weight: ");
 Serial.print(units);
 Serial.println("grams");
 Serial.print("Weight That you Entered: ");
 Serial.print(weight_in_str2);
 Serial.println("grams");
 delay(3000);
}
