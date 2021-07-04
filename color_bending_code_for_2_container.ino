#include "HX711.h"
#include <Servo.h>
Servo servo1;
//Servo servo2;
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
HX711 scale;
float calibration_factor = 213; // this calibration factor must be adjusted according to your load cell
float units;
String value = "";
int weight_in_int1;
int weight_in_int2;
int total_weight;
String weight_in_str1="";
String weight_in_str2="";
int pos = 0;
const int liquid = 5; // Here we attach the DC motor for liquid

void setup() 
{
Serial.begin(9600);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
Serial.println("HX711 calibration sketch");
scale.set_scale(calibration_factor); //Adjust to this calibration factor
scale.tare(); //Reset the scale to 0
pinMode(liquid,OUTPUT);
digitalWrite(liquid,HIGH);
servo1.attach(8);
servo1.write(0);
//servo2.attach(9);
//servo2.write(0);
Serial.println("enter the weight for first container");
}


void loop()
{

while(!Serial.available());
    if(Serial.available())
    {
        //Serial.println("enter the weight for first container");
        delay(500);
        weight_in_str1 = Serial.readString();
        weight_in_int1 = weight_in_str1.toInt();
        Serial.print("Container1:- ");
        Serial.println(weight_in_int1);
        delay(200);
        Serial.println("enter the weight for second container");
        delay(2000);
        weight_in_str2 = Serial.readString();
        weight_in_int2 = weight_in_str2.toInt();
        Serial.print("Container2:- ");
        Serial.println(weight_in_int2);
        delay(100);
        total_weight = weight_in_int1 + weight_in_int2;
        Serial.print("Input total Weight:- ");
        Serial.println(total_weight);
        while(!(value == "complete"))
        {
            value = motor_control1(weight_in_int1);
           
        }
        value = "";
        while(!(value == "complete"))
        {
            value = motor_control2(total_weight - weight_in_int1);
           
        }
        value = "";
    }
    
}



int motor_control1(int x)
{
    int pos;
    units = scale.get_units(), 5;
    if(units < x)
    {
        servo1.write(180);            
        delay(5);
        Serial.println("motor 1 ON");
   }
   
   else if(units >= x)
   {
        servo1.write(0);
        delay(5);
        Serial.println("motor 1 OFF");
        return "complete";

   }
 Serial.print("Weight: ");
 Serial.print(units,"grams");
 //Serial.println("grams");
 Serial.print("Weight That you Entered: ");
 Serial.print(total_weight);
 Serial.println("grams");
 delay(300);
}


int motor_control2(int x)
{
    int pos;
    units = scale.get_units(), 5;
    if(units < x)
    {
        //servo2.write(180);   
        digitalWrite(liquid,HIGH);         
        delay(5);
        Serial.println("motor 2 ON");
   }
   
   else if(units >= x)
   {
        //servo2.write(0);
        digitalWrite(liquid,LOW);
        delay(5);
        Serial.println("motor 2 OFF");
        return "complete";

   }
 Serial.print("Weight: ");
 Serial.print(units);
 Serial.println("grams");
 Serial.print("Weight That you Entered: ");
 Serial.print(total_weight);
 Serial.println("grams");
 delay(3000);
}
