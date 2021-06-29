/*************************************************************************

   AC Voltmeter with Arduino.
   This is a free software with NO WARRANTY.
   USE AT YOUR OWN RISK!
   https://simple-circuit.com/

 ************************************************************************/


#include <driver/adc.h>
#include "esp_adc_cal.h"
float Voltage = 0.0;


int a = 0;
void setup(void) {
  Serial.begin(115200);
//  dc1_config_width(ADC_WIDTH_BIT_12);
//  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);
//  int val = adc1_get_raw(ADC1_CHANNEL_0);

}
// get maximum reading value
uint16_t get_max() {

  uint16_t max_v = 0;
  for (uint8_t i = 0; i < 100; i++) {
    uint16_t r = analogRead(35);  // read from analog channel 3 (A3)
    if (max_v < r) max_v = r;
    delayMicroseconds(200);
  }
  return max_v;
}

// main loop
void loop() {

  char buf[10];
  // get amplitude (maximum - or peak value)
  uint32_t v = get_max();
  // get actual voltage (ADC voltage reference = 1.1V)
  a = v * 1100 / 1023;
  // calculate the RMS value ( = peak/√2 )
  //v /= sqrt(2);
  Serial.print("Volatage :: "); Serial.println(a);
  Voltage = readADC_Cal(v);
  Serial.print("Volatage 2:: "); Serial.println(a);
  delay(100);

}

// end of code.
