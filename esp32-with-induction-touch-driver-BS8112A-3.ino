#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Define constants and variables
#define BUZZER_PIN 23  // Buzzer pin

bool userInputEnabled = true;  // Flag to enable/disable user inputs
uint16_t lastKeyStatus = 0;  // Store the last key status
unsigned long buttonPressStart = 0;  // Time when button press started
const unsigned long longPressThreshold = 2000;  // Long press threshold in milliseconds
bool buttonHeld = false;  // Tracks if the button is held

// Buzzer function with duration
void buzzIndicator(int duration) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
}

// Read which button is pressed
int readButtonPress() {
    uint16_t keyStatus = (readRegister(0x08) | (readRegister(0x09) << 8));
    if (keyStatus != 0xFFFF && keyStatus != 0) {
        return keyStatus;  // Return the detected button status
    }
    return 0;  // No button pressed
}

// Main function to process touch inputs
int processTouch() {
    if (!userInputEnabled) return 0;

    int buttonId = readButtonPress();

    if (buttonId != 0) {  // A button is pressed
        if (!buttonHeld) {  // New button press
            buttonPressStart = millis();  // Record the start time
            buttonHeld = true;  // Mark button as held
            buzzIndicator(100);  // Trigger buzzer for the initial press
            return buttonId;  // Return positive value for short press immediately
        } else {  // Button is being held
            unsigned long pressDuration = millis() - buttonPressStart;

            if (pressDuration >= longPressThreshold) {  // Long press detected
                buzzIndicator(300);  // Longer buzz for long press
                buttonPressStart = millis();  // Reset start time to avoid repeated long press events
                return -buttonId;  // Return negative value for long press
            }
        }
    } else {  // No button is pressed
        if (buttonHeld) {  // Button was released
            buttonHeld = false;  // Reset the button held state
            buttonPressStart = 0;  // Reset the press timer
        }
    }

    return 0;  // No new button press detected
}

// Write to a register (for BS8112A-3)
void writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(0x50);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Read from a register (for BS8112A-3)
uint8_t readRegister(uint8_t reg) {
    Wire.beginTransmission(0x50);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(0x50, (uint8_t)1);
    return (Wire.available() ? Wire.read() : 0xFF);
}

// Setup function
void setup() {
    Serial.begin(115200);
    Wire.begin();

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    calibrateTouchSensitivity(100);  // Default sensitivity calibration
}

// Calibrate touch sensitivity
void calibrateTouchSensitivity(int level) {
    if (level < 8 || level > 63) return;

    for (uint8_t i = 0; i < 12; i++) {
        uint8_t regAddress = 0xB5 + i;  // Calculate register address for each key
        writeRegister(regAddress, level);
    }
}

// Loop function
void loop() {
    int result = processTouch();
    if (result > 0) {
        Serial.println("Short press detected for button: " + String(result));
    } else if (result < 0) {
        Serial.println("Long press detected for button: " + String(-result));
    }
}