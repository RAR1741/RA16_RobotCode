/*
 * Scoring.h
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#ifndef SRC_SCORING_H_
#define SRC_SCORING_H_
#include "WPILib.h"
#include "Logger.h"

class Scoring
{
public:
    typedef enum
    {
    	kWaiting = 0,
		kLoading,
		kIndexing,
		kArmed,
		kTrigger,
		kFiring,
		kReset
    } State;

	typedef enum
	{
		kStart = 0,
		kHomingUp = 1,
		kHomingDown = 2,
		kHomed = 3,
		kReady = 4
	}HomeState;
	Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor, DigitalInput *indexSensor, AnalogInput* absenc);
	void Update();
	void SetAngle(float angle);
	//void AngleHomeLoop();
	void Home();
	void SetPreIncrementalPos(int posNum);
	void SetPredefinedAngle(int posNum);
	float GetAngle();
	State GetState();

	float ValueToVoltage(float value);
	float DegreeToEnc(float value);
	void SetFlySpeed(float speed);
	void SetFireState(Scoring::State State);
	void SetEncPos(int encPos);
	void Load();
	void Fire();
	void ReloadConfig();
	void SetupLogging(Logger * logger);
	void Log(Logger * logger);
	void EnablePID(bool enabled);
private:
	State state;
	HomeState homeState;
	CANTalon *AngleMotor;
	CANTalon *TensionMotor;
	Victor *LFlyMotor;
	Victor *RFlyMotor;
	Timer * homingTimer;
	DigitalInput *IndexSensor;
	PIDController * AimLoop;
	AnalogInput *AngleEncoder;
	Timer * fireTimer;
	float ChooChooLoaded;
	float SP;
	float SI;
	float SD;
	float PP;
	float PI;
	float PD;
	float offset;
	int encPos1;
	int encPos2;
	int encPos3;
	int encPos4;
	int encPos5;
	int degPos1;
	int degPos2;
	int degPos3;
	int degPos4;
	int encHomePos;
	int holdStart;
	int holdInc;
	int holdFire;
	bool isPrototype;
};

#endif /* SRC_SCORING_H_ */
