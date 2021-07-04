#include "HX711.h"
#include <Servo.h>
Servo myservo;
const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
HX711 scale;
float calibration_factor = 213; // this calibration factor must be adjusted according to your load cell
float units;
String value = "";
int weight_in_int;
String weight_in_str="";
int pos = 0;



void setup() {
Serial.begin(9600);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
Serial.println("HX711 calibration sketch");
scale.set_scale(calibration_factor); //Adjust to this calibration factor
scale.tare(); //Reset the scale to 0

myservo.attach(8);
Serial.println("enter the weight");
myservo.write(0); 
}


void loop()
{
   myservo.write(0); 
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
 Serial.print(weight_in_str);
 Serial.println("grams");
 delay(3000);
}
