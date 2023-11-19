#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
AsyncWebServer server(80);

String ssid = "ssid";
String ps = "ps";
String gasId = "gasId";
void setup() {
    Serial.begin(9600);

    // Start SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Set up Access Point
    WiFi.softAP("ESP32-Access-Point", "password");

    // Web server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    

    server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
        DynamicJsonDocument jsonDoc(1024);
        jsonDoc["ssid"] = ssid;
        jsonDoc["ps"] = ps;
        jsonDoc["gasId"] = gasId;
        // JSONオブジェクトを文字列に変換
        String jsonResponse;
        serializeJson(jsonDoc, jsonResponse);

        // レスポンスを送信
        request->send(200, "application/json", jsonResponse);
    });

    server.on("/ssidUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
        String newData = request->arg("newData");
        ssid = newData;
        Serial.print("ssid:");
        Serial.println(ssid);
        request->send(200);
    });

    server.on("/psUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
        String newData = request->arg("newData");
        ps = newData;
        Serial.print("ps:");
        Serial.println(ps);
        request->send(200);
    });

    server.on("/gasIdUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
        String newData = request->arg("newData");
        gasId = newData;
        Serial.print("gasId:");
        Serial.println(gasId);
        request->send(200);
    });

    server.begin();
}

void loop() {
    // Additional loop tasks, if any
}
