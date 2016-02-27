/*
 * Autonomous.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: programmer
 */

#include "DigitalInput.h"
#include "WPILib.h"
#include "RobotHeader.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

Autonomous::Autonomous(Drive * d, Manipulation * m, Scoring * s, Logger * l, Timer * t)
{
	drive = d;
	manipulation = m;
	scoring = s;
	logger = l;
	logtimer = t;
	ReloadConfig();
}

Autonomous::~Autonomous()
{

}

void Autonomous::RunAuto()
{
	switch(autonum)
	{
	case 1:
		if(autonomousState == "start")
		{
			drive->TankDrive(-0.6, -0.6);
			if(drive->FL->GetEncPosition() < 2000)
			{
				autonomousState = "turn";
				drive->FL->SetPosition(0);
			}
		}
		else if(autonomousState == "turn")
		{
			drive->TankDrive(0.6, -0.6);
		}
		break;
	default:
		cout << "bad auto" << endl;
		break;
	}
}

void Autonomous::ReloadConfig()
{
	autonum = Config::GetSetting("autonum", 0);
}

void Log()
{

}
//void Autonomous::SetupLogging(Logger *Log)
//{
//	//todo change all attributes
//
//}
//

