#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "DevSensors.h"

const char* ssid = "Haveli";
const char* password = "WelcomeToHaveli";

AsyncWebServer server(80);


void connectWiFi(){
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
	Serial.println(WiFi.localIP());
}

void setupServer(){
	//Tester
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
	request->send_P(200, "text/plain", "33");
	});
	
	server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", (String(currentStatus.temperature)).c_str());
	});
	server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", (String(currentStatus.humidity)).c_str());
	});
	server.on("/eco2", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", (String(currentStatus.eco2)).c_str());
	});
	server.on("/co", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", "5");
	});
	server.on("/ambientlight", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", (String(currentStatus.ambientlight)).c_str());
	});
	server.on("/airquality", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(200, "application/json", currentStatus.airquality);
	});
	
	server.begin();
}


void setup() {
	Serial.begin(115200);
	while (!Serial) { delay(10); }
	connectWiFi();
	setupEnvSensor();
	setupLuxSensorAuto();
	setupServer();
	
	
}


void loop() {

	if (WiFi.status() != WL_CONNECTED) {
		Serial.print("Not Connected\n");
		WiFi.disconnect();
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, password);
		if (WiFi.waitForConnectResult() != WL_CONNECTED) {
			Serial.printf("WiFi Failed!\n");
			return;
		}
		Serial.println(WiFi.localIP());
	}
	
	Serial.println("Getting Data:");

	sensors_event_t humidity, temp;
	aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
	currentStatus.temperature = temp.temperature;
	currentStatus.humidity = humidity.relative_humidity;
	
	delay(3000);
	ENS160.setTempAndHum(temp.temperature, humidity.relative_humidity);

	uint8_t Status = ENS160.getENS160Status();
	Serial.print("\nSensor operating status : ");
	Serial.println(Status);

	currentStatus.eco2 = ENS160.getECO2();
	currentStatus.tvoc = ENS160.getTVOC();
	currentStatus.aqi = ENS160.getAQI();
	currentStatus.ambientlight = getLuxDataAuto();	
	currentStatus.airquality = "{\"air_quality\":"+String(currentStatus.aqi)+",\"pm10\":32,\"pm2_5\":26,\"voc\":" + String(currentStatus.tvoc) + "}";
	delay(3000);
}

