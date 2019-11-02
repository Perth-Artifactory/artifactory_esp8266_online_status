# Artifactory ESP8266 Online Status
ESP8266 to hit URL for checking internet is up


Minimal modification of BasicHTTPClient.ino for use as online status tool using statuscake. GETs a URL at statuscake every 60s to report if Artifactory internet is online at the space.

Details on getting SHA1 fingerprint so can connect to HTTPS:
 * https://forum.arduino.cc/index.php?topic=515541.0
 * https://github.com/esp8266/Arduino/issues/2556

Those posts gave these commands (on linux host):
 * `openssl s_client -connect push.statuscake.com:443`
 * `nano statuscake-cert.pem (paste key in here)`
 * `Openssl x509 -noout -in statuscake-cert.pem -fingerprint -sha1`
Get end bit of:
 * `SHA1 Fingerprint=07:ED:A2:C1:4C:E6:76:FA:C6:F7:02:B3:52:8C:AE:5A:9D:01:96:1D`
