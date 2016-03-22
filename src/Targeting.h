#ifndef TARGETING_H__
#define TARGETING_H__

#include "WPILib.h"
#include "Target.h"
#include "Logger.h"
class Targeting {
public:
	Targeting(Relay* light);
	Targeting();
	std::vector<Target> GetTargets();
	void SetupLogging(Logger * log);
	void Log(Logger * log);
private:
	CameraServer* m_Server;
	std::shared_ptr<NetworkTable>  m_Grip;
	string trackingBlob;
};

#endif
