#ifndef TARGETING_H__
#define TARGETING_H__

#include "WPILib.h"
class Targeting {
public:
	Targeting(Relay* light);

	void Process();
private:
	CameraServer* m_Server;
	std::shared_ptr<NetworkTable>  m_Grip;
};

#endif
