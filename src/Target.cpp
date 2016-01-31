#include "Target.h"
#include <cmath>

Target::Target(float x, float y, float area, float width) :
  	 m_area(area), m_x(x), m_y(y), m_width(width)
{
	// Nothing to do here.
}

float Target::Area() const {
	return m_area;
}

float Target::X() const {
	return m_x;
}

float Target::Y() const {
	return m_y;
}

float Target::Width() const {
	return m_width;
}

float Target::ApproximateRange() const {
	// see https://wpilib.screenstepslive.com/s/4485/m/24194/l/288985-identifying-and-processing-the-targets
	// for derivation
	float targetWidth = 1; // feet
	float imageWidth = 640; // pixels


	float d = (targetWidth * imageWidth) /
			  (2 * this->Width() * ::tan(kCameraHorizontalFOV));
}
