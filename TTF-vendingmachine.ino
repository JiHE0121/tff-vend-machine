/*
 * TinyFilmFestival - Hejin Jo
 */

#include "TinyFilmFestival.h"
#include "HCSR04.h"
#include "access.h"
#include "poop.h"
#include "battery.h"
#include "drink.h"
#include "boom.h"


TinyFilmFestival film;

// Create Animation objects
Animation accessAnim = access;
Animation poopAnim = poop;
Animation batteryAnim = battery;
Animation drinkAnim = drink;
Animation boomAnim = boom;

// Distance sensor setup
int triggerPin = 14;          // Trigger pin
int echoPin = 15;             // Echo pin
int maxDistance = 200;        // Maximum distance to measure (cm)
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin, maxDistance);

// Distance control variables (cm)
float distanceThreshold0 = 5.0;
float distanceThreshold1 = 20.0;
float distanceThreshold2 = 30.0;
float distanceThreshold3 = 55.0;

int sensorInterval = 100;         // How often to read sensor (ms)
unsigned long lastSensorRead = 0;   // Last sensor reading time
float currentDistance = 0.0;        // Variable to store distance reading

// ============================
void setup() 
{
    Serial.begin(9600); // Initialize serial for debug output
    film.begin(); // Initialize the LED matrix
    
    // Start with battery animation
    film.startAnimation(batteryAnim, LOOP);
    Serial.println("Starting with battery animation");
}

void loop() 
{
    // Read distance value with timing control
    float distance = readDistance();
    
    if (distance >= 0)
    {
        Serial.print("Distance : ");
        Serial.print(distance);
        Serial.println(" cm");

        // Very Close Range: 0-5 cm =======================
        // Triggers most dynamic animation for closest interactions
        if ((distance >= 0) && (distance < distanceThreshold0))
        {
            film.startAnimation(boomAnim, LOOP);
            Serial.println("Playing Boom (Very Close)");
        }

        // Close Range: 5-20 cm =======================
        // Active interaction range for direct manipulation
        else if ((distance >= distanceThreshold0) && (distance < distanceThreshold1))
        {
            film.startAnimation(poopAnim, LOOP);
            Serial.println("Playing Poop (Close)");
        }

        // Close Range: 20 to 30 cm =======================
        // Active interaction range for direct manipulation
        else if ((distance >= distanceThreshold1) && (distance < distanceThreshold2))
        {
            film.startAnimation(drinkAnim, LOOP);
            Serial.println("Playing Drink (Close)");
        }

        // Medium Range: 30 to 55 cm =========================
        // Transitional range for approaching interactions
        else if ((distance >= distanceThreshold2) && (distance < distanceThreshold3))
        {
            film.startAnimation(accessAnim, LOOP);
            Serial.println("Playing Access (Medium)");
        }
        // Far Range: 55cm and beyond =========================
        // Default state for no active interaction
        else if (distance >= distanceThreshold3)
        {
            film.startAnimation(batteryAnim, LOOP);
            Serial.println("Playing Battery (Far)");
        }
    }
    
    film.update(); // update the animation frame
}

// Reads distance from the HC-SR04 sensor at specified intervals
// Function returns float since HC-SR04 can measure partial centimeters
float readDistance()
{
    unsigned long currentTime = millis();
    
    // Only read sensor if interval has elapsed
    if (currentTime - lastSensorRead >= sensorInterval)
    {
        currentDistance = distanceSensor.measureDistanceCm();
        lastSensorRead = currentTime;
    }
    
    return currentDistance;
}