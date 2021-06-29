// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Wire.h>





const int Analog_channel_pin = 35;
float ADC_VALUE = 0;
float voltage_value = 0;



// Replace with your network credentials
const char* ssid = "UCMCM";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



String ADC() {
  ADC_VALUE = analogRead(Analog_channel_pin);
  Serial.print("ADC VALUE = ");
  Serial.println(ADC_VALUE);
  return String(ADC_VALUE);
}

String volta() {

  //ADC_VALUE = analogRead(Analog_channel_pin);
  //Serial.print("ADC VALUE = ");
  //Serial.println(ADC_VALUE);
  voltage_value = (ADC_VALUE * 3.3 ) / (4095);
  Serial.print("Voltage = ");
  Serial.print(voltage_value);
  Serial.print(" volts");
  return String(voltage_value);
  //return String(ADC_VALUE);

}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  WiFi.softAP(ssid, password);                               //Wifi SSID, Password of ESP32
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  
  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS,"/highcharts.js", "text/js");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", ADC().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", volta().c_str());
  });

  server.begin();
}

void loop() {

}
