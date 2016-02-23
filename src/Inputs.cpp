/*
 * Inputs.cpp
 *
 *  Created on: Feb 22, 2016
 *      Author: taylo
 */

#include "WPILib.h"

Inputs::Inputs(Gamepad * d, Gamepad * o)
{
	driver = d;
	opertator = o;
}

void Inputs::AddButtonMap(int index, void(*f)(void))
{
	ButtonMap[index] = f;
}

void Inputs::AddAxisMap(int index, void(*f)(float))
{
	AxisMap[index] = f;
}

void Inputs::Update()
{
	typedef std::map<int, void(*)()>::iterator it_type_void;
	for(it_type_void iterator = ButtonMap.begin(); iterator != ButtonMap.end(); iterator++)
	{
		if(driver->GetNumberedButton(iterator->first))
		{
			iterator->second();
		}
	}
	typedef std::map<int, void(*)(float)>::iterator it_type_float;
	for(it_type_float iterator = AxisMap.begin(); iterator != AxisMap.end(); iterator++)
	{
		iterator->second(driver->GetRawAxis(iterator->first));
	}
}
