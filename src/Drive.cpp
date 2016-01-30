/*
 * Drive.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Taylor Horne
 */

#include "Drive.h"
#include "WPILib.h"
#include "Logger.h"

Drive::Drive(CANTalon * fr, CANTalon * br, CANTalon * fl, CANTalon * bl)
{
	FR = fr;
	BR = br;
	FL = fl;
	BL = bl;

}

void Drive::HaloDrive(float x, float y)
{
	float left = y + x;
	float right = y - x;
	FR->Set(right);
	BR->Set(1);
	FL->Set(left);
	BL->Set(0);
}

void Drive::TankDrive(float l, float r)
{
	FR->Set(r);
	BR->Set(1);
	FL->Set(l);
	BL->Set(0);
}

void Drive::Log(Logger* logger)
{

	//logger->Log("Time", logtimer->Get());
	logger->Log("FLVoltage", FL->GetBusVoltage());
	logger->Log("FRVoltage", FR->GetBusVoltage());
	logger->Log("BLVoltage", BL->GetBusVoltage());
	logger->Log("BRVoltage", BR->GetBusVoltage());
	logger->Log("FLCurrent", FL->GetOutputCurrent());
	logger->Log("FRCurrent", FR->GetOutputCurrent());
	logger->Log("BLCurrent", BL->GetOutputCurrent());
	logger->Log("BRCurrent", BR->GetOutputCurrent());
	logger->WriteLine();
}
