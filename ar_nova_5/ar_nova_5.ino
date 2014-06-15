#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

#define pirPin 11
#define lightsOnPin 6
#define disableLightsPin 8
#define manualLightsPin 9
#define luminosityThreshold 1


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
bool lightsDisabled = false;
bool motionRecentlyDetected = false;


void setup(void)
{
    Serial.begin(9600);
    if(!tsl.begin()){
        Serial.print("Check I2C wiring for luminosity sensor");
        while(1);
    }
    pinMode(pirPin, INPUT);
    pinMode(disableLightsPin, INPUT_PULLUP);
    pinMode(manualLightsPin, INPUT_PULLUP);
    pinMode(lightsOnPin, OUTPUT);
    configureLuminositySensor();
}


void loop(void)
{
    if(!lightsDisabled){
        float luminosity = readLuminosity();
        if(readMotion() && !motionRecentlyDetected){
            motionRecentlyDetected = true;
            if(luminosity <= luminosityThreshold){
                sendLightEvent();
            }
        } else {
            motionRecentlyDetected = false;
        }
    }
    readButtons();
}


void configureLuminositySensor(void)
{
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
}


void sendLightEvent(void)
{
    digitalWrite(lightsOnPin, HIGH);
    delay(100);
    digitalWrite(lightsOnPin, LOW);
    Serial.println("Turning the lights on...");
}


bool readMotion(void)
{
    if(digitalRead(pirPin) == HIGH){
        Serial.println("Motion detected");
        return true;
    }
    return false;
}


float readLuminosity(void)
{
    sensors_event_t event;
    tsl.getEvent(&event);
    Serial.print(event.light);
    Serial.println(" lux");
    return event.light;
}


void toggleDisableButton(void)
{
    if(digitalRead(disableLightsPin) == LOW){
        if(lightsDisabled){
            lightsDisabled = false;
            Serial.println("Enalbe lights");
        } else {
            lightsDisabled = true;
            Serial.println("Disable lights");
        }
        delay(250);
    }
}


void manualLightsButton(void)
{
    if(digitalRead(manualLightsPin) == LOW){
        sendLightEvent();
        delay(250);
    }
}


void readButtons(void)
{
    toggleDisableButton();
    manualLightsButton();
}
