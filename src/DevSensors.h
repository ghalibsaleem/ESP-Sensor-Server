
#include "Adafruit_VEML7700.h"
#include <Adafruit_Sensor.h>

#define DHTPIN 4  
#define DHTTYPE    DHT22 

#define SDA_1 15
#define SCL_1 16
#define I2C_FREQ_1 400000

#define SDA_2 17
#define SCL_2 18
#define I2C_FREQ_2 400000

struct SensorStatus {   
  float temperature = 10;           
  float humidity = 10;     
  float eco2 = 10;  
  float tvoc = 10;
  float ambientlight = 10;
  String airquality;
} currentStatus;


Adafruit_VEML7700 veml = Adafruit_VEML7700();
TwoWire I2C_1 = TwoWire(0);


TwoWire I2C_2 = TwoWire(1);



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



float getCO2(){
    
    return 0;
}

float getTVOC(){
    
    return 0;
}