/*
 * Scoring.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Scoring.h"

Scoring::Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor)
{
	AngleMotor = aMotor;
	TensionMotor = tMotor;
	LFlyMotor = lMotor;
	RFlyMotor = rMotor;
	AngleMotor->SetControlMode(CANTalon::kPosition);
	TensionMotor->SetControlMode(CANTalon::kPosition);
	state = State::kWaiting;
}

void Scoring::Update()
{
	if(state == kLoading)
	{
		TensionMotor->Set(180);
		if(TensionMotor->GetEncPosition() > 179 && TensionMotor->GetEncPosition() < 180)
		{
			state = kReady;
		}
	}
	else if(state == kShooting)
	{
		SetFlySpeed(-1);
		TensionMotor->Set(360);
		if(TensionMotor->GetEncPosition() > 359 && TensionMotor->GetEncPosition() < 360)
		{
			state = kWaiting;
			TensionMotor->Set(0);
			TensionMotor->SetEncPosition(0);
		}
	}
}

void Scoring::Load()
{
	if(state == kWaiting)
	{
		state = kLoading;
	}
}

void Scoring::Fire()
{
	if(state == kReady)
	{
		state = kShooting;
	}
}

void Scoring::SetFlySpeed(float speed)
{
	LFlyMotor->Set(-speed);
	LFlyMotor->Set(speed);
}

void Scoring::SetAngle(float angle)
{
	AngleMotor->Set(angle);
}

float Scoring::GetAngle()
{
	return AngleMotor->GetEncPosition();
}

Scoring::State Scoring::GetState()
{
	return state;
}
