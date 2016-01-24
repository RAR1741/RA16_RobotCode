/*
 * Scoring.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Scoring.h"

Scoring::Scoring(CANTalon *aMotor, CANTalon *tMotor, CANTalon *lMotor, CANTalon *rMotor)
{
	AngleMotor = aMotor;
	TensionMotor = tMotor;
	LFlyMotor = lMotor;
	RFlyMotor = rMotor;
	state = State::kWaiting;
}
