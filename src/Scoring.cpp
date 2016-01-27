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
	state = State::kWaiting;
}

void Scoring::Update()
{
	if(state == kLoading)
	{
		//Do loading things
		//set to kReady if loaded
	}
	else if(state == kShooting)
	{
		SetFlySpeed(-1);
		//Do shooting things
		//set to kWaiting if done
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
