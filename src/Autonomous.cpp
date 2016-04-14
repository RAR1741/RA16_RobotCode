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

Autonomous::Autonomous(Drive * d, Manipulation * m, Scoring * s, Logger * l, Timer * t, Targeting * tar, FakePIDOutput * fpido, FakePIDSource * fpids, PIDController * pidc)
{
	FPIDO = fpido;
	FPIDS = fpids;
	PIDC = pidc;
	drive = d;
	manipulation = m;
	scoring = s;
	logger = l;
	logtimer = t;
	targeting = tar;
	autoTime = new Timer();
	autonomousState = "start";
	ReloadConfig();
}

Autonomous::~Autonomous()
{

}

void Autonomous::RunAuto()
{
	scoring->Update();
	switch(autonum)
	{
	case 1:
		if(State("start"))
		{
			autoTime->Reset();
			autoTime->Start();
			drive->HaloDrive(0, -0.6);
			//scoring->SetPredefinedAngle(1);
			//scoring->EnablePID(true);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			drive->HaloDrive(0, -0.6);
			if(autoTime->Get()>= 2.5)
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
			//scoring->SetPredefinedAngle(1);
			//scoring->EnablePID(true);
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
			drive->HaloDrive(0, -0.35);
			scoring->SetPredefinedAngle(2);
			scoring->EnablePID(true);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			drive->HaloDrive(0, -0.35);
			if(autoTime->Get()>= 5)
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
			//manipulation->Set(1);
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
	case 5:
		if(State("start"))
		{
			autoTime->Reset();
			autoTime->Start();
			drive->HaloDrive(0,0);
			scoring->EnablePID(false);
			scoring->SetPredefinedAngle(4);
			autonomousState = "going";
		}
		else if(State("going"))
		{
			scoring->SetPredefinedAngle(4);
			scoring->EnablePID(true);
			if(autoTime->Get() >= 4)
			{
				scoring->Load();
				autonomousState = "firing";
				autoTime->Reset();
				autoTime->Start();
				//scoring->Load();
			}
		}
		else if(State("firing"))
		{
			scoring->SetPredefinedAngle(4);
			scoring->EnablePID(true);
			if(autoTime->Get() >= 2)
			{
				autonomousState = "done";
			}
		}
		else if(State("done"))
		{
			drive->HaloDrive(0,0);
		}
		break;
	case 6:
			if(State("start"))
			{
				drive->ResetEnc();
				autoTime->Reset();
				autoTime->Start();
				drive->HaloDrive(0, -0.4);
				scoring->SetPredefinedAngle(3);
				scoring->EnablePID(true);
				autonomousState = "going";
			}
			else if(State("going"))
			{
				drive->HaloDrive(0, -.4);
				if(drive->GetFREnc() >= 6900)
				{
					drive->ResetEnc();
					autonomousState = "turning";
					scoring->EnablePID(false);
					autoTime->Reset();
					autoTime->Start();
				}
			}
			else if(State("turning"))
			{
				drive->TankDrive(0, 0.5);
				if(drive->GetFREnc() >= 100)
				{
					drive->ResetEnc();
					autonomousState = "turning2";
					scoring->SetPredefinedAngle(1);
					scoring->EnablePID(true);
				}
			}
			else if(State("turning2"))
			{
				drive->TankDrive(0.8, 0.8);
				if(drive->GetFREnc() <= -930)
				{
					drive->ResetEnc();
					autonomousState = "raising";
					scoring->SetPredefinedAngle(1);
					scoring->EnablePID(true);
				}
			}
			else if(State("raising"))
			{
				drive->TankDrive(0, 0);
				if(autoTime->Get()>= 3)
				{
					autonomousState = "tracking";
					scoring->EnablePID(false);
				}
			}
			else if(State("tracking"))
			{
				vector<Target> targets;
				targets = targeting->GetTargets();
				if(targets.size() != 0)
				{
					if (!PIDC->IsEnabled())
					{
						PIDC->Enable();
					}
					Target closest = targets.at(0);
					for (unsigned int i = 0; i < targets.size(); i++)
					{
						if(targets.at(i).Distance() < closest.Distance())
						{
							closest = targets.at(i);
						}
					}
					FPIDS->PIDSet(closest.Pan());
					float output = FPIDO->PIDGet();
					drive->TankDrive(output, 0);
					if(closest.Pan() <= (Config::GetSetting("autoAimOffset", -2) + 0.5) && closest.Pan() >= (Config::GetSetting("autoAimOffset", -2) - 0.5))
					{
						scoring->Load();
					}
					//aimLoop->SetSetpoint(targetDegreeToTicks(closest.Tilt()) / 800 + autoAimOffset);
				}
				else
				{
					if (PIDC->IsEnabled())
					{
						PIDC->Disable();
					}
					drive->HaloDrive(0,0);
				}
			}
			else if(State("done"))
			{
				autonomousState = "done";
				drive->TankDrive(0,0);
			}
			break;
	case 7:
			if(State("start"))
			{
				drive->ResetEnc();
				autoTime->Reset();
				autoTime->Start();
				drive->HaloDrive(0, -0.4);
				scoring->SetPredefinedAngle(2);
				scoring->EnablePID(true);
				autonomousState = "lower";
			}
			else if(State("lower"))
			{
				drive->TankDrive(0, 0);
				if(autoTime->Get()>= 3)
				{
					autoTime->Reset();
					autoTime->Start();
					autonomousState = "going";
					scoring->EnablePID(false);
				}
			}
			else if(State("going"))
			{
				drive->HaloDrive(0, .45);
				if(drive->GetFREnc() <= -6900)
				{
					drive->ResetEnc();
					autonomousState = "turning2";
					scoring->EnablePID(false);
					autoTime->Reset();
					autoTime->Start();
					drive->TankDrive(-1, -1);
				}
			}
			else if(State("turning2"))
			{
				drive->TankDrive(0.8, 0.8);
				if(drive->GetFREnc() <= -1000)
				{
					drive->ResetEnc();
					autonomousState = "raising";
					scoring->SetPredefinedAngle(1);
					scoring->EnablePID(true);
				}
			}
			else if(State("raising"))
			{
				drive->TankDrive(0, 0);
				if(autoTime->Get()>= 3)
				{
					autonomousState = "tracking";
					scoring->EnablePID(false);
				}
			}
			else if(State("tracking"))
			{
				vector<Target> targets;
				targets = targeting->GetTargets();
				if(targets.size() != 0)
				{
					if (!PIDC->IsEnabled())
					{
						PIDC->Enable();
					}
					Target closest = targets.at(0);
					for (unsigned int i = 0; i < targets.size(); i++)
					{
						if(targets.at(i).Distance() < closest.Distance())
						{
							closest = targets.at(i);
						}
					}
					FPIDS->PIDSet(closest.Pan());
					float output = FPIDO->PIDGet();
					drive->TankDrive(output, 0);
					if(closest.Pan() <= (Config::GetSetting("autoAimOffset", -2) + 0.5) && closest.Pan() >= (Config::GetSetting("autoAimOffset", -2) - 0.5))
					{
						scoring->Load();
					}
					//aimLoop->SetSetpoint(targetDegreeToTicks(closest.Tilt()) / 800 + autoAimOffset);
				}
				else
				{
					if (PIDC->IsEnabled())
					{
						PIDC->Disable();
					}
					drive->HaloDrive(0,0);
				}
			}
			else if(State("done"))
			{
				autonomousState = "done";
				drive->TankDrive(0,0);
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


