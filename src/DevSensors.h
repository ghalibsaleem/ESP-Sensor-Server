
#include "Adafruit_VEML7700.h"
#include <Adafruit_Sensor.h>
#include <DFRobot_ENS160.h>
#include <Adafruit_AHTX0.h>

#define DHTPIN 4  
#define DHTTYPE    DHT22 

#define SDA_1 47
#define SCL_1 19
#define I2C_FREQ_1 400000

#define SDA_2 37
#define SCL_2 38
#define I2C_FREQ_2 400000

struct SensorStatus {   
  float temperature = 10;           
  float humidity = 10;     
  float eco2 = 10;  
  float tvoc = 10;
  float ambientlight = 10;
  float aqi = 1;
  String airquality;
} currentStatus;

TwoWire I2C_1 = TwoWire(0);
Adafruit_VEML7700 veml = Adafruit_VEML7700();

TwoWire I2C_2 = TwoWire(1);
DFRobot_ENS160_I2C ENS160(&I2C_2, 0x53);

Adafruit_AHTX0 aht;



void setupLuxSensorAuto(){
    I2C_1.begin(SDA_1, SCL_1, I2C_FREQ_1);
    if (!veml.begin(&I2C_1)) {
        Serial.println("VEML7700 Auto Lux Sensor not found");
        while (1){
            delay(500);
        }
    }
}

float getLuxDataAuto(){
    float lux = veml.readLux(VEML_LUX_AUTO);
    //Serial.println(lux);
    if (lux < 0.0001)
        return 0.0001;

    return lux;
}


void setupEnvSensor(){
    I2C_2.begin(SDA_2, SCL_2, I2C_FREQ_2);
    while( NO_ERR != ENS160.begin() ){
		Serial.println("Communication with ENS160 failed, please check connection");
		delay(3000);
	}
    ENS160.setPWRMode(ENS160_STANDARD_MODE);

    if (! aht.begin(&I2C_2)) {
		Serial.println("Could not find AHT? Check wiring");
		while (1) delay(10);
	}
}
uint16_t getSensorStatus(){
    return ENS160.getENS160Status();
}