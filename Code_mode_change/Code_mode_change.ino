//<--------------------------------------------------------------------------------------------------------------->
//                                        Design Practicum
//                                            Group-65
//                        Voice Automated Mode Changing Add on for Kitchen Water Tap
//<--------------------------------------------------------------------------------------------------------------->
#include "DFRobot_DF2301Q.h"    // Include the DFRobot Voice Recognition sensor library
#include <Servo.h>              // Include the Servo library for controlling servos

// I2C communication for the voice recognition sensor
DFRobot_DF2301Q_I2C asr;
Servo servo;

// Variable to hold the previous command ID
uint8_t previous = 0;
//<--------------------------------------------------------------------------------------------------------------->
void setup() {
    Serial.begin(115200);      // Initialize serial communication at 115200 baud rate
    servo.attach(9);           // Attach the servo to pin 9

    // Initialize the voice recognition sensor
    while (!(asr.begin())) {
        Serial.println("Communication with device failed, please check connection");
        delay(3000);           // Wait for 3 seconds before trying again
    }
    Serial.println("Begin ok!");  // Print a message when the sensor is initialized successfully

    asr.setVolume(100);         // Set the volume of the sensor
    asr.setMuteMode(0);         // Unmute the sensor
    asr.setWakeTime(100);       // Set the wake-up time for the sensor
    uint8_t wakeTime = 0;
    wakeTime = asr.getWakeTime();  // Get the current wake-up time
    Serial.print("wakeTime = ");   // Print the wake-up time
    Serial.println(wakeTime);

    asr.playByCMDID(1);         // Send the wake-up command to the sensor
    asr.playByCMDID(23);        // Play a specific command word ID (23)
}
//<--------------------------------------------------------------------------------------------------------------->
void loop() {
    uint8_t CMDID = asr.getCMDID();  // Get the command ID from the voice recognition sensor

    switch (CMDID) {
        case 5:  // Command ID 5 (Shower mode)
            Serial.println("Shower"); 
            servo.write(120);        // Move the servo to 120 degrees
            previous = CMDID;        // Update the previous command ID
            break;

        case 7:  // Command ID 7 (Spray mode)
            Serial.println("Spray");                                           
            servo.write(180);        // Move the servo to 180 degrees
            previous = CMDID;        // Update the previous command ID
            break;

        case 6:  // Command ID 6 (Default mode)
            Serial.println("Default");
            
            // Coming from shower mode
            if (previous == 5) {
                servo.write(145);    // Move the servo to 145 degrees
            } 
            // Coming from spray mode
            else if (previous == 7) {
                servo.write(155);    // Move the servo to 155 degrees
            }
            // Coming from default mode
            else if (previous == 6) {
                continue;            // Do nothing if already in default mode
            }
            // Neutral (not in any mode)
            else {
                servo.write(150);    // Move the servo to 150 degrees
            }

            previous = CMDID;        // Update the previous command ID
            break;

        default:  // If the command ID is not recognized
            if (CMDID != 0) {
                Serial.print("CMDID = ");  // Print the unknown command ID
                Serial.println(CMDID);
            }
    }
    delay(300);  // Wait for 300 milliseconds before checking again
}
//<--------------------------------------------------------------------------------------------------------------->
