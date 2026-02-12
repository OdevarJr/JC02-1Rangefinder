# JC-02-1 UART RANGEFINDER MODULE LIBRARY
![JC02-1](https://github.com/OdevarJr/jc02_rangefinder/blob/main/pictures/rangefinder.png)

This library is intended to interface an Arduino board with cheap Chinese JC02-1 UART rangefinder series, that have model with allegated range from 600 to 1200 meters.

### Link to buy the module (last access in 2024/01/28):
https://pt.aliexpress.com/item/1005010430761656.html?spm=a2g0o.order_list.order_list_main.39.79f0caa4c7XaPT&gatewayAdapt=glo2bra
### The original vendor Chinese manuals:
https://github.com/OdevarJr/jc02_rangefinder/blob/main/manual/JC02-1%20%E6%B5%8B%E8%B7%9D%E6%A8%A1%E7%BB%84%E4%B8%AD%E6%96%87%E8%AF%B4%E6%98%8E%E4%B9%A6%20%2020251107%2015%20.pdf
### The original vendor English manual:
https://github.com/OdevarJr/jc02_rangefinder/blob/main/manual/JC02-1%20%E8%8B%B1%E6%96%87.pdf

This library relies on Stream. So, this library is supposed to work with any Arduino/ESP board. Serial or SoftwareSerial should be used depending on the board used.

**WARNING!** The module requires 3.3V, and the UART TTL levels are also 3.3V, and isn't 5V tolerant. Be carefull to provide the proper connections to avoid module damage.

## Object declaration
`Rangefinder myRangefinder(Stream &serial)`

## Functions:

**COMMANDS**
- `begin()`			– Void function. Starts continuous measuring, until a STOP command.
- `stop()`		 	– Void function. Stops the continuous measurings.
- `singleMeasure()` – Void function. With this command, the module will perform only one single measurement, and stops.

**Retrieving data**

-	`getElevation`       :  Returns a *float number* containing the Elevation Angle, in degrees[^1]
-	`getDistance`        :  Returns a *float number* containing the linear distance to the target object, in meters
-	`getSineHeight`      :  Returns a float number containing the Sine Height elevation, in meters[^1]
-	`getHorizontalRange` :  Returns a float number containing the Horizontal Range, in meters[^1]
-	`getTwoPointHeight`  :  Returns a float number containing the Two-Point High, in meters[^1]
-	`getAzimuth`         :  Returns a float number containing the Azimuth, in degrees[^1]
-	`getHorizontalAngle` :  Returns a float number containing the Horizontal Angle, in degrees[^1]
-	`getSpan`            :  Returns a float number containing the Span, in meters[^1]
-	`getSpeed`        	 :  Returns a float number containing the speed/velocity, in Km/h[^1]
-	`getMeasureUnit`     :  Returns a float number containing the module's Linear Measurement unit (1=meters, 2=yards, 3=feet)[^2]
-	`getStatus`     	 :  Returns an integer number representing last data status (0=No data, 1=Continuous measuring started, 2= continuous measuring stopped 3=Measure error 4 =Valid measure)

## Usage example:
```
#include <JC02-1Rangefinder.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial1(10,11); //RX, TX pins to UART communication. Connect ARDUINO RX to RANGEFINDER TX, and ARDUINO TX to RANGEFINDER RX

// Use HardwareSerial (Serial1) or SoftwareSerial if needed
Rangefinder rf(Serial1);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // adjust pins if using SoftwareSerial
  Serial.println("Starting");
  delay(500); // a litle time to sensor start-up
  rf.begin(); // starts the sensor continuous measuring
}

void loop() {
  Serial.print("> Distance = ");
  Serial.println(rf.getDistance());
}
```

[^1]: For JC02-1 series modules, these parameters aren't available. They are kept for compatibility with other high-end models or future firmware upgrades. So all theses commands will return ZERO
[^2]: The linear measurement unit is fixed, depending on module's specs. For JC02-1, this parameter will always return 1 (METERS).