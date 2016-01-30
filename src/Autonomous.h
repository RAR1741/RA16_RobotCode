/*
 * Autonomous.h
 *
 *  Created on: Feb 28, 2015
 *      Author: programmer
 */

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_
#include "WPILib.h"
//#include "Lift.h"
//#include "DriveTrain.h"
//#include "ContainerPlacer.h"
#include "DigitalInput.h"
//#include "Collection.h"
#include <ctime>
#include <iostream>
#include <fstream>
//#include "Robot.cpp"
//#include "Config.h"

using namespace std;

class Autonomous
{
public:
	//Autonomous(Robot * r);
	~Autonomous();


	void RunAuto();

	void ReloadConfig();

	//void SetupLogging(Logger *Log); //starts logging
	//void Log(Logger *Log); //logs

private:
//	Lift *Lifter;
//	ContainerPlacer *Placer;
//	DriveTrain *Drive;
	Timer *AutoTime;
	Timer *WaitTime;
//	Collection *PlasticMagnet;
	DigitalInput *LeftLimit;
	CANTalon * FourBarMotor;
	DoubleSolenoid * ArmBrake;
	DoubleSolenoid *Grabber;

	int AutonomousNumber;
	int autonum;
	int totesPickedUp;

	float autoPlaceTime;
	float autoDriveTime;
	float autoDriveSpeed;
	float autoTurnTime;
	float autoTurnSpeed;

	float contDriveBackShortTime;
	float contDriveBackShortSpeed;
	float contDriveSpeed;
	float contDriveTime;
	float contDrive2Speed;
	float contDrive2Time;
	float contDrive3Speed;
	float contDrive3Time;
	float contDrive4Time;
	float contDrive4Speed;
	float contDrive5Time;
	float contDrive5Speed;
	float contTurnTime;
	float contTurnSpeed;
	string autonomousState;

	//float autoDriveTime; // = Config::GetSetting("auto_drive_time", 2);
	//float autoDriveSpeed; //  = Config::GetSetting("auto_drive_speed", 0.29);


};



#endif /* SRC_AUTONOMOUS_H_ */
