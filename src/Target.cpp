#include "Target.h"
#include <cmath>
#include <sstream>
#include <vector>

Target::Target(float distance, float pan, float tilt)
{
	m_tilt = tilt;
  	m_distance = distance;
  	m_pan = pan;
	// Nothing to do here.
}

Target::Target(std::string in)
{
	stringstream test(in);
	std::string segment;

	vector<float> seglist;

	while(std::getline(test, segment, ','))
	{
	   seglist.push_back(stof(segment));
	}

	m_distance = seglist.at(0);
	m_pan = seglist.at(1);
	m_tilt = seglist.at(2);
}

float Target::Distance()
{
	return m_distance;
}

float Target::Pan()
{
	return m_pan;
}

/*float Target::ApproximateRange() const {
	// see https://wpilib.screenstepslive.com/s/4485/m/24194/l/288985-identifying-and-processing-the-targets
	// for derivation
	float targetWidth = 1; // feet
	float imageWidth = 640; // pixels


	float d = (targetWidth * imageWidth) /
			  (2 * this->Width() * ::tan(kCameraHorizontalFOV));
}*/
