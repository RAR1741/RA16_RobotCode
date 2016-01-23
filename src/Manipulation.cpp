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
}

void Manipulation::ReadPostions()
{
	ifstream file("train.csv");
	int row = 0;
	while(!file.eof())
	{
	    std::string line;
	    std::getline(file, line);
	    if ( !file.good() )
	        break;

	    std::stringstream iss(line);
	    vector<float> temp;
	    for (int col = 0; col < 2; ++col)
	    {
	        std::string val;
	        std::getline(iss, val, ',');
	        if ( !iss.good() )
	            break;
	        temp.push_back(stof(val));
	    }
	    positions.push_back(temp);
	    row++;
	}
}

void Manipulation::Train()
{
	fstream train;
	train.open("train.csv", std::ios_base::app);
	string out = BaseMotor->GetEncPosition() + "," + ArmMotor->GetEncPosition();
	train << out;
	train.close();
}

void Manipulation::Set(int in)
{
	vector<float> pos;
	if((unsigned)in < positions.size() && in >= 0)
	{
		pos = positions.at(in);
		BaseMotor->Set(pos[0]);
		ArmMotor->Set(pos[1]);
	}
}
