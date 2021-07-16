#include <Wire.h>
#include <Adafruit_ADS1015.h>

 Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

void setup(void) 
{
  Serial.begin(115200);
     ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  
  ads.begin();
}

void loop(void) 
{
  int16_t adc0;

  adc0 = ads.readADC_SingleEnded(0);
  
  Serial.println(adc0);
  
  
  
}
