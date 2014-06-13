#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

#define pirPin 4
#define lightEventPin 6
#define luminosityThreshold 1


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


void setup(void)
{
    if(!tsl.begin()){
        Serial.print("Check I2C wiring for luminosity sensor");
        while(1);
    }
    pinMode(pirPin, INPUT);
    pinMode(lightEventPin, OUTPUT);
    configureLuminositySensor();
}


void loop(void)
{
    float luminosity = readLuminosity();

    if(motionDetected()){
        if(luminosity <= luminosityThreshold){
            sendLightEvent();
        }
    }
}


void configureLuminositySensor(void)
{
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
}


bool motionDetected(void)
{
    Serial.println("Motion detected");
    return digitalRead(pirPin);
}


void sendLightEvent(void)
{
    Serial.println("Sending light event");
    digitalWrite(lightEventPin, HIGH);
    delay(100);
    digitalWrite(lightEventPin, LOW);
}


float readLuminosity(void)
{
    sensors_event_t event;
    tsl.getEvent(&event);
    Serial.print(event.light);
    Serial.println(" lux");
    return event.light;
}
