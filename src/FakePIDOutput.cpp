#include "FakePIDOutput.h"

FakePIDOutput::FakePIDOutput() : m_value(0.0){
}

FakePIDOutput::~FakePIDOutput() {

}

void FakePIDOutput::PIDWrite(float value) {
	m_value = value;
}

float FakePIDOutput::PIDGet() {
	return m_value;
}
