/*
 * Autonomous.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: programmer
 */
#include "Autonomous.h"
#include "Logger.h"
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

//Autonomous::Autonomous(DoubleSolenoid *G, DoubleSolenoid *AB, CANTalon * FBM, DigitalInput *LL, Collection *PM, DriveTrain * D, Lift * L, ContainerPlacer * C, Timer * AutonomousTime, Timer * WT)
//{
//	PlasticMagnet = PM;
//	Lifter = L;
//	Placer = C;
//	Drive = D;
//	AutoTime = AutonomousTime;
//	WaitTime = WT;
//	LeftLimit = LL;
//	FourBarMotor = FBM;
//	ArmBrake = AB;
//	Grabber = G;
//
//
//	AutonomousNumber = 0;
//	autonum = 0;
//	totesPickedUp = 0;
//
//	autoPlaceTime = 0;
//	autoDriveTime = 0;
//	autoDriveSpeed = 0;
//	autoTurnTime = 0;
//	autoTurnSpeed = 0;
//
//	contDriveBackShortTime = 0;
//	contDriveBackShortSpeed = 0;
//	contDriveSpeed = 0;
//	contDriveTime = 0;
//	contDrive2Speed = 0;
//	contDrive2Time = 0;
//	contDrive3Speed = 0;
//	contDrive3Time = 0;
//	contDrive4Time = 0;
//	contDrive4Speed = 0;
//	contDrive5Time = 0;
//	contDrive5Speed = 0;
//	contTurnTime = 0;
//	contTurnSpeed = 0;
//	string autonomousState = "Homing";
//
//
//
//
///*	if (AutoTime->Get() > autoDriveTime) {
//		Drive->Drive(0,0,0,0); // stop
//	} else {
//		Drive->Drive(0, -autoDriveSpeed, 0, 0);
//	}
//*/
//
//}

Autonomous::~Autonomous()
{

}

void Autonomous::RunAuto()
{

}

void Autonomous::ReloadConfig()
{

}

void Autonomous::SetupLogging(Logger *Log)
{
	//todo change all attributes

}

void Autonomous::Log(Logger *Log)
{
	//todo don't use these log variables
}

