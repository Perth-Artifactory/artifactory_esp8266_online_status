/**
 * Artifactory ESP8266 Online Status
 * by Blake Samuels aka Tazard
 * 
 * Minimal modification of BasicHTTPClient.ino for use as
 * online status tool using statuscake. GETs a URL at 
 * statuscake every 60s to report if Artifactory internet 
 * is online at the space.
 * 
 * Details on getting SHA1 fingerprint so can connect to 
 * HTTPS:
 * https://forum.arduino.cc/index.php?topic=515541.0
 * https://github.com/esp8266/Arduino/issues/2556
 * 
 * Those posts gave these commands (on linux host):
 *  openssl s_client -connect push.statuscake.com:443
 *  nano statuscake-cert.pem (paste key in here)
 *  openssl x509 -noout -in statuscake-cert.pem -fingerprint -sha1
 * Get end bit of:
 *  SHA1 Fingerprint=07:ED:A2:C1:4C:E6:76:FA:C6:F7:02:B3:52:8C:AE:5A:9D:01:96:1D
 * 
 * Date 2019-11-02
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

// Set statuscake URL and SHA1 fingerprint here (as details above)
const char* url = "https://push.statuscake.com/?PK=3dc5296d8883eda&TestID=5273311&time=0";
const char* fingerprint = "07:ED:A2:C1:4C:E6:76:FA:C6:F7:02:B3:52:8C:AE:5A:9D:01:96:1D";


void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Artifactory", "KEEP DOOR CLOSED");

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url

    http.begin(url, fingerprint); //HTTPS
    //http.begin("http://google.com"); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
