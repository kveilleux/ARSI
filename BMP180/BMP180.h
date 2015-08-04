/* 
* BMP180.h
*
* Created: 8/3/2015 6:18:49 PM
* Author: Kyle
*/


#ifndef __BMP180_H__
#define __BMP180_H__

#include "../i2cmaster.h"
#include <util/delay.h>

#define DevBMPRead 0xEF
#define DevBMPWrite 0xEE

class BMP180
{
	// Our calibration data read from the BMP180 Sensor
	int16_t AC1;
	int16_t	AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t	MB;
	int16_t MC;
	int16_t MD;
	//int16_t calibData[11];
	short	oversampling_setting;

	float oss_delay[4];

	// Uncompensated temperature value from the BMP180 Sensor
	int UCTemp;
	int UCPressure;

	long B5;

	// Retrieves calibration data from the sensor (should be hard coded into the device)
	void BMP180_ReadCalibrationData();
	
	// Polls the BMP180 device for the uncompensated temperature value
	int16_t BMP180_ReadUCTemperature();

	int32_t	BMP180_ReadUCPressure();

public:

	// Takes the uncompensated temperature information and then calibrates it using
	// the calibration data we collected at Setup to return a temperature in 
	// 0.1 degrees C
	int16_t BMP180_GetTemperature();

	int32_t BMP180_GetPressure();
	
	// Initial setup that must be called... talks to the BMP180 sensor to get calibration data
	void Setup();
};

#endif