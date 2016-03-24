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
