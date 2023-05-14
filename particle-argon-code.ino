/**
 * Dog temperature regulator code:
 * The following code will turn on a heater/fan when a dog's
 * temperature exceeds a certain degree for a certain period of time.
 * A notification is sent if the dog is not regulating their temperature
 * with the addition of the fan/heater.
 *
 * University: Deakin University (Burwood)
 * Author: Joshua Piper
 * Student Number: 219135768
 */

// Enable all the pins that will be used
#include <Grove_Temperature_And_Humidity_Sensor.h>

// Enable the dhtpin
#define DHTPIN D8
DHT dht(DHTPIN);

#define blueLedPinOut D5
#define greenLedPinOut D7
#define redLedPinOut D6

#define purpleFanPinOut D2
#define brownHeaterPinOut D3

String PHONE_TRIGGER_NAME = "iPhone-update";

// For measuring seconds elapsed at a certain temperature
// for when a temperature exceeds a certain level. See variables below.
int countTooHot = -1;
int countTooCold = -1;

// Maximum and Minimum temperatures before deciding if a dog is at a
// dangerous temperature
int DANGEROUS_DOG_TOO_COLD_TEMPERATURE = 19;
int DANGEROUS_DOG_TOO_HOT_TEMPERATURE = 45;

// Handles sending alert's, for example, when a dog is too hot
// it will send an alert, but not send another too hot alert
// BUT once temperature goes back down, then a positive alert
// saying the dog is back to normal will be sent
bool shouldSendAlertAboutExcessiveTemperature = false;
bool shouldTerminateApplication = false;
bool hasSentAlertAlready = false;

// Time interval's before activating a fan or heater to
// facilitate a dog's temperature 
int MAXIMUM_HOT_COUNT_5_SECONDS_IN_MICRO_SECONDS = 2000;
int MAXIMUM_COLD_COUNT_5_SECONDS_IN_MICRO_SECONDS = 2000;

// Time interval before sending a time alert. This is not considering
// the time before activating a fan or heater
int MAXIMUM_COUNT_FOR_TEMPERATURE_NOT_REGULATED_TEN_SECONDS = 4000;

// LED code methods for better readability
void turnOnBlueLed() {
    digitalWrite(blueLedPinOut, HIGH);
}

void turnOffBlueLed() {
    digitalWrite(blueLedPinOut, LOW);
}

void turnOnGreenLed() {
    digitalWrite(greenLedPinOut, HIGH);
}

void turnOffGreenLed() {
    digitalWrite(greenLedPinOut, LOW);
}

void turnOnRedLed() {
    digitalWrite(redLedPinOut, HIGH);
}

void turnOffRedLed() {
    digitalWrite(redLedPinOut, LOW);
}

// Turn off all LED's used for resetting purposes.
void turnOffAllLeds() {
    turnOffRedLed();
    turnOffBlueLed();
    turnOffGreenLed();
}

// Fan/Heater code methods for better readability
bool isPurpleFanOn() {
    return digitalRead(purpleFanPinOut);
}

void turnOnPurpleFan() {
    // Not PWN - Hence, less granular control
    // analogWrite(FANPIN, 255);   // to enable PWM for granular control
    digitalWrite(purpleFanPinOut, HIGH);
}

void turnOffPurpleFan() {
    digitalWrite(purpleFanPinOut, LOW);
}

bool isBrownHeaterOn() {
    return digitalRead(brownHeaterPinOut);
}

void turnOnBrownHeater() {
    digitalWrite(brownHeaterPinOut, HIGH);
}

void turnOffBrownHeater() {
    digitalWrite(brownHeaterPinOut, LOW);
}

// Code that initialised all the required data
void setup()
{
    // Turn on LEDS
    pinMode(blueLedPinOut, OUTPUT);
    pinMode(greenLedPinOut, OUTPUT);
    pinMode(redLedPinOut, OUTPUT);

    // Turn on heater and fan
    pinMode(purpleFanPinOut, OUTPUT);
    pinMode(brownHeaterPinOut, OUTPUT);

    // Turn on the serial monitor for debugging purposes

    // Suggested reading:
    // https://docs.particle.io/hardware/expansion/about-serial/
    // is a tutorial to learn how to view the standard output
    Serial.begin(9600);
    dht.begin();
}

// Reset how long a dog has been too hot or too cold
// Used to reset the application
void resetCounts() {
    countTooHot = -1;
    countTooCold = -1;
}

// based on https://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
// convert analog input to celcius temperature
float getCelciusTemperature() {
    return dht.getTempCelcius();
}

// when the dog is too hot then do the following code
bool handleDogTooHot() {
    turnOffAllLeds();
    turnOnRedLed();

    // Dog is too hot scenario in the previous run through as the count isn't initialised
    if (countTooHot != -1) {
        int currentTime = millis();

        int timeElapsedSinceTooHotInitialised = currentTime - countTooHot;
        bool shouldTurnOnFan = timeElapsedSinceTooHotInitialised > MAXIMUM_HOT_COUNT_5_SECONDS_IN_MICRO_SECONDS;

        // only turn on the fan if it isn't on already
        if (shouldTurnOnFan && isPurpleFanOn() == false) {
            turnOnPurpleFan();
        }

        // should only send an alert when it has been too hot and the dog has not cooled down
        return timeElapsedSinceTooHotInitialised > MAXIMUM_COUNT_FOR_TEMPERATURE_NOT_REGULATED_TEN_SECONDS;
    } else {
        // first time the dog has been too hot so initialise everything
        resetCounts();

        countTooHot = millis();
      	// we don't want this debugging message if we have sent an alert about the dog
        // being too hot. Sort of redundant.
		if (hasSentAlertAlready == false) {
		    String initialMessage = "initialising too hot";
			Serial.println(initialMessage);
			
			Particle.publish(PHONE_TRIGGER_NAME, initialMessage);
        }

        // should not send alert
        return false;
    }
}

// when the dog is too cold then do the following code
bool handleDogTooCold() {
    turnOffAllLeds();
    turnOnBlueLed();

    // Dog is too cold scenario in the previous run through
    if (countTooCold != -1) {
        int currentTime = millis();

        int timeElapsedSinceTooColdInitialised = currentTime - countTooCold;
        bool shouldTurnOnHeater = timeElapsedSinceTooColdInitialised > MAXIMUM_COLD_COUNT_5_SECONDS_IN_MICRO_SECONDS;

        if (shouldTurnOnHeater && isBrownHeaterOn() == false) {
            turnOnBrownHeater();
        }

        // should only send an alert when it has been too hot and the dog has not cooled down
        return timeElapsedSinceTooColdInitialised > MAXIMUM_COUNT_FOR_TEMPERATURE_NOT_REGULATED_TEN_SECONDS;
    } else {
        resetCounts();

        countTooCold = millis();
      
      	// we don't want this debugging message if we have sent an alert about the dog
        // being too cold. Sort of redundant.
        if (hasSentAlertAlready == false) {
            String initialMessage = "initialising too cold";
			Serial.println(initialMessage);
			
			Particle.publish(PHONE_TRIGGER_NAME, initialMessage);
        }
        
        // should not sending alert
        return false;
    }
}

void loop()
{
    // Code starts here
    float celciusTemperature = getCelciusTemperature();
    Serial.println(String(celciusTemperature) + " :-)");
    bool isDogOptimalTemperature = celciusTemperature >= DANGEROUS_DOG_TOO_COLD_TEMPERATURE && celciusTemperature <= DANGEROUS_DOG_TOO_HOT_TEMPERATURE;
    bool isDogTooCold = celciusTemperature < DANGEROUS_DOG_TOO_COLD_TEMPERATURE;
    bool isDogTooHot = celciusTemperature > DANGEROUS_DOG_TOO_HOT_TEMPERATURE;

    if (isDogOptimalTemperature) {
        turnOffAllLeds();
        turnOnGreenLed();

        // Turn off any devices that are on when the dog was too hot or too cold
        if (isPurpleFanOn()) {
            turnOffPurpleFan();
        }
        if (isBrownHeaterOn()) {
            turnOffBrownHeater();
        }
      
      	// If we just sent an alert about an excessive temperature
      	// and the dog is now regulated, send an alert about the temperature being
      	// back to normal
        if (hasSentAlertAlready == true) {
            String updateMessage = "Dog is back at a normal temperature. No need to worry! :-)";
          	Serial.println(updateMessage);
          	Particle.publish(PHONE_TRIGGER_NAME, updateMessage);
          	
            // shouldSendAlertAboutExcessiveTemperature is used to know if we should send
            // an alert during this walkthrough
            shouldSendAlertAboutExcessiveTemperature = false;
          	// hasSentAlertAlready = false;
        }

        // Stop measuring millis elapsed of dog's extreme heats
        resetCounts();
      
      	// Reset the sending of alerts so we can switch from too hot to too cold
	    // etc.
      	hasSentAlertAlready = false;

    } else if (isDogTooCold) {
      	bool shouldSendAlert = handleDogTooCold();
      	// Happens when an alert has already been sent. To help notify
      	// when the dog is back to a normal temperature
      	if (shouldSendAlertAboutExcessiveTemperature == false) {
        	shouldSendAlertAboutExcessiveTemperature = shouldSendAlert;
        }
        
    } else if (isDogTooHot) {
      	bool shouldSendAlert = handleDogTooHot();
      	// Happens when an alert has already been sent. To help notify
      	// when the dog is back to a normal temperature
      	if (shouldSendAlertAboutExcessiveTemperature == false) {
        	shouldSendAlertAboutExcessiveTemperature = shouldSendAlert;
        }
    }

    // Build out the message for when we should send out a message
    if (hasSentAlertAlready == false && shouldSendAlertAboutExcessiveTemperature) {
        String dogAtTemperatureMessage = String("Dog is at temperature " + String(celciusTemperature) + " celcius.");
        String temperatureMessage = isDogTooHot == true ? " Dog was too hot" : " Dog was too cold";
      	String finalMessage = String(dogAtTemperatureMessage + temperatureMessage + ". Please check on your pet");

        resetCounts();
		
      	Serial.println(finalMessage);
      	Particle.publish(PHONE_TRIGGER_NAME, finalMessage);
      	hasSentAlertAlready = true;
    }
}
