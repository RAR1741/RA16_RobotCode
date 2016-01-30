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
#include "Drive.h"
#include "Scoring.h"
#include "Manipulation.h"

using namespace std;

class Autonomous
{
public:
	Autonomous(Drive * d, Manipulation * m, Scoring * s, Logger * l, Timer t);
	~Autonomous();


	void RunAuto();

	void ReloadConfig();

	//void SetupLogging(Logger *Log); //starts logging
	void Log(); //logs

private:
	Drive * drive;
	Manipulation * manipulation;
	Scoring * scoring;
	Logger * logger;
	Timer * logtimer;

	string autonomousState;

};



#endif /* SRC_AUTONOMOUS_H_ */
