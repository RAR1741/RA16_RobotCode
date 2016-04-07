/*
 * Dashboard.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: taylo
 */

#include "RobotHeader.h"

using namespace std;

Dashboard::Dashboard()
{
	station = NetworkTable::GetTable("Station");
}

void Dashboard::WriteBool(string name, bool write)
{
	station->PutBoolean(name, write);
}

void Dashboard::WriteString(string name, string write)
{
	station->PutString(name, write);
}

void Dashboard::WriteDouble(string name, double write)
{
	station->PutNumber(name, write);
}

bool Dashboard::ReadBool(string name, bool def)
{
	return station->GetBoolean(name, def);
}

string Dashboard::ReadString(string name, string def)
{
	return station->GetString(name, def);
}

double Dashboard::ReadDouble(string name, double def)
{
	return station->GetNumber(name, def);
}

bool Dashboard::CheckTrigger(string name)
{
	if(station->GetBoolean(name, false))
	{
		station->PutBoolean(name, true);
		return true;
	}
	else
	{
		return false;
	}
}
