/*
 * Scoring.h
 *
 *  Created on: Jan 23, 2016
 *      Author: taylo
 */

#ifndef SRC_SCORING_H_
#define SRC_SCORING_H_
#include "WPILib.h"

class Scoring
{
public:
    typedef enum
    {
        kLoading, kReady, kShooting, kWaiting
    } State;
	Scoring(CANTalon *aMotor, CANTalon *tMotor, Victor *lMotor, Victor *rMotor);
	void Update();
	void SetAngle(float angle);
	float GetAngle();
	State GetState();
	void SetFlySpeed(float speed);
	void Load();
	void Fire();
private:
	State state;
	CANTalon *AngleMotor;
	CANTalon *TensionMotor;
	Victor *LFlyMotor;
	Victor *RFlyMotor;
};

#endif /* SRC_SCORING_H_ */
