/*
 * Inputs.h
 *
 *  Created on: Feb 22, 2016
 *      Author: taylo
 */

#ifndef SRC_INPUTS_H_
#define SRC_INPUTS_H_

#include "Gamepad.h"

using namespace std;

class Inputs
{
public:
	Inputs(Gamepad * d, Gamepad * o);
	void AddButtonMap(Gamepad * controller, int button, void(*f)(void));
	void AddAxisMap(Gamepad * controller, int axisnum, void(*f)(float));
	void Update();
private:
	map < Gamepad *, map < int, void (*)(void) > > ButtonMap;
	map < Gamepad *, map < int, void (*)(float) > > AxisMap;
	Gamepad * driver;
	Gamepad * op;
};



#endif /* SRC_INPUTS_H_ */
