/*
 * Manipulation.h
 *
 *  Created on: Jan 20, 2016
 *      Author: Taylor Horne
 */

#ifndef SRC_MANIPULATION_H_
#define SRC_MANIPULATION_H_

#include "WPILib.h"
#include "Logger.h"

using namespace std;
class Manipulation
{
public:
	Manipulation(CANTalon *bMotor, CANTalon *aMotor, DigitalInput *bLimit=NULL, DigitalInput *aLimit=NULL);
	typedef enum
	{
		kStart = 0,
		//kHomingUp = 1,
		kHomingDown = 2,
		kHomed = 3,
		kReady = 4
	}State;
	void Train();
	void Set(int index);
	void ResetTrain();
	bool EditTrain(int index, float val[2]);
	void Update();
	void CreateMotion(int pos[], int size);
	void StartMotion(int index);
	void ContinueMotion();
	void Home();
	void Process();
	State GetState();
	void ReloadConfig();

	void ManualDrive(float base, float arm);
	void GoToAngles(float baseDegrees, float armDegrees);
	void GoToXY(float x, float y);

	float ArmAngle()  { return ArmDegreesByEncoder(ArmMotor->GetPosition());   }
	float BaseAngle() { return BaseDegreesByEncoder(BaseMotor->GetPosition()); }

	bool IsArmHome();
	bool IsBaseHome();

	void SetupLogging(Logger * logger);
	void Log(Logger * logger);

	float X();
	float Y();


private:
	// Utility methods to convert between revolutions and degrees
	int BaseEncoderByDegrees(float degrees);
	float BaseDegreesByEncoder(int encoderTicks);
	float ArmEncoderByDegrees(float degrees);
	int ArmDegreesByEncoder(int encoderTicks);

	float DegreesToRadians(float degrees);
	float RadiansToDegrees(float radians);

	float BaseLength() {
		return _base_length;
	}

	float ArmLength() {
		return _arm_length;
	}

	float X0() {
		return _x0;
	}

	float Y0() {
		return _y0;
	}

	bool ReadMotion(int in);
	void ReadPostions();
	CANTalon *BaseMotor;
	CANTalon *ArmMotor;
	DigitalInput *ArmLimit;
	DigitalInput *BaseLimit;
	vector <vector<int>> movements;
	vector <vector<float>> positions;
	State state;
	Timer * homingTimer;
	int currentMov;
	int place;
	float absZeroPos;
	float absToInc;
	AnalogInput *BaseAbs;
	const float kEncoderTicksPerDegree = 19.91111111;
	const float kRevolutionsPerDegree = 0.71111111;

	float _x0;
	float _y0;

	float _arm_length;
	float _base_length;
};

#endif /* SRC_MANIPULATION_H_ */
