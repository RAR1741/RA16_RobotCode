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
	FR->Set(-right);
	BR->Set(8);
	FL->Set(left);
	BL->Set(7);
}

void Drive::TankDrive(float l, float r)
{
	FR->Set(r);
	BR->Set(8);
	FL->Set(l);
	BL->Set(7);
}

void Drive::SetupLogging(Logger * logger)
{
	logger->AddAttribute("Time");
	logger->AddAttribute("FLpos");
	logger->AddAttribute("FRpos");
	logger->AddAttribute("BLpos");
	logger->AddAttribute("BRpos");
	logger->AddAttribute("FLCurrent");
	logger->AddAttribute("FRCurrent");
	logger->AddAttribute("BLCurrent");
	logger->AddAttribute("BRCurrent");
}

void Drive::Log(Logger* logger)
{
	logger->Log("FLpos", FL->GetEncPosition());
	logger->Log("FRpos", FR->GetEncPosition());
	logger->Log("BLpos", BL->GetEncPosition());
	logger->Log("BRpos", BR->GetEncPosition());
	logger->Log("FLCurrent", FL->GetOutputCurrent());
	logger->Log("FRCurrent", FR->GetOutputCurrent());
	logger->Log("BLCurrent", BL->GetOutputCurrent());
	logger->Log("BRCurrent", BR->GetOutputCurrent());
	logger->WriteLine();
}
