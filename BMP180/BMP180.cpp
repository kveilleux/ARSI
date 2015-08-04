/* 
* BMP180.cpp
*
* Created: 8/3/2015 6:18:49 PM
* Author: Kyle
*/


#include "BMP180.h"
#include <math.h>

void BMP180::Setup()
{
	oversampling_setting = 0;

	// We need a longer delay based on the OSS chosen above for more readings
	oss_delay[0] = 4.5f;
	oss_delay[1] = 7.5f;
	oss_delay[2] = 13.5f;
	oss_delay[3] = 25.5f;

	BMP180_ReadCalibrationData();
}

int32_t BMP180::BMP180_GetPressure()
{
	BMP180_ReadUCTemperature();
	int UCPressure = BMP180_ReadUCPressure();
	long pressure = 0;

	long B6 =	(long)B5 - 4000;

	long X1 = (B2 * ((B6 * B6) >> 12))>> 11;
	long X2 = (AC2 * B6) >> 11;
	long X3 = X1 + X2;
	long B3 = (((AC1 * 4 + X3) << oversampling_setting) + 2) / 4;
	X1 = (AC3 * B6) >> 13;
	X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;

	
	int32_t B4 = AC4 * (unsigned long)(X3 + 32768) >> 15;
	uint32_t B7 = ((unsigned long)UCPressure - B3) * (50000 >> oversampling_setting);
	if (B7 < 0x80000000){
		pressure = (B7 * 2) / B4;
	}
	else {
		pressure = (B7 / B4) *2;
	}

	X1 = (pressure >> 8) * (pressure >> 8);

	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * pressure) >> 16;

	pressure = pressure + ((X1 + X2 + 3791) >> 4);
	
	return pressure;
}

int BMP180::BMP180_GetTemperature()
{
	// Get the uncompensated temp
	UCTemp = BMP180_ReadUCTemperature();
	
	long X1 = (UCTemp - (int32_t)AC6) * (int32_t)AC5 >> 15;
	long X2 = ((int32_t)MC << 11) / (X1 + (int32_t)MD);
	B5 = X1 + X2;
	long T = (B5 + 8) >>4;
	return T;
}

int32_t BMP180::BMP180_ReadUCPressure()
{
	i2c_start_wait(DevBMPWrite);
	i2c_write(0xF4);
	i2c_write(0xF6 + (oversampling_setting << 6));
	i2c_stop();

	// Wait some time for the pressure to read
	switch (oversampling_setting)
	{
		case 0:
			_delay_ms(4.5);
			break;
		case 1:
			_delay_ms(7.5);
			break;
		case 2:
			_delay_ms(13.5);
			break;	
		case 3:
			_delay_ms(25.5);
			break;
	}

	i2c_start_wait(DevBMPWrite);
	i2c_write(0xF6);

	i2c_rep_start(DevBMPRead);
	
	volatile int32_t MSB = i2c_readAck();
	volatile int32_t LSB = i2c_readAck();
	volatile int32_t XLSB = i2c_readNak();

	i2c_stop();

	int32_t retVal = (((MSB << 16) & 0xFF0000) + ((LSB << 8) & 0xFF00) + (XLSB & 0xFF)) >> (8-oversampling_setting);

	return retVal;
}

int BMP180::BMP180_ReadUCTemperature()
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

void BMP180::BMP180_ReadCalibrationData()
{
	// Tell BMP180 we want it to move its register pointer to the 0xAA register, the first calibration data
	i2c_start_wait(DevBMPWrite);
	i2c_write(0xAA);
	i2c_rep_start(DevBMPRead);
	

	// Go through all 22 registers. Every time we read, the BMP180 moves to the next register
	for (int i = 0; i < 11; i++)
	{
		int MSB = i2c_readAck();
		int LSB;
		if (i == 10)
		LSB = i2c_readNak();
		else
		LSB = i2c_readAck();
		
		switch(i)
		{
			case 0:
				AC1 = 0;
				AC1 |= (MSB << 8);
				AC1 |= (LSB);
				break;
			case 1:
				AC2 = 0;
				AC2 |= (MSB << 8);
				AC2 |= (LSB);
				break;
			case 2:
				AC3 = 0;
				AC3 |= (MSB << 8);
				AC3 |= (LSB);
				break;
			case 3:
				AC4 = 0;
				AC4 |= (MSB << 8);
				AC4 |= (LSB);
				break;
			case 4:
				AC5 = 0;
				AC5 |= (MSB << 8);
				AC5 |= (LSB);
				break;
			case 5:
				AC6 = 0;
				AC6 |= (MSB << 8);
				AC6 |= (LSB);
				break;
			case 6:
				B1 = 0;
				B1 |= (MSB << 8);
				B1 |= (LSB);
				break;
			case 7:
				B2 = 0;
				B2 |= (MSB << 8);
				B2 |= (LSB);
				break;
			case 8:
				MB = 0;
				MB |= (MSB << 8);
				MB |= (LSB);
				break;
			case 9:
				MC = 0;
				MC |= (MSB << 8);
				MC |= (LSB);
				break;
			case 10:
				MD = 0;
				MD |= (MSB << 8);
				MD |= (LSB);
				break;
		}
	}

	i2c_stop();
}

