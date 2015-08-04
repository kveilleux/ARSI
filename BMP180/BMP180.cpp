/* 
* BMP180.cpp
*
* Created: 8/3/2015 6:18:49 PM
* Author: Kyle
*/


#include "BMP180.h"

void BMP180::Setup()
{
	BMP180_GetCalibrationData();
}

int BMP180::BMP180_GetTemperature()
{
	// Get the uncompensated temp
	UCTemp = BMP180_GetUCTemperature();

	// The following is the Bosch Sensortec calibration algorithm for the BMP 180
	long AC6 = (long)calibData[5];
	long AC5 = (long)calibData[4];
	long MC = (long)calibData[9];
	long MD = (long)calibData[10];

	long X1 = ((UCTemp-AC6) * AC5) >> 15;
	long X2 = (MC << 11) / (X1 + MD);
	long B5 = X1 + X2;
	long T = ((B5 + 8)>>4);
	return T;
}

int BMP180::BMP180_GetUCTemperature()
{
	int retVal = 0;
	uint8_t MSB = 0;
	uint8_t LSB = 0;

	// We need to write 0x2E into the 0xF4 register to tell the BMP180 to begin
	// sensing temperature. We do that here. First write is the register to write
	// to, second write is the value
	i2c_start_wait(DevBMPWrite);
	i2c_write(0xF4);
	i2c_write(0x2E);
	i2c_stop();

	// A 4.5 ms delay is required from mfg to accurately sample temperature
	_delay_ms(4.5);
	
	// Tell the BMP180 we want it to start giving us data from the 0xF6 register
	i2c_start_wait(DevBMPWrite);
	i2c_write(0xF6);

	// Tell the BMP180 we are going to start reading now
	i2c_rep_start(DevBMPRead);
	
	// First Ack() gives us the most significant 8 bits, and Ack()
	// requests more data from the device
	MSB = i2c_readAck();

	// Get the least significant 8 bits and Nak() tells the device we are done
	LSB = i2c_readNak();

	// Stops transfer and releases the I2C bus for other devices 
	i2c_stop();
	
	// Store MSB and LSB into our short and return it
	retVal |= (MSB << 8);
	retVal |= (LSB);

	return retVal;
}

void BMP180::BMP180_GetCalibrationData()
{
	// Tell BMP180 we want it to move its register pointer to the 0xAA register, the first calibration data
	i2c_start_wait(DevBMPWrite);
	i2c_write(0xAA);
	i2c_rep_start(DevBMPRead);
	
	// Go through all 22 registers. Every time we read, the BMP180 moves to the next register
	for (int i = 0; i < 11; i++)
	{
		uint8_t MSB = i2c_readAck();
		uint8_t LSB;
		if (i == 10)
		LSB = i2c_readNak();
		else
		LSB = i2c_readAck();
		
		calibData[i] = 0;
		calibData[i] |= (MSB<<8);
		calibData[i] |= (LSB);
	}

	i2c_stop();
}

