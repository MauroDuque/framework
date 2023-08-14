#include <Arduino.h>
#include "WiFi.h"
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "COGECO-BE210";
const char* password = "68F96COGECO";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char* htmlPage = 
  "<html>"
  "<body>"
  "<form action='/save' method='POST'>"
  "SSID: <input type='text' name='ssid'><br>"
  "Password: <input type='password' name='password'><br>"
  "<input type='submit' value='Save'>"
  "</form>"
  "</body>"
  "</html>";

void setup() {
  Serial.begin(115200);
  Serial.println("Setting AP (Access Point & Station)");
  WiFi.mode(WIFI_AP_STA);
   // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", htmlPage);
  });
  // Start server
  server.begin();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("connected");
  }
  delay(500);
}