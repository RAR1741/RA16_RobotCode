#include "WPILib.h"
//#include <string>
#include <iostream>
#include "CameraServer.h"

using namespace std;

class Robot: public IterativeRobot
{
private:
	Joystick * joy1;
	Victor * motor1;
	LiveWindow * lw;
	SendableChooser *chooser;
	CameraServer * cameraUSB;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
    AnalogGyro * gyro;

public:
	Robot()
	{
		motor1 = NULL;
		joy1=NULL;
		lw = NULL;
		chooser = NULL;
		cameraUSB = NULL;
		gyro = NULL;
	};

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		chooser = new SendableChooser();
		cameraUSB = CameraServer::GetInstance();
		//cameraUSB->StartAutomaticCapture("cam0");
		cameraUSB->SetQuality(2);
		cameraUSB->StartAutomaticCapture();

		gyro = new AnalogGyro(1);

		//chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		//chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);

		SmartDashboard::PutData("Auto Modes", chooser);
		lw->AddSensor((const std::string)"Main", 0, gyro);
		joy1 = new Joystick(0);
		motor1 = new Victor(0);
	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom)
		{
			//Custom Auto goes here
		}
		else
		{
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		motor1->Set(joy1->GetX());
		cout << gyro->GetAngle() << endl;
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
