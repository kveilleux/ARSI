/*
 * BNO055.h
 *
 * Created: 8/12/2015 7:23:55 PM
 *  Author: Kyle
 */ 


#ifndef BNO055_H_
#define BNO055_H_

#include "../i2cmaster.h"
#include <util/delay.h>

#define DevBNORead 0x51
#define DevBNOWrite 0x50
class BNO055
{
public:
	
	void Setup();	
	
	
	void ReadEulerOrientation(volatile int32_t& heading, volatile int32_t& roll, volatile int32_t& pitch);
};



#endif /* BNO055_H_ */