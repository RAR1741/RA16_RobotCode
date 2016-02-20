/* Manipulation.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: Taylor Horne
 */
#include "Manipulation.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

Manipulation::Manipulation(CANTalon *bMotor, CANTalon *aMotor, DigitalInput *bLimit, DigitalInput *aLimit, AnalogInput * bAbs)
{
	BaseMotor = bMotor;
	ArmMotor = aMotor;
	BaseLimit = bLimit;
	ArmLimit = aLimit;
	state = kReady;
	homingTimer = new Timer();
	ReadPostions();
	BaseAbs = bAbs;
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
	vector<float> pos;
	if((unsigned)in < positions.size() && in >= 0)
	{
		pos = positions.at(in);
		BaseMotor->Set(pos.at(0));
		ArmMotor->Set(pos.at(1));
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
}

void Manipulation::ContinueMotion()
{
	place++;
	if(movements.at(place).size() >= (unsigned)place)
	{
		Set(movements.at(currentMov).at(place));
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

void Manipulation::Process()
{
	switch(state)
	{
	case Manipulation::kStart:
		cout << "Starting\n";
		BaseMotor->SetControlMode(CANTalon::kSpeed);
		cout << "Starting2\n";
		homingTimer->Reset();
		homingTimer->Start();
		cout << "Starting3\n";
		if(!BaseLimit->Get())//is triggered
		{
			cout << "Starting4\n";
			state = kHomingUp;
		}
		else if(BaseLimit->Get())//isnt triggered
		{
			cout << "Starting5\n";
			state = kHomingDown;
		}
		cout << "EndStart\n";
		break;
	case Manipulation::kHomingUp:
		cout << "StartHomingUP\n";
		if(homingTimer->Get()<.75)
		{
			BaseMotor->Set(150);
		}
		else
		{
			BaseMotor->Set(0);
			state = kHomingDown;
		}
		cout << "EndHomingUP\n";
		break;
	case Manipulation::kHomingDown:
		cout << "StartHomingDown\n";
		if(BaseLimit->Get())
		{
			BaseMotor->Set(-100);
		}
		else
		{
			BaseMotor->Set(0);
			BaseMotor->SetControlMode(CANTalon::kPosition);
			BaseMotor->SetEncPosition(0);
			BaseMotor->Set(0);
			state = kHomed;
		}
		cout << "StartHomingDown\n";
		break;
	case Manipulation::kHomed:	{
		BaseMotor->Set(0);
		state = kReady;
		break;
	}
	case Manipulation::kReady:
		//cout << "Ready\n";
		break;

	}
}

Manipulation::State Manipulation::GetState()
{
	return state;
}

void Manipulation::ReloadConfig()
{
//	absZeroPos = Config::GetSetting("manAbsZeroPos", 2);
//	absToInc = Config::GetSetting("manAbstoInc", 800);
}
