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
	Drive(CANTalon * fl, CANTalon * fr, CANTalon * br, CANTalon * bl);
	~Drive();
	void HaloDrive(float x, float y);
private:
	CANTalon * dFL;
	CANTalon * dFR;
	CANTalon * dBL;
	CANTalon * dBR;
};

#endif /* SRC_DRIVE_H_ */
