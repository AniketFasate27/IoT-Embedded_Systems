
#include <WiFi.h>
#include <Update.h>
#include <WiFiClient.h>
#include "OAT_FS.h"

// Variables to validate response
long contentLength = 0;
bool isValidContentType = false;
char Response[500];
String FW_Version;
int FW_Version_Start = 0;
int FW_Version_End = 0;
char FW_Version_Num[4];
int Latest_FW = 0;
const int FW_VERSION = 1;


// S3 Bucket Config
String host = "..............."; // Host => bucket-name.s3.region.amazonaws.com
int ota_port = 80; // Non https. For HTTPS 443. As of today, HTTPS doesn't work.
String bin = "................................"; // bin file name with a slash in front.
String ver = "......................................";



String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}


void checkVersion() {
  timer7 = micros();
  Serial.println("Connecting to: " + String(host));
  // Connect to S3
  if (client2.connect(host.c_str(), ota_port)) {
    // Connection Succeed.
    Serial.println("Fetching Latest Version: " + String(bin));

    // Get the contents of the bin file
    client2.print(String("GET ") + ver + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Cache-Control: no-cache\r\n" +
                  "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client2.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("client2 Timeout !");
        client2.stop();
        return;
      }
    }

    int i = 0;
    while (client2.available()) {
      char c = client2.read();
      Serial.print(c);
      Response[i] = c;
      i = i + 1;
    }

    Serial.print("Length of response received is: "); Serial.println(i);
    for (int j = 1; j < i; j++)
    {
      if (Response[j - 1] == 'F' && Response[j] == 'W')
      {
        FW_Version_Start = j + 3;
      }
      if (FW_Version_Start > 0)
      {
        if ((Response[j] == '*') && (j > FW_Version_Start))
        {
          FW_Version_End = j;
        }
      }
    }

    int len = FW_Version_End - FW_Version_Start;
    for (int k = 0; k < len; k++)
    {
      FW_Version_Num[k] = Response[FW_Version_Start + k];
    }

    FW_Version = String(FW_Version_Num);
    Serial.print("The FW Version is: "); Serial.println(FW_Version);
    Latest_FW = FW_Version.toInt();
    Serial.print("The Integer FW Version is: "); Serial.println(Latest_FW);

  }
}




// OTA Logic
void execOTA() {
  //Serial.println("Disable client connections");

  Serial.println("Connecting to: " + String(host));
  // Connect to S3
  if (client2.connect(host.c_str(), ota_port)) {

    // Fecthing the bin
    Serial.println("Fetching Bin: " +  String(bin));


    // Get the contents of the bin file
    client2.print(String("GET ") + bin + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Cache-Control: no-cache\r\n" +
                  "Connection: close\r\n\r\n");

    // Check what is being sent
    //    Serial.print(String("GET ") + bin + " HTTP/1.1\r\n" +
    //                 "Host: " + host + "\r\n" +
    //                 "Cache-Control: no-cache\r\n" +
    //                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client2.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("client2 Timeout !");
        client2.stop();
        return;
      }
    }
    while (client2.available()) {
      // read line till /n
      line = client2.readStringUntil('\n');
      //line = client2.readStringUntil('\n');
      // remove space, to check if the line is end of headers
      line.trim();
      //    read back one line from the server
      // if the the line is empty,
      // this is end of headers
      // break the while and feed the
      // remaining `client22` to the
      // Update.writeStream();
      if (!line.length()) {
        //headers ended
        break; // and get the OTA started
      }
      // Check if the HTTP Response is 200
      // else break and Exit Update
      if (line.startsWith("HTTP/1.1")) {

        if (line.indexOf("200") < 0) {
          Serial.println("Got a non 200 status code from server. Exiting OTA Update.");
          break;
        }
      }
      // extract headers here
      // Start with content length
      if (line.startsWith("Content-Length: ")) {

        contentLength = atol((getHeaderValue(line, "Content-Length: ")).c_str());
        Serial.println("Got " + String(contentLength) + " bytes from server");
      }
      // Next, the content type
      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");
        Serial.println("Got " + contentType + " payload.");
        if (contentType == "text/html" || contentType == "application/octet-stream" || contentType == "text/css" || contentType == "application/javascript" || contentType == "image/x-icon payload") {
          isValidContentType = true;
        }
      }
    }
  }
  else {
    // Connect to S3 failed
    // May be try?
    // Probably a choppy network?
    Serial.println("Connection to " + String(host) + " failed. Please check your setup");
    // retry??
  }

  // Check what is the contentLength and if content type is `application/octet-stream`
  Serial.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

  // check contentLength and content type
  if (contentLength && isValidContentType) {
    // Check if there is enough to OTA Update
    Serial.println("update started");
    bool canBegin = Update.begin(contentLength);

    // If yes, begin
    if (canBegin) {
      //esp_task_wdt_feed();
      Serial.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
      //delay(1);
      // No activity would appear on the Serial monitor
      // So be patient. This may take 2 - 5mins to complete

      line = client2.readString();
      Serial.println(line);
      const char * x = line.c_str();
      writeFile(SPIFFS, filename, x);// here we have to provide the file name for sketch folder

      if (Update.end()) {
        Serial.println("OTA done!");
        OTA_flag = false;
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
          timer8 = micros();
          valota = (timer8 - timer7);
          Serial.print("ota time ");
          Serial.print(valota);
          // ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        //Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      // not enough space to begin OTA
      // Understand the partitions and
      // space availability
      Serial.println("Not enough space to begin OTA");
      client2.flush();
    }
  } else {
    Serial.println("There was no content in the response");
    client2.flush();
  }


}

// OTA Logic
void execOTA2() {
  bin3 = "/generalai.in/site_survey_kit/ssk.bin";
  Serial.println("Connecting to: " + String(host));
  // Connect to S3
  if (client2.connect(host.c_str(), ota_port)) {

    // Fecthing the bin
    Serial.println("Fetching Bin: " + String(bin3));

    // Get the contents of the bin file
    client2.print(String("GET ") + bin3 + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Cache-Control: no-cache\r\n" +
                  "Connection: close\r\n\r\n");

    // Check what is being sent
    //    Serial.print(String("GET ") + bin + " HTTP/1.1\r\n" +
    //                 "Host: " + host + "\r\n" +
    //                 "Cache-Control: no-cache\r\n" +
    //                 "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client2.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("client2 Timeout !");
        client2.stop();
        return;
      }
    }
    // Once the response is available,
    // check stuff

    /*
       Response Structure
        HTTP/1.1 200 OK
        x-amz-id-2: NVKxnU1aIQMmpGKhSwpCBh8y2JPbak18QLIfE+OiUDOos+7UftZKjtCFqrwsGOZRN5Zee0jpTd0=
        x-amz-request-id: 2D56B47560B764EC
        Date: Wed, 14 Jun 2017 03:33:59 GMT
        Last-Modified: Fri, 02 Jun 2017 14:50:11 GMT
        ETag: "d2afebbaaebc38cd669ce36727152af9"
        Accept-Ranges: bytes
        Content-Type: application/octet-stream
        Content-Length: 357280
        Server: AmazonS3

        {{BIN FILE CONTENTS}}
    */
    while (client2.available()) {
      // read line till /n
      String line = client2.readStringUntil('\n');
      // remove space, to check if the line is end of headers
      line.trim();

      // if the the line is empty,
      // this is end of headers
      // break the while and feed the
      // remaining `client2` to the
      // Update.writeStream();
      if (!line.length()) {
        //headers ended
        break; // and get the OTA started
      }

      // Check if the HTTP Response is 200
      // else break and Exit Update
      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
          Serial.println("Got a non 200 status code from server. Exiting OTA Update.");
          break;
        }
      }

      // extract headers here
      // Start with content length
      if (line.startsWith("Content-Length: ")) {
        contentLength = atol((getHeaderValue(line, "Content-Length: ")).c_str());
        Serial.println("Got " + String(contentLength) + " bytes from server");
      }

      // Next, the content type
      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");
        Serial.println("Got " + contentType + " payload.");
        if (contentType == "application/octet-stream") {
          isValidContentType = true;
        }
      }
    }
  } else {
    // Connect to S3 failed
    // May be try?
    // Probably a choppy network?
    Serial.println("Connection to " + String(host) + " failed. Please check your setup");
    // retry??
    // execOTA();
  }

  // Check what is the contentLength and if content type is `application/octet-stream`
  Serial.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

  // check contentLength and content type
  if (contentLength && isValidContentType) {
    // Check if there is enough to OTA Update
    bool canBegin = Update.begin(contentLength);

    // If yes, begin
    if (canBegin) {
      Serial.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
      delay(1);
      // No activity would appear on the Serial monitor
      // So be patient. This may take 2 - 5mins to complete
      size_t written = Update.writeStream(client2);
      delay(1);
      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?" );
        // retry??
        // execOTA();
      }

      if (Update.end()) {
        Serial.println("OTA done!");
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
          // ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      // not enough space to begin OTA
      // Understand the partitions and
      // space availability
      Serial.println("Not enough space to begin OTA");
      client2.flush();
    }
  } else {
    Serial.println("There was no content in the response");
    client2.flush();
  }
}


void OTA_update() {
  listDir(SPIFFS, "/", 0);

  listAllFiles();

  //deleteFile(SPIFFS, "/");
  //  deleteFile(SPIFFS, "/");
  //  deleteFile(SPIFFS, "/");
  //Serial.println("Alll files deleted");
  //listDir(SPIFFS, "/", 0);
  //Serial.println("Disable client connections");
  listAllFiles();
  listDir(SPIFFS, "/", 0);
  String bin1 = "/generalai.in/site_survey_kit/data.txt";
  Serial.println("Connecting to: " + String(host));
  // Connect to S3
  if (client2.connect(host.c_str(), ota_port)) {

    // Fecthing the bin
    Serial.println("Fetching Bin: " +  String(bin1));


    // Get the contents of the bin file
    client2.print(String("GET ") + bin1 + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Cache-Control: no-cache\r\n" +
                  "Connection: close\r\n\r\n");

    // Check what is being sent
    //    Serial.print(String("GET ") + bin + " HTTP/1.1\r\n" +
    //                 "Host: " + host + "\r\n" +
    //                 "Cache-Control: no-cache\r\n" +
    //                 "Connection: close\r\n\r\n");



    unsigned long timeout = millis();
    while (client2.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("client2 Timeout !");
        client2.stop();
        return;
      }
    }


    // Once the response is available,
    // check stuff


    //String line = client2.readString();

    //Serial.println("i am here 2");
    while (client2.available()) {
      // read line till /n
      //line = client2.readStringUntil('\n');
      //line = client2.readStringUntil('\n');
      // remove space, to check if the line is end of headers
      //line.trim();

      // Define
      //String str = "This is my string";
      //
      //// Length (with one extra character for the null terminator)
      //int str_len = str.length() + 1;
      //
      //// Prepare the character array (the buffer)
      //char char_array[str_len];
      //
      //// Copy it over
      //str.toCharArray(char_array, str_len);



      String line = client2.readString();
      line.trim();
      //Serial.println(line);
      int str_len = line.length();
      //Serial.println(str_len);


      while (k >= 0) {
        k = line.indexOf('\n');


        //       new_line = line.substring(0,k);
        bin = line.substring(0, k);
        m = bin.length();

        l = bin.lastIndexOf('/');
        name_file = bin.substring(l, m);

        filename = name_file.c_str();


        rem_line = line.substring(k + 1, str_len);
        line = rem_line;


        //count = count + 1;
        //        Serial.print("index :");
        //        Serial.println(bin.indexOf('\n'));
        //        Serial.print("lenght :");
        //         Serial.println(bin.length());


        //        Serial.print("k :");
        //        Serial.println(k);
        //        Serial.print("m :");
        //        Serial.println(m);
        //        Serial.print("l :");
        //        Serial.println(l);
        //        Serial.print("new line");
        //        Serial.println(bin);
        //        Serial.print("file name");
        //        Serial.println(filename);


        if (m >= 35) {
          //vTaskDelay(1);
          execOTA();
          //vTaskDelay(1);
        }

        //  for (int j = 0; j <= 10; j++) {
        //    if (j == 0) {
        //      bin = "/generalai.in/site_survey_kit/data/main.css";
        //      filename = "/main.css";
        //      execOTA();
        //      Serial.println("1");
        //
        //    }
        //
        //    else if (j == 1) {
        //      bin = "/generalai.in/site_survey_kit/data/httppost9.js";
        //      String name1 = "/httppost9.js";
        //      filename = name1.c_str();
        //      //filename ="/httppost.js";
        //      execOTA();
        //      Serial.println("5");
        //    }
        //
        //    else if (j == 2) {
        //      bin = "/generalai.in/site_survey_kit/data/mip-hub-new.html";
        //      String name1 = "/mip-hub-new.html";
        //      filename = name1.c_str();
        //      //filename ="/mip-hub-new.html";
        //      execOTA();
        //      Serial.println("10");
        //    }
        //
        //    else if (j == 3) {
        //      bin = "/generalai.in/site_survey_kit/ssk.bin"; // bin file name with a slash in front.
        //      //      String name1 = "/ssk.bin";
        //      //      filename = name1.c_str();
        //      execOTA2();
        //
        //      Serial.println("Done Copmleted it will restart");
        //
        //
        //    }
        //  }
        //ESP.restart();
      }
    }
  }
  listDir(SPIFFS, "/", 0);
  execOTA2();
  ESP.restart();
}
    void Setup() {
      const char * OTAssid = ota1.c_str();
      const char * OTApassword = ota2.c_str();
      Serial.println(OTAssid);
      Serial.println(OTApassword);
      Serial.println("it is not started");
      WiFi.begin(OTAssid, OTApassword);
      while (WiFi.status() != WL_CONNECTED) {
        //    //delay(500);

        Serial.println("not connected to wifi");
      }

      Serial.println("Connected to wifi Checking");
      checkVersion();

      if (Latest_FW > FW_VERSION)
      {
        OTA_update();
      }
      else {
        ESP.restart();
        Serial.println("OTA not done");
      }
    }


    // Utility to extract header value from headers
