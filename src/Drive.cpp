/*
 * Drive.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Taylor Horne
 */

#include "Drive.h"
#include "WPILib.h"

Drive::Drive(CANTalon * r1, CANTalon * r2, CANTalon * l1, CANTalon * l2)
{
	dR1 = r1;
	dR2 = r2;
	dL1 = l1;
	dL2 = l2;
}

void Drive::HaloDrive(float x, float y)
{
	float left = y + x;
	float right = y - x;
	dR1->Set(right);
	dR2->Set(right);
	dL1->Set(left);
	dL2->Set(left);
}
