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

bool Manipulation::ReadMotion(int in)
{
	vector <float[2]> data;
	ifstream file("train" + std::to_string(in) + ".csv");
	if(!file.good())
	{
		return false;
	}
	int row = 0;
	while(!file.eof())
	{
	    std::string line;
	    std::getline(file, line);
	    if ( !file.good() )
	        break;

	    std::stringstream iss(line);
	    float temp[2];
	    for (int col = 0; col < 2; ++col)
	    {
	        std::string val;
	        std::getline(iss, val, ',');
	        if ( !iss.good() )
	            break;
	        temp[col] = stof(val);
	    }
	    data.push_back(temp);
	    row++;
	}
	movements.push_back(data);
	return true;
}
