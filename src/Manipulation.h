/*
 * Manipulation.h
 *
 *  Created on: Jan 20, 2016
 *      Author: Taylor Horne
 */

#ifndef SRC_MANIPULATION_H_
#define SRC_MANIPULATION_H_
#include "WPILib.h"
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

//	void SetPositionBase(int pos);
//	int GetPositionBase();
//
	//Manipulation(const Manipulation&);
private:
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
};

#endif /* SRC_MANIPULATION_H_ */
