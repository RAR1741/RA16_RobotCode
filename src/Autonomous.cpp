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
	autoTime = new Timer();
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
		if(State("start"))
		{
			autoTime->Reset();
			autoTime->Start();
			drive->HaloDrive(0, -0.6);
			scoring->SetPredefinedAngle(1);
			scoring->EnablePID(true);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			drive->HaloDrive(0, -0.6);
			if(autoTime->Get()>= 3)
			{
				autonomousState = "done";
				scoring->EnablePID(false);
				//drive->FL->SetPosition(0);
			}
		}
		else if(State("done"))
		{
			drive->TankDrive(0,0);
		}
//		else if(autonomousState == "turn")
//		{
//			drive->TankDrive(0.6, -0.6);
//		}
		break;
	case 2:
		if(State("start"))
		{
			autoTime->Reset();
			autoTime->Start();
			drive->HaloDrive(0, -0.85);
			scoring->SetPredefinedAngle(1);
			scoring->EnablePID(true);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			drive->HaloDrive(0, -0.85);
			if(autoTime->Get()>= 2)
			{
				autonomousState = "done";
				scoring->EnablePID(false);
				//drive->FL->SetPosition(0);
			}
		}
		else if(State("done"))
		{
			drive->TankDrive(0,0);
		}
//		else if(autonomousState == "turn")
//		{
//			drive->TankDrive(0.6, -0.6);
//		}
		break;
	case 3:
		if(State("start"))
		{
			manipulation->Home();
			autonomousState = "homing";
		}
		if(State("homing"))
		{
			manipulation->Process();
			if(manipulation->GetState() == 3)
			{
				autonomousState = "drive";
			}
		}
		if(State("drive"))
		{
			autoTime->Reset();
			autoTime->Start();
			drive->HaloDrive(0, -0.45);
			scoring->SetPredefinedAngle(2);
			scoring->EnablePID(true);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			drive->HaloDrive(0, -0.45);
			if(autoTime->Get()>= 4)
			{
				autonomousState = "done";
				scoring->EnablePID(false);
				//drive->FL->SetPosition(0);
			}
		}
		else if(State("done"))
		{
			drive->TankDrive(0,0);
		}
//		else if(autonomousState == "turn")
//		{
//			drive->TankDrive(0.6, -0.6);
//		}
		break;
	case 4:
		if(State("start"))
		{
			autoTime->Reset();
			autoTime->Start();
			drive->HaloDrive(0, -0.35);
			scoring->SetPredefinedAngle(2);
			scoring->EnablePID(true);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			manipulation->Set(1);
			drive->HaloDrive(0, -0.35);
			if(autoTime->Get()>= 1.8)
			{
				autonomousState = "done";
				scoring->EnablePID(false);
				//drive->FL->SetPosition(0);
			}
		}
		else if(State("done"))
		{
			drive->TankDrive(0,0);
		}
//		else if(autonomousState == "turn")
//		{
//			drive->TankDrive(0.6, -0.6);
//		}
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

void Autonomous::Log(Logger * log)
{
	log->Log("autonum", autonum);
	log->Log("AutoState", autonomousState);
}

void Autonomous::SetupLogging(Logger *Log)
{
	Log->AddAttribute("autonum");
	Log->AddAttribute("AutoState");
}

bool Autonomous::State(string state)
{
	string lAuto = autonomousState;
	transform(lAuto.begin(), lAuto.end(), lAuto.begin(), ::tolower);
	transform(state.begin(), state.end(), state.begin(), ::tolower);
	if(autonomousState == state)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Autonomous::ResetTime()
{
	autoTime->Reset();
	autoTime->Start();
}


