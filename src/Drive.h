/*
 * Drive.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Taylor Horne
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_
#include "WPILib.h"

class Drive
{
public:
	Drive(CANTalon * r1, CANTalon * r2, CANTalon * l1, CANTalon * l2);
	~Drive();
	void HaloDrive(float x, float y);
	void TankDrive(float l, float r);
	CANTalon * dR1;
	CANTalon * dR2;
	CANTalon * dL1;
	CANTalon * dL2;
private:

};

#endif /* SRC_DRIVE_H_ */
