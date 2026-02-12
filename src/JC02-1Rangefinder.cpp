/***** ARDUINO LIBRARY FOR JC02-1 Rangefinder Series *****

https://github.com/OdevarJr/JC02-1_rangefinder

License under the MIT license.
**********************************************************/

#include "JC02-1Rangefinder.h"

#define NO_DATA				0
#define MEASUREMENT_STOPPED 1
#define MEASUREMENT_STARTED 2
#define MEASUREMENT_ERROR	3
#define MEASUREMENT_VALID	4
#define FAIL_MEASURE	   -1

// COMMANDS - to controls the rangefinder
const byte beginCmd[]  = {0xAE,0xA7,0x04,0x00,0x0E,0x12,0xBC,0xBE};
const byte stopCmd[]   = {0xAE,0xA7,0x04,0x00,0x0F,0x13,0xBC,0xBE};
const byte singleCmd[] = {0xAE,0xA7,0x04,0x00,0x05,0x09,0xBC,0xBE};

// RESPONSES - from rangefinder to inform it's status
const byte stopCmdOk[]  = {0xAE,0xA7,0x04,0x00,0x8F,0x93,0xBC,0xBE};
const byte beginCmdOk[] = {0xAE,0xA7,0x04,0x00,0x8E,0x92,0xBC,0xBE};
const byte readFail[]   = {0xAE,0xA7,0x04,0x00,0x0E,0x12,0xBC,0xBE};

Rangefinder::Rangefinder(Stream &stream) : _stream(stream) {}



// --- public functions ---
void Rangefinder::stop()   { _sendCommand(stopCmd, sizeof(stopCmd)); }

void Rangefinder::begin()  { _sendCommand(beginCmd, sizeof(beginCmd)); }

void Rangefinder::singleMeasure() { _sendCommand(singleCmd, sizeof(singleCmd)); }

float Rangefinder::getElevation(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) 	return _parseShort(_receivedData[5], _receivedData[6])*0.1;
}

float Rangefinder::getDistance(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[7], _receivedData[8])*0.1;
}

float Rangefinder::getSineHeight(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[9], _receivedData[10])*0.1;
}

float Rangefinder::getHorizontalRange(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[11], _receivedData[12])*0.1;
}

float Rangefinder::getTwoPointHeight(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[13], _receivedData[14])*0.1;
}

float Rangefinder::getAzimuth(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[15], _receivedData[16])*0.1;
}

float Rangefinder::getHorizontalAngle(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[17], _receivedData[18])*0.1;
}

float Rangefinder::getSpan(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[19], _receivedData[20])*0.1;
}

float Rangefinder::getSpeed(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _parseShort(_receivedData[21], _receivedData[22])*0.1;
}

float Rangefinder::getMeasureUnit(){
	_getData();
	if (_status == MEASUREMENT_ERROR) return FAIL_MEASURE;
	else if (_status == MEASUREMENT_VALID) return _receivedData[23];
}
	
int Rangefinder::getStatus(){
	return _status;
}

// --- private functions ---
void Rangefinder::_sendCommand(const byte *cmd, size_t len) {
  _stream.write(cmd, len);
}

void Rangefinder::_getData() {
  if (_stream.available() > 0) {
    _stream.readBytes(_header, sizeof(_header)); // Reads the first four bytes to identify the kind of data from sensor

    // Check if it's a RESPONSE packet
    if ((_header[0]==0xAE) && (_header[1]==0xA7) && (_header[2]==0x04) && (_header[3]==0x00)) {
	  memcpy(_receivedResponse, _header, sizeof(_header));
      _stream.readBytes(_header, sizeof(_header)); // read the next four bytes to complete the response
	  memcpy(&_receivedResponse[4], _header, sizeof(_header));
	  
	  if (memcmp(stopCmdOk,  _receivedResponse, sizeof(_receivedResponse)) == 0) {_status = MEASUREMENT_STOPPED;} // if STOP command was succesfuly
      if (memcmp(beginCmdOk, _receivedResponse, sizeof(_receivedResponse)) == 0) {_status = MEASUREMENT_STARTED;} // if BEGIN (START) command was succesfuly
      if (memcmp(readFail,   _receivedResponse, sizeof(_receivedResponse)) == 0) {_status = MEASUREMENT_ERROR;}   // if measurement error (ex. object too close)
    }

    // If not, check if it's a MEASUREMENT packet
    else if ((_header[0]==0xAE) && (_header[1]==0xA7) && (_header[2]==0x17) && (_header[3]==0x00)) {
      memcpy(_receivedData, _header, sizeof(_header));
      _stream.readBytes(&_receivedData[4], 23); // read the next 23 bytes to complete the measurement data
      _status = MEASUREMENT_VALID; // if valid measurement data was received
    }
  }
  else _status = NO_DATA; // if not, isn't a valid packet
}


// --- Helper ---
int16_t Rangefinder::_parseShort(byte high, byte low) {
return (int16_t)((high << 8) | low);
}
