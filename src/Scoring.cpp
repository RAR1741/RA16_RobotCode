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

Scoring::Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor, DigitalInput *indexSensor,DigitalInput *homeSensor,DigitalInput *forwardEnd)
{
	AngleMotor = aMotor;
	TensionMotor = tMotor;
	LFlyMotor = lMotor;
	RFlyMotor = rMotor;
	AngleMotor->SetControlMode(CANTalon::kPosition);
	TensionMotor->SetControlMode(CANTalon::kPosition);
	IndexSensor = indexSensor;
	HomeAngle = homeSensor;
	homingTimer = new Timer();
	ForwardEnd = forwardEnd;
	state = State::kWaiting;
	homeState = HomeState::kReady;
	fireTimer = new Timer();
	Config::LoadFromFile("/home/lvuser/config.txt");
	ChooChooLoaded = Config::GetSetting("ChooChooLoaded", 400);
	SP = Config::GetSetting("S_S_P", 1);
	SI = Config::GetSetting("S_S_I", 1);
	SD = Config::GetSetting("S_S_D", 1);
	PP = Config::GetSetting("S_P_P", 1);
	PI = Config::GetSetting("S_P_I", 1);
	PD = Config::GetSetting("S_P_D", 1);
	encPos1 = Config::GetSetting("AnglePos1", 1);
	encPos2 = Config::GetSetting("AnglePos2", 1);
	encPos3 = Config::GetSetting("AnglePos3", 1);
	encPos4 = Config::GetSetting("AnglePos4", 1);
	encHomePos = Config::GetSetting("AngleHomedPos", 1);
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

			if(TensionMotor->GetEncPosition() > -50000)
			{
				cout << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.55);
			}
			else if(TensionMotor->GetEncPosition() > -75000)
			{
				cout << "2: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.45);
			}
			else if(TensionMotor->GetEncPosition() > -100000)
			{
				cout << "3: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.4);
			}
			else if(TensionMotor->GetEncPosition() < -135000)
			{
				cout << "4: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.3);
			}
			//TensionMotor->Set(.35);//Get to Exact Position
			if(TensionMotor->GetEncPosition() < -135000)
			{
				cout << "switched" << endl;
				TensionMotor->Set(0);
				state = kArmed;
			}
			break;

		case Scoring::State::kArmed:
			TensionMotor->Set(0);
			//JUST DO IT


			break;
		case Scoring::State::kTrigger:
			//TensionMotor->SetControlMode(CANTalon::kPosition);
			//SetFlySpeed(-1);
			if(TensionMotor->GetEncPosition() < -175000)
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
		AngleMotor->Set(encPos1);
		break;
	case 2:
		AngleMotor->Set(encPos2);
		break;
	case 3:
		AngleMotor->Set(encPos3);
		break;
	case 4:
		AngleMotor->Set(encPos4);
		break;
	default:
		break;
	}
}

float Scoring::GetAngle()
{
	return AngleMotor->GetEncPosition();
}

Scoring::State Scoring::GetState()
{
	return state;
}
