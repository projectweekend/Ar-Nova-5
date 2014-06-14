#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

#define pirPin 11
#define lightEventPin 6
#define disableLightsPin 8
#define enableLightsPin 9
#define luminosityThreshold 1


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
bool lightsDisabled = false;


void setup(void)
{
    Serial.begin(9600);
    if(!tsl.begin()){
        Serial.print("Check I2C wiring for luminosity sensor");
        while(1);
    }
    pinMode(pirPin, INPUT);
    pinMode(disableLightsPin, INPUT_PULLUP);
    pinMode(enableLightsPin, INPUT_PULLUP);
    pinMode(lightEventPin, OUTPUT);
    configureLuminositySensor();
}


void loop(void)
{
    readButtons();
    float luminosity = readLuminosity();
    if(motionDetected()){
        Serial.println("Motion detected");
        if(!lightsDisabled && luminosity <= luminosityThreshold){
            Serial.println("Turning the lights on...");
            sendLightEvent();
        }
        delay(10000);
    }
}


void configureLuminositySensor(void)
{
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
}


bool motionDetected(void)
{
    return digitalRead(pirPin) == HIGH;
}


void sendLightEvent(void)
{
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


void readButtons(void)
{
    if(digitalRead(disableLightsPin) == LOW){
        lightsDisabled = true;
        Serial.println("Disable lights");
    } else if(digitalRead(enableLightsPin) == LOW) {
        lightsDisabled = false;
        Serial.println("Enable lights");
    }
}
