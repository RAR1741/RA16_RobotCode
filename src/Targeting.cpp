#include "Targeting.h"
#include <unistd.h>
#include <iostream>

/*Targeting::Targeting(Relay* light) :
	m_Server(NULL), m_Grip(NULL)
{
	char * GRIP_ARGS[5] = {"java" , "-jar" , "/home/lvuser/grip.jar" ,
		"/home/lvuser/project.grip",
		'\0'};
	const char *JAVA = "/usr/local/frc/JRE/bin/java";
	// kill all other grip instances to make sure we're ok on
	// memory
	system("ps aux | grep grip | grep -v grep | xargs kill");

	// Fork our program into 2 parts, the robot code and a shell for
	// GRIP. In one version (the "child"), fork() returns 0, so we
	// know to stop doing robot stuff and do GRIP stuff instead.
	// Otherwise we keep going to the rest of the program. The "parent"
	// gets an integer with the process id (pid) of the child, so
	// it can control it.

	if (fork() == 0) {
		if (execv(JAVA, GRIP_ARGS) == -1) {
			perror("Error running GRIP");
		}
	}

	m_Server = CameraServer::GetInstance();
	//m_Server->StartAutomaticCapture("cam0");
	m_Server->SetQuality(2);
	m_Server->StartAutomaticCapture();

	m_Grip = NetworkTable::GetTable("Targeting");
}*/

std::vector<Target> Targeting::GetTargets()
{
	std::string t  = m_Grip->GetString("targets");
	//std::vector<double> xs     = m_Grip->GetNumberArray("myContoursReport/x",    llvm::ArrayRef<double>());
	//std::vector<double> ys     = m_Grip->GetNumberArray("myContoursReport/y",    llvm::ArrayRef<double>());
	//std::vector<double> widths = m_Grip->GetNumberArray("myContourReport/width", llvm::ArrayRef<double>());

	std::vector<Target> targets;

	std::stringstream test(t);
	std::string segment;

	while(std::getline(test, segment, '|'))
	{
	   Target target(segment);
	   targets.push_back(target);
	}
	return targets;
}
