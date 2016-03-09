/*
 * Scoring.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Scoring.h"
#include "Config.h"
#include "outputlog.h"
#include <iostream>

using namespace std;

Scoring::Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor, DigitalInput *indexSensor,AnalogInput *absenc)
{
	AngleMotor = aMotor;
	TensionMotor = tMotor;
	LFlyMotor = lMotor;
	RFlyMotor = rMotor;
	AngleEncoder = absenc;
	AngleMotor->SetControlMode(CANTalon::kPercentVbus);
	AngleMotor->SetInverted(false);
	AngleMotor->SetPID(0,0,0);
	IndexSensor = indexSensor;
	homingTimer = new Timer();
	state = State::kWaiting;
	homeState = HomeState::kReady;
	fireTimer = new Timer();
	Config::LoadFromFile("/home/lvuser/config.txt");
	ChooChooLoaded = Config::GetSetting("ChooChooLoaded", 400);
	SP = Config::GetSetting("S_S_P", 1);
	SI = Config::GetSetting("S_S_I", 1);
	SD = Config::GetSetting("S_S_D", 1);
	PP = Config::GetSetting("S_P_P", 19);
	PI = Config::GetSetting("S_P_I", 2);
	PD = Config::GetSetting("S_P_D", 0);
	offset = Config::GetSetting("S_offset", 200);
	encPos1 = Config::GetSetting("AnglePos1", 2750);
	encPos2 = Config::GetSetting("AnglePos2", 2900);
	encPos3 = Config::GetSetting("AnglePos3", 3250);
	encPos4 = Config::GetSetting("AnglePos4", 3480);
	degPos1 = Config::GetSetting("AngleDeg1", 2750);
	degPos2 = Config::GetSetting("AngleDeg2", 2900);
	degPos3 = Config::GetSetting("AngleDeg3", 3250);
	degPos4 = Config::GetSetting("AngleDeg4", 3480);
	encHomePos = Config::GetSetting("AngleHomedPos", 1);
	holdStart = Config::GetSetting("holdStart", -97000);
	holdInc = Config::GetSetting("holdInc", -25000);
	holdFire = Config::GetSetting("holdFire", -20000);
	isPrototype = !Config::GetSetting("isPrototype", 0) == 0;

	// Set up tension motor
	TensionMotor->SetControlMode(CANTalon::kPercentVbus);
	TensionMotor->EnableZeroSensorPositionOnIndex(true, false);
	TensionMotor->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateQuadEncoder, 6);
	TensionMotor->Enable();


	// Initialize PID Controller
	AimLoop = new PIDController(19, 2, 0, AngleEncoder, AngleMotor ,0.05);
	AimLoop->SetPID(PP,PI,PD);
	AimLoop->SetContinuous(false);
	AimLoop->SetPIDSourceType(PIDSourceType::kDisplacement);
	AimLoop->SetInputRange(0,4.8);
	AimLoop->SetOutputRange(-.9,.9);
	AimLoop->Disable();

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
			if(TensionMotor->GetPinStateQuadIdx() == 1)//CHANGE THIS SOON, IDK WHAT THE DEFAULT IS && fabs( TensionMotor->GetEncPosition() ) <= 1000 0change to a range
			{
				TensionMotor->SetPosition(0);
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
				TensionMotor->Set(-.6);
			}
			else if(TensionMotor->GetEncPosition() > start + inc)
			{
				cout << "2: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.55);
			}
			else if(TensionMotor->GetEncPosition() > start + (2 * inc))
			{
				cout << "3: " << TensionMotor->GetEncPosition() << endl;
				TensionMotor->Set(-.45);
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
			if(TensionMotor->GetEncPosition() < holdStart + (holdInc * 3) + holdFire)
			{
				TensionMotor->Set(0);
				fireTimer->Reset();
				fireTimer->Start();
				state = kFiring;
			}
			else
			{
				TensionMotor->Set(-.75);
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
	OutputLog * jim = new OutputLog();
	jim->Troll(cout);
}

void Scoring::Home()
{
	if(homeState == kReady)
	{
		homeState = kStart;
	}
}

#if 0
void Scoring::AngleHomeLoop()
{
	switch(homeState)
	{
	case Scoring::HomeState::kStart:

		AngleMotor->SetControlMode(CANTalon::kPercentVbus);

		homingTimer->Reset();
		homingTimer->Start();

		if(HomeAngle->Get())//is triggered
		{
			cout << "Starting4\n";
			homeState = kHomed;
		}
		else if(!HomeAngle->Get())//isnt triggered
		{
			homeState = kHomingDown;
		}
		break;
	case Scoring::HomeState::kHomingUp:
		cout << "StartHomingUP\n";
		if(homingTimer->Get()< 0.5)
		{
			AngleMotor->Set(.6);
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
		if(!HomeAngle->Get())//isnt triggered
		{
			AngleMotor->Set(-.6);
		}
		else
		{
			AngleMotor->Set(0);
			AngleMotor->SetControlMode(CANTalon::kPosition);
			//AngleMotor->SetPosition(encHomePos);
			AngleMotor->Set(encHomePos);
			homeState = kHomed;
		}
		cout << "StartHomingDown\n";
		break;
	case Scoring::HomeState::kHomed:
	{
		AngleMotor->SetControlMode(CANTalon::kPosition);
		AngleMotor->Set(encHomePos);
		homeState = kReady;
		break;
	}
	case Scoring::HomeState::kReady:
		break;

	}
}
#endif

void Scoring::SetPreIncrementalPos(int posNum)
{
	switch(posNum)
	{
	case 1:
		AimLoop->SetSetpoint(DegreeToEnc(degPos1));
		break;
	case 2:
		AimLoop->SetSetpoint(DegreeToEnc(degPos2));
		break;
	case 3:
		AimLoop->SetSetpoint(DegreeToEnc(degPos3));
		break;
	case 4:
		AimLoop->SetSetpoint(DegreeToEnc(degPos4));
		break;
	default:
		break;
	}
}

void Scoring::SetPredefinedAngle(int posNum)
{
	switch(posNum)
	{
	case 0:
		AimLoop->SetSetpoint(ValueToVoltage(encPos1 + offset));
		break;
	case 1:
		AimLoop->SetSetpoint(ValueToVoltage(encPos2 + offset));
		break;
	case 2:
		AimLoop->SetSetpoint(ValueToVoltage(encPos3 + offset));
		break;
	case 3:
		AimLoop->SetSetpoint(ValueToVoltage(encPos4 + offset));
		break;
	default:
		break;
	}
}

float Scoring::ValueToVoltage(float value)
{
	return value/800.0F;
}

float Scoring::DegreeToEnc(float value)
{
	return value * 16118.518518F;
}

float Scoring::GetAngle()
{
	return AngleMotor->GetEncPosition();
}

Scoring::State Scoring::GetState()
{
	return state;
}

void Scoring::ReloadConfig()
{
	ChooChooLoaded = Config::GetSetting("ChooChooLoaded", 400);
	SP = Config::GetSetting("S_S_P", 1);
	SI = Config::GetSetting("S_S_I", 1);
	SD = Config::GetSetting("S_S_D", 1);
	PP = Config::GetSetting("S_P_P", 19);
	PI = Config::GetSetting("S_P_I", 2);
	PD = Config::GetSetting("S_P_D", 0);
	AimLoop->SetPID(PP,PI,PD);
	offset = Config::GetSetting("S_offset", 200);
	encPos1 = Config::GetSetting("AnglePos1", 1800);
	encPos2 = Config::GetSetting("AnglePos2", 2000);
	encPos3 = Config::GetSetting("AnglePos3", 2400);
	encPos4 = Config::GetSetting("AnglePos4", 2680);
	degPos1 = Config::GetSetting("AngleDeg1", 2750);
	degPos2 = Config::GetSetting("AngleDeg2", 2900);
	degPos3 = Config::GetSetting("AngleDeg3", 3250);
	degPos4 = Config::GetSetting("AngleDeg4", 3480);
	encHomePos = Config::GetSetting("AngleHomedPos", 1);
	holdStart = Config::GetSetting("holdStart", -97000);
	holdInc = Config::GetSetting("holdInc", -25000);
	holdFire = Config::GetSetting("holdFire", -20000);
	isPrototype = !Config::GetSetting("isPrototype", 0) == 0;
}

void Scoring::SetupLogging(Logger * logger)
{
	logger->AddAttribute("AimPos");
	logger->AddAttribute("AimTarget");
	logger->AddAttribute("PunchPos");
	logger->AddAttribute("AimCurrent");
	logger->AddAttribute("PunchCurrent");
	logger->AddAttribute("puncherState");
	logger->AddAttribute("AbsEnc");
	logger->AddAttribute("PuncherIndex");
}

void Scoring::Log(Logger * logger)
{
	logger->Log("AimPos", AngleMotor->GetEncPosition());
	logger->Log("AimTarget", AimLoop->GetSetpoint());
	logger->Log("PunchPos", TensionMotor->GetEncPosition());
	logger->Log("AimCurrent", AngleMotor->GetOutputCurrent());
	logger->Log("PunchCurrent", TensionMotor->GetOutputCurrent());
	logger->Log("puncherState", state);
	logger->Log("AbsEnc", AngleEncoder->GetVoltage());
	logger->Log("PuncherIndex", TensionMotor->GetPinStateQuadIdx());
}

void Scoring::EnablePID(bool e)
{
	if(e)
	{
		AimLoop->Enable();
	}
	else
	{
		AimLoop->Disable();
	}
}
