#ifndef TARGETING_H__
#define TARGETING_H__

#include "WPILib.h"
#include "Target.h"
class Targeting {
public:
	Targeting(Relay* light);
	Targeting();
	std::vector<Target> GetTargets();
private:
	CameraServer* m_Server;
	std::shared_ptr<NetworkTable>  m_Grip;
};

#endif
