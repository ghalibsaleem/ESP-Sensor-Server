#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "DevSensors.h"


const char* ssid = "WiFi Name";
const char* password = "Password";



AsyncWebServer server(80);

void connectWiFi(){
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi ..");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(1000);
	}
	Serial.println(WiFi.localIP());
}



void setup() {
	Serial.begin(115200);
	while (!Serial) { delay(10); }
	WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }
	Serial.println(WiFi.localIP());
	setupSGP();
	setupLuxSensorAuto();
	setupDHTSensor();
	
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
	server.on("/ambientlight", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send_P(200, "text/plain", (String(currentStatus.ambientlight)).c_str());
	});
	server.on("/airquality", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(200, "application/json", currentStatus.airquality);
	});
	
	server.begin();
	
}

String getAirQuality(){
    
    String json = "{\"air_quality\":1,\"pm10\":32,\"pm2_5\":26,\"voc\":" + String(getTVOC()) + "}";
    return json;
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
	currentStatus.temperature = dht.readTemperature();
	currentStatus.humidity = dht.readHumidity();
	currentStatus.eco2 = getCO2();
	currentStatus.tvoc = getTVOC();	
	currentStatus.ambientlight = getLuxDataAuto();	
	currentStatus.airquality = getAirQuality();
	delay(1000);
}

