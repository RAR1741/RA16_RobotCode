#ifndef TARGET_H__
#define TARGET_H__
#include <string>

using namespace std;

class Target {
public:
	Target(float distance, float pan, float tilt);
	Target(std::string in);

	float Distance();
	float Pan();
	float Tilt();
private:
	float m_distance;
	float m_pan;
	float m_tilt;

	//const float kCameraHorizontalFOV = 41.7; // 54 degrees for axis 206,
											 // 41.7 supposedly works better
};
#endif
