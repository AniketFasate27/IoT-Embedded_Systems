#include "DHTesp.h" //decleration of library
DHTesp dht; //calling for library function

#define relay 12 //Decleare the pin for the pin 12
void setup()
{
Serial.begin(9600); // Decleration for the serial begin
Serial.println(); // print function
Serial.println("Status\t\tHumidity (%)\t\tTemperature (C)\t"); 
dht.setup(14, DHTesp::DHT11); // GPIO14 //decleration of pin D5 of ESP8266 and DHT11 out
pinMode(12,OUTPUT); // GPIO // D6 decleared as output pin 
digitalWrite(12,HIGH); //declration state of pin D6
}

void loop()
{
//delay(dht.getMinimumSamplingPeriod() + 100);
delay(5000);
float humidity = dht.getHumidity();                 //collecting value for humidity
float temperature = dht.getTemperature();          //collecting value for the temprature
 if ( temperature >= 30){
  digitalWrite(12,LOW); // relay will tegereed 
  Serial.println("AC is on");
 }
 else{
  digitalWrite(12,HIGH); // relay is off
  Serial.println("AC is off");
 }

Serial.print("Status: ");                        // printinng the statement 
Serial.print(dht.getStatusString());            // status of the data ok or not
Serial.print("\t");                            // tab in line
Serial.print("Humidity (%): ");               // printing humidity of serial monitor
Serial.print(humidity, 2);
Serial.print("\t");                          // tab in line
Serial.print("Temperature (C): ");           // printing temprature of serial monitor
Serial.print(temperature, 2);
Serial.print("\t");
Serial.println();
}
