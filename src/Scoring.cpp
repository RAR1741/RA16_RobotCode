/*
 * Scoring.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Scoring.h"
#include "Config.h"
#include <iostream>

using namespace std;

Scoring::Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor, DigitalInput *indexSensor,PIDController * aimLoop,DigitalInput *homeSensor,DigitalInput *forwardEnd)
{
	AimLoop = aimLoop;
	AngleMotor = aMotor;
	TensionMotor = tMotor;
	LFlyMotor = lMotor;
	RFlyMotor = rMotor;
	AngleMotor->SetControlMode(CANTalon::kPercentVbus);
	TensionMotor->SetControlMode(CANTalon::kPercentVbus);
	IndexSensor = indexSensor;
	HomeAngle = homeSensor;
	homingTimer = new Timer();
	ForwardEnd = forwardEnd;
	state = State::kWaiting;
	homeState = HomeState::kReady;
	fireTimer = new Timer();
	Config::LoadFromFile("/home/lvuser/config.txt");
	ChooChooLoaded = Config::GetSetting("ChooChooLoaded", 400);
//	SP = Config::GetSetting("S_S_P", 1);
//	SI = Config::GetSetting("S_S_I", 1);
//	SD = Config::GetSetting("S_S_D", 1);
	PP = Config::GetSetting("S_P_P", 19);
	PI = Config::GetSetting("S_P_I", 2);
	PD = Config::GetSetting("S_P_D", 0);
	AimLoop->SetPID(PP,PI,PD);
	encPos1 = Config::GetSetting("AnglePos1", 2750);
	encPos2 = Config::GetSetting("AnglePos2", 2900);
	encPos3 = Config::GetSetting("AnglePos3", 3250);
	encPos4 = Config::GetSetting("AnglePos4", 3480);
	encHomePos = Config::GetSetting("AngleHomedPos", 1);
	holdStart = Config::GetSetting("holdStart", -97000);
	holdInc = Config::GetSetting("holdInc", -25000);
}

void Scoring::Update()
{
	switch (state)
	{
		case Scoring::State::kWaiting:

			break;
		case Scoring::State::kLoading:
			TensionMotor->SetControlMode(CANTalon::kPercentVbus);
			TensionMotor->Set(-.65);//just past the Index
			if(IndexSensor->Get())// && fabs( TensionMotor->GetEncPosition() ) <= 1000 0change to a range
			{
				TensionMotor->SetEncPosition(0);
				cout << TensionMotor->GetEncPosition() << endl;
				//TensionMotor->EnableZeroSensorPositionOnIndex(false, false);
				//TensionMotor->SetControlMode(CANTalon::kPosition);
				state = kIndexing;
			}
			break;
		case Scoring::State::kIndexing:
		{
			int start = holdStart; //-97000;
			int inc = holdInc; //-25000;
			if(TensionMotor->GetEncPosition() > start)
			{
				cout << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.55);
			}
			else if(TensionMotor->GetEncPosition() > start + inc)
			{
				cout << "2: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.45);
			}
			else if(TensionMotor->GetEncPosition() > start + (2 * inc))
			{
				cout << "3: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.4);
			}
			else if(TensionMotor->GetEncPosition() < start + (3 * inc))
			{
				cout << "4: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.3);
			}
			//TensionMotor->Set(.35);//Get to Exact Position
			if(TensionMotor->GetEncPosition() < start + (3 * inc))
			{
				cout << "switched" << endl;
				TensionMotor->Set(0);
				state = kArmed;
			}
			break;
		}
		case Scoring::State::kArmed:
			TensionMotor->Set(0);
			//JUST DO IT


			break;
		case Scoring::State::kTrigger:
			//TensionMotor->SetControlMode(CANTalon::kPosition);
			//SetFlySpeed(-1);
			if(TensionMotor->GetEncPosition() < holdStart + (holdInc * 3) - 15000)
			{
				TensionMotor->Set(0);
				fireTimer->Reset();
				fireTimer->Start();
				state = kFiring;
			}
			else
			{
				TensionMotor->Set(-.3);
			}
			//TensionMotor->Set(200000);//Just past Trigger point

			//state = kFiring;
			break;
		case Scoring::State::kFiring:
			TensionMotor->Set(0);
			if(fireTimer->Get() >= 1.5)//wait 1.5 seconds to go to next state
			{
				state = kReset;
			}
			//Timer for waiting for fire to happen
			break;
		case Scoring::State::kReset:
			TensionMotor->Set(0);
			//SetFlySpeed(0);
			//TensionMotor->EnableZeroSensorPositionOnIndex(true, false);
			if(true)//Some condition that confirms resting pos
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
	//AngleMotor->Set(angle);
}

void Scoring::AngleHome()
{
	switch(homeState)
	{
	case Scoring::HomeState::kStart:

		AngleMotor->SetControlMode(CANTalon::kSpeed);

		homingTimer->Reset();
		homingTimer->Start();

		if(!HomeAngle->Get())//is triggered
		{
			cout << "Starting4\n";
			homeState = kHomingUp;
		}
		else if(HomeAngle->Get())//isnt triggered
		{
			homeState = kHomingDown;
		}
		break;
	case Scoring::HomeState::kHomingUp:
		cout << "StartHomingUP\n";
		if(homingTimer->Get()< 2)
		{
			AngleMotor->Set(1300);
		}
		else
		{
			AngleMotor->Set(0);
			homeState = kHomingDown;
		}
		cout << "EndHomingUP\n";
		break;
	case Scoring::HomeState::kHomingDown:
		cout << "StartHomingDown\n";
		if(HomeAngle->Get())//isnt triggered
		{
			AngleMotor->Set(-1300);
		}
		else
		{
			AngleMotor->Set(0);
			AngleMotor->SetControlMode(CANTalon::kPosition);
			AngleMotor->SetEncPosition(encHomePos);
			AngleMotor->Set(encHomePos);
			homeState = kHomed;
		}
		cout << "StartHomingDown\n";
		break;
	case Scoring::HomeState::kHomed:
	{
		AngleMotor->Set(encHomePos);
		homeState = kReady;
		break;
	}
	case Scoring::HomeState::kReady:

		break;

	}
}

void Scoring::SetPredefinedAngle(int posNum)
{
	switch(posNum)
	{
	case 1:
		AimLoop->SetSetpoint(ValueToVoltage(encPos1));
		break;
	case 2:
		AimLoop->SetSetpoint(ValueToVoltage(encPos2));
		break;
	case 3:
		AimLoop->SetSetpoint(ValueToVoltage(encPos3));
		break;
	case 4:
		AimLoop->SetSetpoint(ValueToVoltage(encPos4));
		break;
	default:
		break;
	}
}

float Scoring::ValueToVoltage(float value)
{
	return value/800.0F;
}

float Scoring::GetAngle()
{
	return AngleMotor->GetEncPosition();
}

Scoring::State Scoring::GetState()
{
	return state;
}
