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
	void AddButtonMap(int index, void(*f)(void));
	void AddAxisMap(int index, void(*f)(float));
	void Update();
private:
	map < int, void (*)(void) > ButtonMap;
	map < int, void (*)(float) > AxisMap;
	Gamepad * driver;
	Gamepad * opertator;
};



#endif /* SRC_INPUTS_H_ */
