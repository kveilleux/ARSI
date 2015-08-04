/* 
* BMP180.h
*
* Created: 8/3/2015 6:18:49 PM
* Author: Kyle
*/


#ifndef __BMP180_H__
#define __BMP180_H__

#include "i2cmaster.h"
#include <util/delay.h>

#define DevBMPRead 0xEF
#define DevBMPWrite 0xEE

class BMP180
{
	// Our calibration data read from the BMP180 Sensor
	int16_t calibData[11];

	// Uncompensated temperature value from the BMP180 Sensor
	int UCTemp;

	// Retrieves calibration data from the sensor (should be hard coded into the device)
	void BMP180_GetCalibrationData();
	
	// Polls the BMP180 device for the uncompensated temperature value
	int16_t BMP180_GetUCTemperature();

public:

	// Takes the uncompensated temperature information and then calibrates it using
	// the calibration data we collected at Setup to return a temperature in 
	// 0.1 degrees C
	int16_t BMP180_GetTemperature();
	
	// Initial setup that must be called... talks to the BMP180 sensor to get calibration data
	void Setup();
};

#endif