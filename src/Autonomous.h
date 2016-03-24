/*
 * Autonomous.h
 *
 *  Created on: Feb 28, 2015
 *      Author: programmer
 */

#ifndef SRC_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_H_
#include "WPILib.h"
#include "Drive.h"
#include "Manipulation.h"
#include "Scoring.h"
#include "Logger.h"
#include "Targeting.h"
#include "RobotHeader.h"
#include "FakePIDOutput.h"
#include "FakePIDSource.h"
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

class Autonomous
{
public:
	Autonomous(Drive * d, Manipulation * m, Scoring * s, Logger * l, Timer * t, Targeting * tar, FakePIDOutput * fpido, FakePIDSource * fpids, PIDController * pidc);
	~Autonomous();


	void RunAuto();

	void ReloadConfig();
	void ResetTime();

	void SetupLogging(Logger *Log); //starts logging
	void Log(Logger * log); //logs

private:
	bool State(string state);
	FakePIDOutput * FPIDO;
	FakePIDSource * FPIDS;
	PIDController * PIDC;
	Drive * drive;
	Manipulation * manipulation;
	Scoring * scoring;
	Logger * logger;
	Timer * logtimer;
	Timer * autoTime;
	Targeting * targeting;

	string autonomousState = "start";
	int autonum;
};



#endif /* SRC_AUTONOMOUS_H_ */
