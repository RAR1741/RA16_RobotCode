/*
 * Scoring.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Scoring.h"

Scoring::Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor, DigitalInput *homeSensor)
{
	AngleMotor = aMotor;
	TensionMotor = tMotor;
	LFlyMotor = lMotor;
	RFlyMotor = rMotor;
	AngleMotor->SetControlMode(CANTalon::kPosition);
	TensionMotor->SetControlMode(CANTalon::kPosition);
	HomeSensor = homeSensor;
	state = State::kWaiting;
}

void Scoring::Update()
{
	switch (state)
	{
		case Scoring::State::kWaiting:

			break;
		case Scoring::State::kLoading:
			TensionMotor->Set(180);//Near the Index
			TensionMotor->EnableZeroSensorPositionOnIndex(true,HomeSensor->Get());
			if(TensionMotor->GetEncPosition()==0)
			{
				state = kIndexing;
			}
			break;
		case Scoring::State::kIndexing:
			TensionMotor->Set(10);//Get to Exact Position
			break;

		case Scoring::State::kArmed:
			//JUST DO IT

			break;
		case Scoring::State::kTrigger:
			SetFlySpeed(-1);
			TensionMotor->Set(20);//Just past Trigger point

			break;
		case Scoring::State::kFire:

			break;
		case Scoring::State::kReset:
			if(false)//Some condition that confirms home
			{
				state = kWaiting;
			}
			break;
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
	if(state == kArmed)
	{
		state = kTrigger;
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
