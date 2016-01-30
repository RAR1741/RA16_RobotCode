/*
 * Autonomous.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: programmer
 */
#include "Autonomous.h"
#include "Logger.h"
//#include "Robot.cpp"
//#include "Config.h"
//#include "Lift.h"
//#include "DriveTrain.h"
//#include "ContainerPlacer.h"
#include "DigitalInput.h"
//#include "Collection.h"
#include "WPILib.h"
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
}

Autonomous::~Autonomous()
{

}

void Autonomous::RunAuto()
{

}

void Autonomous::ReloadConfig()
{

}

//void Autonomous::SetupLogging(Logger *Log)
//{
//	//todo change all attributes
//
//}
//
void Autonomous::Log()
{
	logger->Log("Time", logtimer->Get());
	logger->Log("FLVoltage", drive->FL->GetBusVoltage());
	logger->Log("FRVoltage", drive->FR->GetBusVoltage());
	logger->Log("BLVoltage", drive->BL->GetBusVoltage());
	logger->Log("BRVoltage", drive->BR->GetBusVoltage());
	logger->Log("FLCurrent", drive->FL->GetOutputCurrent());
	logger->Log("FRCurrent", drive->FR->GetOutputCurrent());
	logger->Log("BLCurrent", drive->BL->GetOutputCurrent());
	logger->Log("BRCurrent", drive->BR->GetOutputCurrent());
	logger->WriteLine();
}

