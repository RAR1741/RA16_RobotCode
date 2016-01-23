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
	Manipulation(CANTalon *bMotor, CANTalon *aMotor, DigitalInput *bLimit, DigitalInput *aLimit);
	void Train();
	void Set(int index);
	void ResetTrain();
	bool EditTrain(int index, float val[2]);
	//Manipulation(const Manipulation&);
private:
	bool ReadMotion(int in);
	void ReadPostions();
	CANTalon *BaseMotor;
	CANTalon *ArmMotor;
	DigitalInput *ArmLimit;
	DigitalInput *BaseLimit;
	//vector <vector<float[2]>> movements;
	vector <vector<float>> positions;
};

#endif /* SRC_MANIPULATION_H_ */
