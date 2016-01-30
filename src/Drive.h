/*
 * Drive.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Taylor Horne
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_
#include "WPILib.h"
#include "Logger.h"

class Drive
{
public:
	Drive(CANTalon * r1, CANTalon * r2, CANTalon * l1, CANTalon * l2);
	~Drive();
	void HaloDrive(float x, float y);
	void TankDrive(float l, float r);
	void Log(Logger * logger);

private:
	CANTalon * FL;
	CANTalon * BL;
	CANTalon * FR;
	CANTalon * BR;
};

#endif /* SRC_DRIVE_H_ */
