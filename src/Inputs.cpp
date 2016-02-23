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
	op = o;
}

void Inputs::AddButtonMap(int controller, int button, void(*f)(void))
{
	ButtonMap[controller][button] = f;
}

void Inputs::AddAxisMap(int controller, int axisnum, void(*f)(float))
{
	AxisMap[controller][axisnum] = f;
}

void Inputs::Update()
{
	for(auto const &ent1 : ButtonMap)
	{
		if(ent1.first == 0)
		{
			for(auto const &ent2 : ent1.second)
			{
				if(driver->GetNumberedButton(ent2.first))
				{
					ent2.second();
				}
			}
		}
		else
		{
			for(auto const &ent2 : ent1.second)
			{
				if(op->GetNumberedButton(ent2.first))
				{
					ent2.second();
				}
			}
		}
	}

	for(auto const &ent1 : AxisMap)
	{
		if(ent1.first == 0)
		{
			for(auto const &ent2 : ent1.second)
			{
				ent2.second(driver->GetRawAxis(ent2.first));
			}
		}
		else
		{
			for(auto const &ent2 : ent1.second)
			{
				ent2.second(op->GetRawAxis(ent2.first));
			}
		}
	}
}
