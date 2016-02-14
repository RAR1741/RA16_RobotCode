#include "WPILib.h"
//#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include "Logger.h"
#include "Gamepad.h"
#include "Manipulation.h"
#include "Drive.h"
#include "Targeting.h"
#include "Config.h"
#include "Scoring.h"
#include "outputlog.h"

using namespace std;

#define TESTBED (0)

class Robot: public IterativeRobot
{
private:
	Gamepad *driver;
	Gamepad *op;
	///////////////////
	CANTalon * motorFL;
	CANTalon * motorFR;
	CANTalon * motorBL;
	CANTalon * motorBR;
	/////////////////////
	CANTalon * motorBase;
	CANTalon * motorArm;
	////////////////////
	LiveWindow * lw;
	////////////////////////////
	//SendableChooser *chooser;
	//const std::string autoNameDefault = "Default";
	//const std::string autoNameCustom = "My Auto";
	//std::string autoSelected;
	//////////////////////////////
    AnalogGyro * gyro;
    ////////////////////////////
    Logger *logger;
    Timer *logTime;
    Servo *xServo;
    Servo *yServo;
    Manipulation *arm;
    Drive *drive;
    Targeting *targeting;
    Relay *light;
    ///////////////////////
    DigitalOutput * osciliscope1;
    DigitalOutput * osciliscope2;
    /////////////////////////////
    CANTalon * puncher;
    DigitalInput * Index;
    CANTalon * aimer;
    Victor * lin;
    Victor * rin;
    //////////////////////////
    Scoring * score;
    OutputLog * logthing = new OutputLog();


    std::string profile = "everything";
    char * nope = '\0';

    uint32_t jim = false;


    bool getPos = false;
    //string *nope = NULL;

    //std::string GRIP_ARGS = "java -jar /home/lvuser/GRIP.jar /home/lvuser/" + profile + ".grip";

public:
	Robot()
	{
		Index = NULL;
		op = NULL;
		score = NULL;
		lin = NULL;
		rin = NULL;
		aimer = NULL;
		puncher = NULL;
		osciliscope1 = NULL;
		osciliscope2 = NULL;
        driver = NULL;
		motorFL = NULL;
		motorFR = NULL;
		motorBL = NULL;
		motorBR = NULL;
		motorBase = NULL;
		motorArm = NULL;
		lw = NULL;
		//chooser = NULL;
		logger = NULL;
		logTime = NULL;
		xServo = NULL;
		yServo = NULL;
		arm = NULL;
		drive = NULL;
		gyro = new AnalogGyro(1);// = NULL;
		targeting = NULL;
		light = NULL;
	};

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		//chooser = new SendableChooser();

		//light = new Relay(0);

		// turn this on once we have a camera connected
		// targeting = new Targeting(light);

		driver = new Gamepad(0);
		//op = new Gamepad(1);
		//gyro = new AnalogGyro(1);

		//chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		//chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);

		//SmartDashboard::PutData("Auto Modes", chooser);
		//lw->AddSensor((const std::string)"Main", 0, gyro);
//		cout << "Arm initializing\n";
//		motorBase = new CANTalon(2);
//		motorBase->SetControlMode(CANSpeedController::kPosition);
//		motorBase->SetPID(5,.0001,0);
//		motorArm = new CANTalon(1);
//		motorArm->SetControlMode(CANSpeedController::kPosition);
//		motorArm->SetPID(1,0,0);
//		cout << "Arm set to pos: " << motorArm->GetEncPosition();
//		arm = new Manipulation(motorBase, motorArm, NULL, NULL);

#if !TESTBED
		motorFL = new CANTalon(7);
		motorFL->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateQuadEncoder,6);
		motorBL = new CANTalon(9);
		motorBL->SetControlMode(CANTalon::ControlMode::kFollower);

		motorFR = new CANTalon(8);
		motorBR = new CANTalon(10);
		motorBR->SetControlMode(CANTalon::ControlMode::kFollower);

		drive = new Drive(motorFR, motorBR, motorFL, motorBL);
#else
		osciliscope1 = new DigitalOutput(8);
		osciliscope2 = new DigitalOutput(9);
#endif


		//xServo = new Servo(0);
		//yServo = new Servo(1);

		logTime = new Timer;
		logTime->Start();
		logger = new Logger();


		Index = new DigitalInput(0);
		puncher = new CANTalon(4);
		puncher->SetControlMode(CANTalon::kPercentVbus);
		//puncher->SetAllowableClosedLoopErr(5000);
		puncher->EnableZeroSensorPositionOnIndex(true, false);
		//puncher->SetPID(.1,0,0);
		puncher->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateQuadEncoder, 6);
		puncher->Enable();
		aimer = new CANTalon(3);
		aimer->SetControlMode(CANTalon::kPercentVbus);
		rin = new Victor(0);
		lin = new Victor(1);

		score = new Scoring(aimer,puncher,lin,rin,Index,NULL,NULL);

		ReloadConfig();
		//StartLogging("init");
		logthing->Troll(cout);
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
		StartLogging("auto");
		//autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		//std::cout << "Auto selected: " << autoSelected << std::endl;

		//if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		//} else {
			//Default Auto goes here
		//}
	}

	void AutonomousPeriodic()
	{
//		if(autoSelected == autoNameCustom)
//		{
//			//Custom Auto goes here
//		}
//		else
//		{
//			//Default Auto goes here
//		}
		Log();
	}

	void TeleopInit()
	{
		StartLogging("teleop");
		cout << "thing" << endl;
	}

	void DisabledInit()
	{
		logger->Close();
	}

	void TeleopPeriodic()
	{
//		xServo->Set((driver->GetRightX() + 1) / 2);
//		yServo->Set((driver->GetRightY() + 1) / 2);
		if(driver->GetRightBumper())
		{
			drive->HaloDrive(driver->GetRightX()* 0.6, -driver->GetLeftY());
		}
		else
		{
			drive->HaloDrive(driver->GetRightX() * 0.6, -driver->GetLeftY() * 0.6);
		}
//
//		if(op->GetA())
//		{
//			getPos = true;
//		}
//		else
//		{
//			getPos = false;
//		}
//
		if(fabs(op->GetRTriggerAxis()) >= .1)
		{
			score->SetFlySpeed(op->GetRTriggerAxis()*4/5);
		}
		else if(fabs(op->GetLTriggerAxis()) >= .1)
		{
			score->SetFlySpeed(-(op->GetLTriggerAxis())*4/5);
		}
		else
		{
			score->SetFlySpeed(0);
		}
//
//		if(fabs(op->GetRightX()) >= 0.1)//fabs(op->GetRightX()) >= 0.1)
//		{
//			puncher->Set(op->GetRightX() *.5);//op->GetRightX() *
//		}
//		else
//		{
//			puncher->Set(0);
//		}
//
		if(fabs(op->GetLeftX()) >= 0.1)
		{
			aimer->Set(op->GetLeftX() * .5);
		}
		else
		{
			aimer->Set(0);
		}

		if(driver->GetA())
		{
			score->Load();
		}

		if(driver->GetB())
		{
			score->Fire();
		}
		//puncher->Set(3000);
//		cout << puncher->GetPinStateQuadIdx() << endl;
//		cout << puncher->GetEncPosition() << endl;
//		cout << score->GetState() << endl;
		score->Update();
		//cout << aimer->GetPinStateQuadIdx();
		//Log();
	}

	void TestInit()
	{
		StartLogging("test");
	}

	void TestPeriodic()
	{
		Log();
		//lw->Run();
	}

	void StartLogging(string mode)
	{
		logger->Close();
		time_t t = time(0);
		struct tm *now = localtime(&t);
		//don't touch it
		std::string dir = "/home/lvuser";
		struct stat st;
		if(stat("/media/sda",&st) == 0)
		{
		    if(st.st_mode & (S_IFDIR != 0))
		    {
		    	dir = "/media/sda";
		    }
		}
		std::string name = dir + "/log-" +
				std::to_string(now->tm_year + 1900) +
				"-\0" +
				std::to_string(now->tm_mon + 1) +
				"-\0" + std::to_string(now->tm_mday) + "_\0" +
				std::to_string(now->tm_hour) + "-\0" +
				std::to_string(now->tm_min) + "-\0" + std::to_string(now->tm_sec) + mode + ".csv";
		cout << name << endl;
		logger->Open(name);
		SetupLogging();
	}

	void SetupLogging()
	{
		logger->AddAttribute("Time");
		logger->AddAttribute("FLpos");
		logger->AddAttribute("FRpos");
		logger->AddAttribute("BLpos");
		logger->AddAttribute("BRpos");
		logger->AddAttribute("FLCurrent");
		logger->AddAttribute("FRCurrent");
		logger->AddAttribute("BLCurrent");
		logger->AddAttribute("BRCurrent");
		logger->AddAttribute("AimPos");
		logger->AddAttribute("PunchPos");
		logger->AddAttribute("AimCurrent");
		logger->AddAttribute("PunchCurrent");
		logger->AddAttribute("getPos");
		logger->WriteAttributes();
	}

	void Log()
	{
		drive->Log(logger);

		logger->Log("Time", logTime->Get());
#if !TESTBED
		logger->Log("FLpos", motorFL->GetEncPosition());
		logger->Log("FRpos", motorFR->GetEncPosition());
		logger->Log("BLpos", motorBL->GetEncPosition());
		logger->Log("BRpos", motorBR->GetEncPosition());
		logger->Log("FLCurrent", motorFL->GetOutputCurrent());
		logger->Log("FRCurrent", motorFR->GetOutputCurrent());
		logger->Log("BLCurrent", motorBL->GetOutputCurrent());
		logger->Log("BRCurrent", motorBR->GetOutputCurrent());
		logger->Log("AimPos", aimer->GetEncPosition());
		logger->Log("PunchPos", puncher->GetEncPosition());
		logger->Log("AimCurrent", aimer->GetOutputCurrent());
		logger->Log("PunchCurrent", puncher->GetOutputCurrent());
		logger->Log("getPos", getPos);
#endif
		logger->WriteLine();
		//cout << logTime->Get() << endl;
	}

	void OsciliscopeBeginning()
	{
		//osciliscope1->Set(jim);
	}

	void OsciliscopeEnd()
	{

	}

	void ReloadConfig()
	{
		Config::LoadFromFile("/home/lvuser/config.txt");
		cout << Config::GetSetting("wow", 0) << endl;
	}
};

START_ROBOT_CLASS(Robot)
