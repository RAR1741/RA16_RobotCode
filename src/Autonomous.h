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
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

class Autonomous
{
public:
	Autonomous(Drive * d, Manipulation * m, Scoring * s, Logger * l, Timer * t);
	~Autonomous();


	void RunAuto();

	void ReloadConfig();

	void SetupLogging(Logger *Log); //starts logging
	void Log(Logger * log); //logs

private:
	Drive * drive;
	Manipulation * manipulation;
	Scoring * scoring;
	Logger * logger;
	Timer * logtimer;

	string autonomousState = "start";
	int autonum;
};



#endif /* SRC_AUTONOMOUS_H_ */
