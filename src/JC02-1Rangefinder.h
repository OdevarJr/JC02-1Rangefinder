/***** ARDUINO LIBRARY FOR JC02-1 Rangefinder Series *****

https://github.com/OdevarJr/JC02-1_rangefinder

License under the MIT license.
**********************************************************/

#ifndef JC02-1Rangefinder_h
#define JC02-1Rangefinder_h

#include <Arduino.h>

class Rangefinder {
  public:
    Rangefinder(Stream &stream);

	// commands to sensor
    void begin();
    void stop();
    void singleMeasure();

    // getting data from sensor. For JC02-1 series, distances are in meters. Angles in degrees
    float getElevation();
    float getDistance();
    float getSineHeight();
    float getHorizontalRange();
    float getTwoPointHeight();
    float getAzimuth();
    float getHorizontalAngle();
    float getSpan();
    float getSpeed();
    float getMeasureUnit(); // the JC02-1 series will always return 1 (meters) as distance unit. Other models may return inches(2) or feet(3)
	int	  getStatus();

  private:
    Stream &_stream;
	
    byte _receivedData[27]; // holds measured values
    byte _receivedResponse[8]; // holds sensor responses
    byte _header[4]; // holds the headers bytes to determine if a resonse or a measure was received
	int  _status; // holds the current status info(0=no data, 1=stopped, 2=started, 3=measure error, 4=measure ok)

    void 	_sendCommand(const byte *cmd, size_t len);
	void 	_getData(); // Data acquiring
	int16_t _parseShort(byte high, byte low);
};

#endif