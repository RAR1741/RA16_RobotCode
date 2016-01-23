/*
 * Manipulation.cpp
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

Manipulation::Manipulation(CANTalon *bMotor, CANTalon *aMotor, DigitalInput *bLimit, DigitalInput *aLimit)
{
	BaseMotor = bMotor;
	ArmMotor = aMotor;
	BaseLimit = bLimit;
	ArmLimit = aLimit;
	ReadPostions();
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
	if(index < positions.size() && index >= 0)
	{
		fstream train;
		train.open("/home/lvuser/train.csv", std::ios_base::out);
		for (unsigned int i; i < positions.size(); i++)
		{
			string out;
			if(i != index)
			{
				out = to_string(positions.at(i).at(0)) + "," + to_string(positions.at(i).at(1)) + ",\n";
			}
			else
			{
				out = to_string(val[0]) + "," + to_string(val[1]) + ",\n";
			}
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
