/* Manipulation.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: Taylor Horne
 */
#include "Manipulation.h"
#include "Config.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

Manipulation::Manipulation(CANTalon *bMotor, CANTalon *aMotor, DigitalInput *bLimit, DigitalInput *aLimit)
{
	BaseMotor = bMotor;
	//BaseMotor->ConfigEncoderCodesPerRev(28);
	ArmMotor = aMotor;
	//ArmMotor->ConfigEncoderCodesPerRev(28);
	BaseLimit = bLimit;
	ArmLimit = aLimit;
	state = kReady;
	homingTimer = new Timer();
	ReadPostions();
	BaseMotor->SetPID(5,0,0);
	BaseMotor->Set(0);
	BaseMotor->SetInverted(false);
	BaseMotor->SetControlMode(CANTalon::kPosition);
	BaseMotor->SetEncPosition(0);
	BaseMotor->Set(0);

	ArmMotor->SetPID(5,0,0);
	ArmMotor->Set(0);
	ArmMotor->SetInverted(false);
	ArmMotor->SetControlMode(CANTalon::kPosition);
	ArmMotor->SetEncPosition(0);
	ArmMotor->Set(0);
	movementRunning = false;
	// Wait for reload config to fix the PID constants.
}

int Manipulation::BaseEncoderByDegrees(float degrees) {
	return (int) degrees * kEncoderTicksPerDegree;
}
float Manipulation::BaseDegreesByEncoder(int encoderTicks) {
	return encoderTicks / kEncoderTicksPerDegree;
}

float Manipulation::ArmEncoderByDegrees(float degrees) {
	return (int) degrees * kEncoderTicksPerDegree;
}
int Manipulation::ArmDegreesByEncoder(int encoderTicks) {
	return encoderTicks / kEncoderTicksPerDegree;
}

void Manipulation::SetupLogging(Logger * logger)
{
	logger->AddAttribute("ManipBaseAngle");
	logger->AddAttribute("ManipArmAngle");
	logger->AddAttribute("ManipState");
	logger->AddAttribute("ManipBaseHome");
	logger->AddAttribute("ManipArmHome");
	logger->AddAttribute("ManipBaseVoltage");
	logger->AddAttribute("ManipArmVoltge");
	logger->AddAttribute("ManipBaseSetpoint");
	logger->AddAttribute("ManipArmSetpoint");
	logger->AddAttribute("ManipHandX");
	logger->AddAttribute("ManipHandY");
}

void Manipulation::Log(Logger * logger)
{
	logger->Log("ManipBaseAngle", BaseAngle());
	logger->Log("ManipArmAngle",  ArmAngle());
	logger->Log("ManipBaseHome", IsBaseHome());
	logger->Log("ManipArmHome", IsArmHome());

	std::string stateName = "unknown";

	switch(GetState()) {
	case Manipulation::State::kStart:
		stateName = "kStart";
		break;
	case Manipulation::State::kReady:
		stateName = "kReady";
		break;
	case Manipulation::State::kHomed:
		stateName = "kHomed";
		break;
	case Manipulation::State::kHomingDown:
		stateName = "kHomingDown";
		break;
	default:
		stateName = "unknown (" + std::to_string(GetState()) + ")";
	}

	logger->Log("ManipState", stateName);
	logger->Log("ManipBaseVoltage", BaseMotor->GetOutputVoltage());
	logger->Log("ManipArmVoltage", ArmMotor->GetOutputVoltage());
	logger->Log("ManipBaseEncoder", BaseMotor->GetEncPosition());
	logger->Log("ManipArmEncoder", ArmMotor->GetEncPosition());
	logger->Log("ManipBaseSetpoint", BaseMotor->GetSetpoint());
	logger->Log("ManipArmSetpoint", ArmMotor->GetSetpoint());
	logger->Log("ManipHandX", X());
	logger->Log("ManipHandY", Y());
}

float Manipulation::DegreesToRadians(float degrees)
{
	return degrees * ::acos(-1) / 180.0;
}

float Manipulation::RadiansToDegrees(float radians)
{
	return radians * 180.0 / ::acos(-1);
}

float Manipulation::BaseOK()
{
	return BaseLength() * ::sin(DegreesToRadians(-BaseAngle())) + ArmLength() * ::sin(DegreesToRadians(ArmAngle() - -BaseAngle()));
}

float Manipulation::ArmOK()
{
	return -ArmLength() * ::sin(DegreesToRadians(ArmAngle() - -BaseAngle()));
}

float Manipulation::X()
{
	float theta = -BaseAngle();
	float phi   = ArmAngle();

	return X0() - BaseLength() * ::cos(DegreesToRadians(theta)) +
			    ArmLength()  * ::cos(DegreesToRadians(phi - theta));
}

float Manipulation::Y()
{
	float theta = BaseAngle();
	float phi   = ArmAngle();

	return Y0() + BaseLength() * ::sin(DegreesToRadians(theta)) +
			     ArmLength()  * ::sin(phi - theta);

}

void Manipulation::GoToXY(float x, float y)
{
	// todo: calculate

	float baseDegrees = 0;
	float armDegrees = 0;

	GoToAngles(baseDegrees, armDegrees);
}

void Manipulation::GoToAngles(float baseDegrees, float armDegrees)
{
	isManual = true;
	BaseMotor->Set(BaseEncoderByDegrees(baseDegrees));
	ArmMotor->Set(ArmEncoderByDegrees(armDegrees));
}

void Manipulation::ReadPostions()
{
	ifstream file("/home/lvuser/train.csv");
	int row = 0;
	if(file.good())
	{
		while(!file.eof())
		{
			std::string line;
			std::getline(file, line);
			if ( !file.good() )
				break;
			cout << "did a thing" << endl;
			std::stringstream iss(line);
			vector<float> temp;
			for (int col = 0; col < 2; col++)
			{
				std::string val;
				std::getline(iss, val, ',');
				if ( !iss.good() )
					break;
				temp.push_back(stof(val));
			}
			positions.push_back(temp);
			cout << temp.size() << endl;
			row++;
		}
		file.close();
	}
}

void Manipulation::Train()
{
	fstream train;
	train.open("/home/lvuser/train.csv", std::ios_base::app);
	string out = std::to_string(BaseMotor->GetEncPosition()) + "," + std::to_string(ArmMotor->GetEncPosition()) + ",\n";
	train << out;
	train.close();
	ReadPostions();
}

void Manipulation::Set(int in)
{
	if(this->GetState() == 4)
	{
		isManual = false;
		vector<float>  pos;
		if((unsigned)in < positions.size() && in >= 0)
		{
			pos = positions.at(in);
			cout << "pos: " << pos.at(0) << pos.at(1) << endl;
			BaseMotor->Set(pos.at(0));
			ArmMotor->Set(pos.at(1));
		}
	}
}

void Manipulation::ResetTrain()
{
	std::ofstream ofs;
	ofs.open("/home/lvuser/train.csv", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

bool Manipulation::EditTrain(int index, float val[2])
{
	if((unsigned)index < positions.size() && (unsigned)index >= 0)
	{
		vector<float> tmp;
		tmp.push_back(val[0]);
		tmp.push_back(val[1]);
		positions.at(index) = tmp;
		fstream train;
		train.open("/home/lvuser/train.csv", std::ios_base::out);
		for (unsigned int i = 0; i < positions.size(); i++)
		{
			string out;
			cout << "eowo" << endl;
			out = to_string(positions.at(i).at(0)) + "," + to_string(positions.at(i).at(1)) + ",\n";
			train << out;
		}
		train.close();
		ReadPostions();
		return true;
	}
	else
	{
		return false;
	}
}

void Manipulation::Update()
{
	if(BaseLimit->Get() == false)//object in is false
	{
		BaseMotor->Set(BaseMotor->GetEncPosition());
	}

	if(ArmLimit->Get() == false)
	{
		ArmMotor->Set(ArmMotor->GetEncPosition());
	}
}

void Manipulation::CreateMotion(int pos[], int size)
{
	vector<int> temp;
	for (int i = 0; i<size; i++)
	{
		temp.push_back(pos[i]);
	}
	movements.push_back(temp);
}

void Manipulation::StartMotion(int index)
{
	place = 0;
	currentMov = index;
	Set(movements.at(currentMov).at(place));
	movementRunning = true;
}

void Manipulation::ContinueMotion()
{
	if(movementRunning)
	{
		place++;
		if(movements.at(currentMov).size() >= (unsigned)place)
		{
			Set(movements.at(currentMov).at(place));
		}
		else
		{
			movementRunning = false;
		}
	}
}

void Manipulation::Home()
{
	//BaseMotor->Set((BaseAbs.GetVoltage() + absZeroPos) * absToInc);
	if(state == kReady)
	{
		cout << "Start\n";
		state = Manipulation::kStart;
	}
}

bool Manipulation::IsArmHome() {
	return !(ArmLimit->Get());
}

bool Manipulation::IsBaseHome() {
	return !(BaseLimit->Get());
}

void Manipulation::Process()
{
	//state = kReady;
	switch(state)
	{
	case Manipulation::kStart:
		if(!IsArmHome())
		{
			ArmMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
			ArmMotor->Set(Config::GetSetting("manip_arm_home_speed", -0.5));
			if(!IsBaseHome())
			{
				BaseMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
				BaseMotor->Set(Config::GetSetting("manip_base_home_speed", 0.5));
				state = kHomingDown;
			}
			state = kHomingDown;
		}
		else if(!IsBaseHome())
		{
			BaseMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
			BaseMotor->Set(Config::GetSetting("manip_base_home_speed", 0.5));
			if(!IsArmHome())
			{
				ArmMotor->SetControlMode(CANTalon::ControlMode::kPercentVbus);
				ArmMotor->Set(Config::GetSetting("manip_arm_home_speed", -0.5));
				state = kHomingDown;
			}
			state = kHomingDown;
		}
		else
		{
			state = kHomed;
		}



		//state = Manipulation::kHomingDown;
		break;
	case Manipulation::kHomingDown:
		if(IsArmHome())
		{
			ArmMotor->Set(0);
			ArmMotor->SetPosition(0);
		}
		if(IsBaseHome())
		{
			BaseMotor->Set(0);
			BaseMotor->SetPosition(0);
		}

		if(IsArmHome() && IsBaseHome())
		{
			state = kHomed;
		}
		break;
	case Manipulation::kHomed:
		ArmMotor->SetControlMode(CANTalon::ControlMode::kPosition);
		BaseMotor->SetControlMode(CANTalon::ControlMode::kPosition);
		BaseMotor->Set(0);
		ArmMotor->Set(0);
		state = Manipulation::kReady;
		break;
	case Manipulation::kReady:
		float limit;
		if(isManual)
		{
			limit = 12.5;
		}
		else
		{
			limit = 14;
		}
		cout << "x: " << X() << "\n";
		cout << "limit: " << limit << "\n";
		cout << BaseAngle() << " , " << ArmAngle() << "\n";
//		if(!ArmLimit->Get() && ArmMotor->GetEncVel() <= 0)
//		{
//			ArmMotor->Set(ArmMotor->GetEncPosition());
//		}
		if(X() >= limit) //soft limit + distance to bumpers
		{
			if(BaseOK() > 0 && BaseMotor->GetSetpoint() - BaseMotor->GetEncPosition() <= 0)
			{
				BaseMotor->Set(BaseMotor->GetEncPosition());
			}
			else if(BaseOK() < 0 && BaseMotor->GetSetpoint() - BaseMotor->GetEncPosition() >= 0)
			{
				BaseMotor->Set(BaseMotor->GetEncPosition());
			}

			if(ArmOK() > 0 && ArmMotor->GetSetpoint() - ArmMotor->GetEncPosition() >= 0)
			{
				ArmMotor->Set(ArmMotor->GetEncPosition());
			}
			else if(ArmOK() < 0 && ArmMotor->GetSetpoint() - ArmMotor->GetEncPosition() <= 0)
			{
				ArmMotor->Set(ArmMotor->GetEncPosition());
			}
		}
		break;
	}
}

void Manipulation::ManualDrive(float base, float arm)
{
	BaseMotor->Set(base);
	ArmMotor->Set(arm);
}


Manipulation::State Manipulation::GetState()
{
	return state;
}

void Manipulation::ReloadConfig()
{
	ReadPostions(); // XD
	BaseMotor->SetPID(
		Config::GetSetting("manip_base_p", 5.0),
		Config::GetSetting("manip_base_i", 0.001),
		Config::GetSetting("manip_base_d", 0));

	ArmMotor->SetPID(
		Config::GetSetting("manip_arm_p", 5.0),
		Config::GetSetting("manip_arm_i", 0.001),
		Config::GetSetting("manip_arm_d", 0));

	_base_length = Config::GetSetting("manip_base_length", 15);
	_arm_length =  Config::GetSetting("manip_arm_length", 15);

	_x0 = Config::GetSetting("manip_x0", 6.5);
	_y0 = Config::GetSetting("manip_y0", 11.5);
}

bool Manipulation::isNear()
{
	if(abs(BaseMotor->GetEncPosition() - BaseMotor->Get()) < 30 && abs(ArmMotor->GetEncPosition() - ArmMotor->Get()) < 30)
	{
		return true;
	}
	else
	{
		return false;
	}
}
