#ifndef FAKE_PIDSOURCE_H__
#define FAKE_PIDSOURCE_H__
#include "WPIlib.h"

class FakePIDSource : public PIDSource {
public:
	FakePIDSource();
	virtual ~FakePIDSource();
	virtual double PIDGet();

	void PIDSet(float value);
private:
	float m_value;
};

#endif
