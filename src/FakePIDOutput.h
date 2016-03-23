#ifndef FAKE_PID_OUTPUT_H__
#define FAKE_PID_OUTPUT_H__
#include "wpilib.h"

class FakePIDOutput : public PIDOutput {
public:
	FakePIDOutput();
	virtual ~FakePIDOutput();

	virtual void PIDWrite(float value);

	float PIDGet();
private:
	float m_value;
};

#endif
