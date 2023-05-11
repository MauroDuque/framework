// // This sketch provide the functionality of OTA Firmware Upgrade
// #include <Arduino.h>
// #define OTA_TASK_STACK_SIZE 16384
// #include "HttpsOTAUpdate.h"
// // This sketch shows how to implement HTTPS firmware update Over The Air.
// // Please provide your WiFi credentials, https URL to the firmware image and the server certificate.

// static const char *url = "http://communication.enerinno.ca/external/devices/firmware?device=6388e27b8f8cb92a7480d863"; //state url of your firmware image

// static const char *server_certificate = "-----BEGIN PRIVATE KEY-----\n"\
// "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCydoYRUn6HGBju\n"\
// "mcMr7O8RT9IH7Ao7ST6WiT7AuHoRddIf/xSWI7gQwJNZqf/BqMANNJ7x11oQK76V\n"\
// "nDbfebFmvaKJYRFZ0zaePWENgqgP2IPSLGdL/H9B8Fk3pWfsyRjlVQTR74xF1P8w\n"\
// "xCS+qVRhhuLQrCFMVvUT9ebV9SJWH+lt7pKE8yBKZvUTK+dqTI6tBSjG9Pc+j7rS\n"\
// "rm0xEoYzY9ic0ivZtXYjt4X/T4WxK9y6Dk2dtqtyeBgU6RryNckpJLddP7gQe0k4\n"\
// "QlLkdSlsU7trGqyI/al+cDIX/ZQalDeo5N1tihzHStdl8O9np5nPrpuUkgtbtCKf\n"\
// "p5luZgIlAgMBAAECggEBAI6o6pFMVbLek4egyQEDHTTEjwwgJymvamXh/pUGoyXz\n"\
// "/m3vdeBzWz+hAelqWPXRVPz5dJm04d0NawOj5cWNPT/5FcYmHz2j9wRV4pBvQrOw\n"\
// "C+hl8IkwpfXrERbHlaUWOF2TMhA0IvjrxLeSBCGZsMdpkUToZJHm7XDwYH/mvnUD\n"\
// "JP71XkUsvFQwb3z6IE1DlMgtxYClgC87EOVa8JWO4ZGeM45afKZSLcwQTpzNjafr\n"\
// "T2egt8QDg5UiZQ7qbWm6cE4qEkzaZ3pIibZ6qg22poZy/W+rJ6MZUdWiGNlZzyNN\n"\
// "3xw2qJTZkM1kGFDEIHp5H/th5DbypSDWTnnNlq/HquECgYEA5UuhqrISusDE2JQT\n"\
// "MXaMeBcy9ufUNa/vgbWPXtqrq07TNSDDXxp4n48JMEQcq1n/8oJ9wFsccZjXEzH8\n"\
// "A1MumD/YPh0LmeCdtq7Sl+vka2kmGxl4bIe9obXQLwG0abdWqr9zCQ/82sBO1G56\n"\
// "oTgS+xnThrvZpCXWcy3r2yJORWkCgYEAxz9W1/gd4FsYXKFLVrFAD6XF2l8Z3y43\n"\
// "7K++AU1/Cu9HAfU5SwBDcHdIfdgbQ27BCkrgJrzDneYC9C2KfEwmBRxIaSFqQQtH\n"\
// "sWoqbpZ/zewlQZXmhyRB4zpUccx9wH8FL/DFStdwwuyWgpuw6wVPJqQjS+Jd9Amh\n"\
// "i52iMbhQE10CgYBAd6U6ktqFO1Gc+vTygTw5P3yKwt99QLv8IgnvR1fg0Ea9tzp2\n"\
// "8MBdTB/vPBc7bhByJJIqlPNIVQqTUQhj/u0ozTqFLDFo0ChHhpubLy2HrYaTM/QR\n"\
// "v5BnePDRZo9M8ckiNLWpJJ+Y+VywiWddEKD1jGtWhFZKuoCy3YRklh6fkQKBgEX2\n"\
// "+uLJkIwo0MwuI0tk4G7eMjS1MluJWj/1kk3YzaSkUxXf62dnx0alzEHMcf5i1Mxt\n"\
// "cHqJzFMU4rKljzyIbM1cmhy4jBwc/ubQteexV12S754xKgsIc058PAzd1saSk9q6\n"\
// "vDzEnCcGdu+UBjLw4VNbTb2m1oEMJLrm9n5pD01NAoGBAKeltuF2f2Bx3wrDOS/N\n"\
// "2z4SXIZntUyz1vJ0RLj0mHGTbOfwf6nuCn/gjm/E/IOMJ4VRtgS10k+lmUxcsvbr\n"\
// "ApRs6/ns+cRTi7ALcPEkBWbjzpXUSFdHF6vVH13Nf80rG/JrrBUklQKqGQjLbwvs\n"\
// "VM/hoYvKO0u7t6lIoJ/azUcm\n"\
// "-----END PRIVATE KEY-----";

// static HttpsOTAStatus_t otastatus;

// void HttpEvent(HttpEvent_t *event) {
//     switch(event->event_id) {
//         case HTTP_EVENT_ERROR:
//             Serial.println("Http Event Error");
//             break;
//         case HTTP_EVENT_ON_CONNECTED:
//             Serial.println("Http Event On Connected");
//             break;
//         case HTTP_EVENT_HEADER_SENT:
//             Serial.println("Http Event Header Sent");
//             break;
//         case HTTP_EVENT_ON_HEADER:
//             Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
//             break;
//         case HTTP_EVENT_ON_DATA:
//             break;
//         case HTTP_EVENT_ON_FINISH:
//             Serial.println("Http Event On Finish");
//             break;
//         case HTTP_EVENT_DISCONNECTED:
//             Serial.println("Http Event Disconnected");
//             break;
//     }
// }

// void setup_ota() {
//     HttpsOTA.onHttpEvent(HttpEvent);
//     Serial.println("Starting OTA");
//     HttpsOTA.begin(url, server_certificate, false); 
//     Serial.println("Please Wait it takes some time ...");
// }

// void loop_ota(){
//     otastatus = HttpsOTA.status();
//     if(otastatus == HTTPS_OTA_IDLE) Serial.println("OTA IDLE");
//     if(otastatus == HTTPS_OTA_ERR) Serial.println("OTA ERROR");
//     if(otastatus == HTTPS_OTA_UPDATING) Serial.println("OTA UPDATING");
    
//     Serial.print("STATUS--------> ");
//     Serial.println(otastatus);

//     if(otastatus == HTTPS_OTA_SUCCESS) { 
//       Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
//       Serial.print("Rebooting");
//       delay(5000);
//       ESP.restart();
//     } else if(otastatus == HTTPS_OTA_FAIL) { 
//         Serial.println("Firmware Upgrade Fail");
//         delay(5000);
//         Serial.print("Rebooting");
//         ESP.restart();
//     }
//     delay(1000);
// }
