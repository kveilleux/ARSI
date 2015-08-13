# ARSI
Autonomous Room Space Interrogator

ARSI is a 4-rotor multicopter (Quadcopter) developed off an F450 frame. ARSI will utilize a number of sensors, from an IMU, to barometer and LIDAR, in order to learn and map its position in 3d space around an interior structure. This data will be mapped utilizing a number of range points obtained from the LIDAR sensor which will be transmitted to a PC for processing. When processed, this will provide a 3D interior of the room space with which the computer operator can then use to issue movement commands to ARSI in real time.

This project is the code for the ARSI flight computer, which is being developed in C++ on an Atmel ATmega328p AVR 16Mhz.

Temperature corrected altitude information is provided by a Bosch Sensortec BMP180.
Accelerometer, Gyroscope and Magnetometer information provided by a Bosch Sensotec BNO055.
GPS data will be sourced through a low-power GPS sensor, however at this time one has not been chosen.

The ARSI flight computer will then drive four electronic speed controllers (ESC) which will spin attached brushless motors and in turn, spin the propeller to provide lift. The flight computer will take all sensor information to provide automatic levelling and stabilization during flights.

If the ATmega328p is not sufficient with its limited program space and clock speed, low cost 32-bit, 50-60 Mhz options are available.
