/*
 * Drive.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Taylor Horne
 */

#include "Drive.h"
#include "WPILib.h"

Drive::Drive(CANTalon * fl, CANTalon * fr, CANTalon * br, CANTalon * bl)
{
	dFL = fl;
	dFR = fr;
	dBR = br;
	dBL = bl;
}

void Drive::HaloDrive(float x, float y)
{
	float left = y + x;
	float right = y - x;
	dFL->Set(left);
	dBL->Set(left);
	dFR->Set(right);
	dBR->Set(right);
}
