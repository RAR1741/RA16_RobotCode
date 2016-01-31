#ifndef TARGET_H__
#define TARGET_H__

class Target {
public:
	Target(float x, float y, float width, float area);

	float Area() const;
	float X() const;
	float Y() const;
	float Width() const;

	float ApproximateRange() const;
private:
	float m_area;
	float m_x;
	float m_y;
	float m_width;

	const float kCameraHorizontalFOV = 41.7; // 54 degrees for axis 206,
											 // 41.7 supposedly works better
};
#endif
