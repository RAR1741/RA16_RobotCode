/*
 * Inputs.cpp
 *
 *  Created on: Feb 22, 2016
 *      Author: taylo
 */

#include "WPILib.h"
#include "Inputs.h"

Inputs::Inputs()
{

}

void Inputs::AddButtonMap(Gamepad * controller, int button, void(*f)(void))
{
	ButtonMap[controller][button] = f;
}

void Inputs::AddAxisMap(Gamepad * controller, int axisnum, void(*f)(float))
{
	AxisMap[controller][axisnum] = f;
}

void Inputs::Update()
{
	for(auto const &ent1 : ButtonMap)
	{
		for(auto const &ent2 : ent1.second)
		{
			if(ent1.first->GetNumberedButton(ent2.first))
			{
				ent2.second();
			}
		}
	}

	for(auto const &ent1 : AxisMap)
	{
		for(auto const &ent2 : ent1.second)
		{
			ent2.second(ent1.first->GetRawAxis(ent2.first));
		}
	}
}
