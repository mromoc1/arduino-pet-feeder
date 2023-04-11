#include "env.h"
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

Servo servo1;
const int httpsPort = 443;

void setup() {
  servo1.attach(2);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(2000);
    Serial.println("Intentando conectar...");
  }
  Serial.println("Conectado");
}

void loop() {
  servo1.write(0);
  if(WiFi.status() == WL_CONNECTED){
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.begin(client, PY_SERVICE, httpsPort, "/status");
    int httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
        if(payload.equals("True")){
          servo1.write(180);
          delay(5000);
          http.begin(client, PY_SERVICE, httpsPort, "/cerrar");
          httpCode = http.GET();
        }
    }
    http.end();    
  }else{
    Serial.println("No hay conexion, reintentando conectar...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED){
      delay(2000);
      Serial.println("Intentando conectar...");
    }
  }
}