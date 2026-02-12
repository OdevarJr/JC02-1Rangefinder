/*
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
|                                                      JC-02-1 series laser rangefinder library examples                                                          |
| A more complex example that uses the full library capabilities. Here we get the module's STATUS to control the measuring behavior:                              |
| the library returns the following status: 0=No data available 1=continuous measuring started 2=continuous measuring stopped 3=measuring error 4=valid measuring |
|                                                                                                                                                                 |
| WARNING! JC-02-1 sensors operates at 3.3 volts, including TTL level RX/TX. Please use level shifter if communicating with 5V Arduino boards!                    |
| Please refers to library's README at https://github.com/OdevarJr/JC02-1Rangefinder to see the full list of methods to get data from sensor                      |
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <JC02-1Rangefinder.h>
#include <SoftwareSerial.h>

SoftwareSerial rfSerial(10,11); //RX, TX
Rangefinder rf(rfSerial);

void setup() {
  Serial.begin(115200);
  delay(500);
  rfSerial.begin(9600); // the module has a default baudrate of 9600bps
  // functions menu
  Serial.println("**  JC02-1 LASER RANGEFINDER MENU  **");
  Serial.println(">SELECT:");
  Serial.println("1 - To start continuous measurements reading");
  Serial.println("2 - To stop continuous Measurements reading");
  Serial.println("3 - To get a single measurement");
  Serial.println();
}

void loop() {
  sendCommand();
  getData();
}

void sendCommand(){
  if(Serial.available()>0){
    char command = Serial.read();
    switch(command){
      case '1':
        rf.begin();
        break;
      case '2':
        rf.stop();
        break;
      case '3':
        rf.singleMeasure();
        break;
      default:
        break;
    }
  }
}

// functions to communicate with library
void getData(){
int status = rf.getStatus();
float distance = rf.getDistance();
//if (status == 0) Serial.println("NO DATA");
if (status == 1) Serial.println("Stopped");
if (status == 2) Serial.println("Started");
if (status == 3) Serial.println("Measure error");
if (status == 4){
  Serial.print("> Distance = ");
  Serial.println(distance);
  }
}
