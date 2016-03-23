#include "FakePIDSource.h"

FakePIDSource::FakePIDSource() : m_value(0.0)
{

}

FakePIDSource::~FakePIDSource()
{

}

double FakePIDSource::PIDGet()
{
	return m_value;
}


void FakePIDSource::PIDSet(float value)
{
	m_value = value;
}
