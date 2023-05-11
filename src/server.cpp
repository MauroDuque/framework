// Import required libraries
#include "settings.h"
#include "SPIFFS.h"
#include "spiffile.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";

void init_server(){
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String());
    });

    server.serveStatic("/", SPIFFS, "/");

    // Route to load style.css file
    server.on("/wificonnection", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/wifimanager.html", "text/html");
    });

    server.on("/ssid", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        for(int i=0; i < params; i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
            // HTTP POST ssid value
            if (p->name() == PARAM_INPUT_1) {
                String ssid;
                ssid = p->value().c_str();
                Serial.print("SSID set to: ");
                Serial.println(ssid);
                // Write file to save value
                write_file(SPIFFS, get_ssid_path(), ssid.c_str());
            }
            
            if (p->name() == PARAM_INPUT_2) {
                String pass;
                pass = p->value().c_str();
                Serial.print("Password set to: ");
                Serial.println(pass);
                // Write file to save value
                write_file(SPIFFS, get_pass_path(), pass.c_str());
            }
            // HTTP POST ip value
            if (p->name() == PARAM_INPUT_3) {
                String ip;
                ip = p->value().c_str();
                Serial.print("IP Address set to: ");
                Serial.println(ip);
                // Write file to save value
                write_file(SPIFFS, get_ip_path(), ip.c_str());
            }
            // HTTP POST gateway value
            if (p->name() == PARAM_INPUT_4) {
                String gateway;
                gateway = p->value().c_str();
                Serial.print("Gateway set to: ");
                Serial.println(gateway);
                // Write file to save value
                write_file(SPIFFS, get_gateway_path(), gateway.c_str());
            }
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        }

        ESP.restart();
    });

    server.on("/hotspot", HTTP_POST, [](AsyncWebServerRequest *request) {
        
        request->send(200, "text/text", "ok hospot");
    });

    // Services CRUD
    //   server.on("/status", HTTP_GET,[](AsyncWebServerRequest *request){
    //     String json = get_status_data();
    //     request->send(200, "application/json", json);
    //     json = String();
    //   });

    //   server.on("/ssid_scanner", HTTP_GET,[](AsyncWebServerRequest *request){
    //     // Serial.println(list_of_wifi);
    //     // request->send(200, "text/text", "ok");
    //     String json = JSON.stringify(list_of_wifi);
    //     request->send(200, "application/json", json);
    //     json = String();
    //   });

    server.on("/disconnect", HTTP_GET,[](AsyncWebServerRequest *request){
        WiFi.disconnect();
    });

    server.on("/restart", HTTP_GET,[](AsyncWebServerRequest *request){
        ESP.restart();
    });

    server.on("/get_wifi", HTTP_GET,[](AsyncWebServerRequest *request){
        request->send(200, "text/text", "ok");
    });

    // Start server
    server.begin();
}