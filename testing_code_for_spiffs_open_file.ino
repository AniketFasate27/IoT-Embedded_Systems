#include "SPIFFS.h"
 
//void listAllFiles(){
// 
//  File root = SPIFFS.open("/css");
// 
//  File file = root.openNextFile();
// 
//  while(file){
// 
//      Serial.print("FILE: ");
//      //Serial.println(file.name());
//      String x = file.name();
//      SPIFFS.remove("/x");
//      Serial.println("gusdfufgui");
//      Serial.println(x);
// 
//      file = root.openNextFile();
//  }
// 
//}


 
void setup() {
 
  Serial.begin(115200);
 
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
 
//  File file = SPIFFS.open("/test.txt", FILE_WRITE);
// 
//  if (!file) {
//    Serial.println("There was an error opening the file for writing");
//    return;
//  }
// 
//  if (file.print("some content")) {
//    Serial.println("File was written");
//  } else {
//    Serial.println("File write failed");
//  }
// 
//  file.close();
// 
//  Serial.println("\n\n---BEFORE REMOVING---");
//  listAllFiles();
// 
//  //SPIFFS.remove("/");
// 
//  Serial.println("\n\n---AFTER REMOVING---");
//  listAllFiles();
  File root = SPIFFS.open("/css");
 
  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      //Serial.println(file.name());
      String x = file.name();
      //SPIFFS.remove("/x");
      Serial.println("gusdfufgui");
      Serial.println(x);
 
      file = root.openNextFile();
 
}
}
 
void loop() {
  }
