/*
------------------------------------------------------------------------------------------------------------------------------------------------
|                    A simple example on how to read continuous measurements from JC-02-1 series laser rangefinder                             |
|                                                                                                                                              |
| WARNING! JC-02-1 sensors operates at 3.3 volts, including TTL level RX/TX. Please use level shifter if communicating with 5V Arduino boards! |
| Please refers to library's README at https://github.com/OdevarJr/JC02-1Rangefinder to see the full list of methods to get data from sensor   |
------------------------------------------------------------------------------------------------------------------------------------------------
*/

#include <JC02-1Rangefinder.h>
#include <SoftwareSerial.h>

SoftwareSerial rf.Serial(10,11); //RX, TX pins to UART communication. Connect ARDUINO RX to RANGEFINDER TX, and ARDUINO TX to RANGEFINDER RX

Rangefinder rf(rf.Serial);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // the module has a default baudrate of 9600
  Serial.println("Starting");
  delay(500); // a litle time to sensor start-up
  rf.begin(); // starts the sensor continuous measuring
}

void loop() {
  float distance = rf.getDistance();
  Serial.print("> Distance = ");
  Serial.println(distance());
}
