/*
 * Scoring.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Scoring.h"
#include "Config.h"

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
	fireTimer = new Timer();
	Config::LoadFromFile("/home/lvuser/config.txt");
	ChooChooLoaded = Config::GetSetting("ChooChooLoaded", 400);
	SP = Config::GetSetting("S_S_P", 1);
	SI = Config::GetSetting("S_S_I", 1);
	SD = Config::GetSetting("S_S_D", 1);
	PP = Config::GetSetting("S_P_P", 1);
	PI = Config::GetSetting("S_P_I", 1);
	PD = Config::GetSetting("S_P_D", 1);
}

void Scoring::Update()
{
	switch (state)
	{
		case Scoring::State::kWaiting:

			break;
		case Scoring::State::kLoading:
			//TensionMotor->SetControlMode(CANTalon::kSpeed);Speed Mode?
			TensionMotor->Set(180);//just past the Index
			if(HomeSensor->Get() && fabs( TensionMotor->GetEncPosition() ) <= 20)//change to a range
			{
				state = kIndexing;
			}
			break;
		case Scoring::State::kIndexing:
			TensionMotor->Set(100);//Get to Exact Position
			if(fabs(TensionMotor->GetClosedLoopError())< 20)
			{
				state = kArmed;
			}
			break;

		case Scoring::State::kArmed:
			//JUST DO IT


			break;
		case Scoring::State::kTrigger:
			SetFlySpeed(-1);
			TensionMotor->Set(20);//Just past Trigger point
			state = kFiring;
			break;
		case Scoring::State::kFiring:
			fireTimer->Reset();
			fireTimer->Start();
			if(fireTimer->Get() >= 1.5)//wait 1.5 seconds to go to next state
			{
				state = kReset;
			}
			//Timer for waiting for fire to happen
			break;
		case Scoring::State::kReset:
			SetFlySpeed(0);
			if(false)//Some condition that confirms resting pos
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
	LFlyMotor->Set(speed);
	RFlyMotor->Set(speed);
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
