/*
 * BNO055.cpp
 *
 * Created: 8/12/2015 7:23:46 PM
 *  Author: Kyle
 */ 

#include "BNO055.h"
#include <math.h>

#define OPR_MODE 0x3D

void BNO055::Setup()
{
	i2c_start_wait(DevBNOWrite);
	i2c_write(OPR_MODE);
	i2c_write(0xC);
	
	i2c_stop();
}


void BNO055::ReadEulerOrientation(volatile int32_t& heading,volatile  int32_t& roll, volatile int32_t& pitch)
{
	heading = roll = pitch = 0;
	
	i2c_start_wait(DevBNOWrite);
	i2c_write(0x1A);
	i2c_rep_start(DevBNORead);
	
	volatile int32_t headLSB = i2c_readAck();
	volatile int32_t headMSB = i2c_readAck();
	
	volatile int32_t rollLSB = i2c_readAck();
	volatile int32_t rollMSB = i2c_readAck();
	
	volatile int32_t pitchLSB = i2c_readAck();
	volatile int32_t pitchMSB = i2c_readNak();
	
	i2c_stop();
	
	
	heading |= (headMSB << 8);
	heading |= headLSB;
	
	roll |= (rollMSB << 8);
	roll |= rollLSB;
	
	pitch |= (pitchMSB << 8);
	pitch |= pitchLSB;
	
	
}