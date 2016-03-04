#include "WPILib.h"
//#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>
#include <cstdlib>
#include "RobotHeader.h"

using namespace std;

#define TESTBED (0)

class Robot: public IterativeRobot
{
private:
	//controllers
	Gamepad *driver;
	Gamepad *op;
	EdgeDetect * trainTrigger;
	//drivetrain
	CANTalon * motorFL;
	CANTalon * motorFR;
	CANTalon * motorBL;
	CANTalon * motorBR;
    Drive *drive;
	//scoring
	CANTalon * puncher;
	CANTalon * aimer;
    DigitalInput * Index;
    DigitalInput * FlyLimit;
    AnalogInput * absenc;
    PIDController * aimLoop;
    Victor * lin;
    Victor * rin;
    Scoring * score;
	//manipulation
	CANTalon * motorBase;
	CANTalon * motorArm;
    DigitalInput * ArmLimit;
    DigitalInput * BaseLimit;
    Manipulation *arm;
    //logging
    Logger *logger;
    Logger * DriverLog;
    Logger * OpLog;
    Timer *logTime;
    //misc
    OutputLog * logthing = new OutputLog();
    CameraServer * m_server;
	LiveWindow * lw;
    AnalogGyro * gyro;
    Targeting *targeting;
    Relay *light;
    DigitalOutput * osciliscope1;
    DigitalOutput * osciliscope2;
    //variables
    bool triggerToggle;
    bool lastTrigger;
    bool isForward;
    bool getPos = false;
    int bottomHardLimit;
    int DegreesToZero;
    float autoPanP;
    float encoderTicksPerDegree;
    float tempLogTime;
    char * nope = '\0';
    std::string profile = "everything";
    uint8_t jim = false;//This is stupid
    int portmotion[] = {9,10,11,12,13,14};
public:
	Robot()
	{
		//controllers
		driver = NULL;
		op = NULL;
		trainTrigger = new EdgeDetect();
		//drivetrain
		motorFL = NULL;
		motorFR = NULL;
		motorBL = NULL;
		motorBR = NULL;
		drive = NULL;
		//scoring
		absenc = NULL;
		aimLoop = NULL;
		Index = NULL;
		aimer = NULL;
		puncher = NULL;
		lin = NULL;
		rin = NULL;
		FlyLimit = NULL;
		score = NULL;
		//manipulation
		motorBase = NULL;
		motorArm = NULL;
		ArmLimit = NULL;
		BaseLimit = NULL;
		arm = NULL;
		//logger
		logger = NULL;
		DriverLog = NULL;
		OpLog = NULL;
		logTime = NULL;
		//misc
		osciliscope1 = NULL;
		osciliscope2 = NULL;
		lw = NULL;
		gyro = new AnalogGyro(1);// = NULL;
		targeting = NULL;
		light = NULL;
		m_server = NULL;
		//variables
		autoPanP = 0;
		encoderTicksPerDegree = 0;
		bottomHardLimit = 0;
		DegreesToZero = 0;
		tempLogTime = 0;
		triggerToggle = false;
		lastTrigger = false;
		isForward = true;
		//targeting = NULL;
	};

	void RobotInit()
	{
		cout << "Initializing Drivetrain..." << endl;
		//Initialize drive motors
		motorFL = new CANTalon(7);
		motorBL = new CANTalon(9);
		motorFR = new CANTalon(8);
		motorBR = new CANTalon(10);

		drive = new Drive(motorFR, motorBR, motorFL, motorBL);
		cout << "Drivetrain initialized!" << endl;

		cout << "Initializing scoring..." << endl;
		Index = new DigitalInput(0);
		FlyLimit = new DigitalInput(1);
		absenc = new AnalogInput(0);

		puncher = new CANTalon(4);
		aimer = new CANTalon(3);

		rin = new Victor(0);
		lin = new Victor(1);

		score = new Scoring(aimer,puncher,lin,rin,Index,absenc);
		cout << "Scoring initialized!" << endl;

		cout << "Initalizing arm..." << endl;
		motorBase = new CANTalon(2);
		motorArm = new CANTalon(1);
		ArmLimit = new DigitalInput(6);
		BaseLimit = new DigitalInput(3);
		arm = new Manipulation(motorBase, motorArm, BaseLimit, ArmLimit);

		arm->CreateMotion(portmotion, 6);

		cout << "Arm initialized!" << endl;

//		cout << "Initializing USB camera" << endl;
//		m_server = CameraServer::GetInstance();
//		m_server->StartAutomaticCapture("cam1");
//		m_server->SetQuality(2);
//		m_server->StartAutomaticCapture();
//		cout << "Camera initialized!" << endl;

		cout << "Finishing up...";
		cout << "Initializing targeting..." << endl;
		light = new Relay(0);
		targeting = new Targeting();
		cout << "Targeting initialized" << endl;

		cout << "Initializing driver/op input..." << endl;
		driver = new Gamepad(0);
		op = new Gamepad(1);
		cout << "driver/op input initialized" << endl;;

		cout << "Initializing logger..." << endl;
		logTime = new Timer;
		logTime->Start();
		logger = new Logger();
		DriverLog = new Logger();
		OpLog = new Logger();
		cout << "Logger Initialized!" << endl;

		cout << "Reloading configuration..." << endl;
		ReloadConfig();
		cout << "Config reloaded." << endl;

		cout << "Done!" << endl;

		srand(time(NULL));
		switch(rand() % 3) {
		case 0:
			logthing->Troll(cout);
			break;
		case 1:
			logthing->Mesa(cout);
			break;
		case 2:
			logthing->Aperture(cout);
			break;
		default:
			logthing->Troll(cout);
			break;
		}
	}

	void TeleopInit()
	{
		score->SetPredefinedAngle(3);
		score->EnablePID(false);
		//light->Set(Relay::Value::kOn);
		StartLogging("teleop", logger);
		StartLogging("driver", DriverLog);
		StartLogging("operator", OpLog);
		SetupLogging();
		SetupControllerLog(DriverLog);
		SetupControllerLog(OpLog);
		ReloadConfig();
	}

	void TeleopPeriodic()
	{
		//DRIVE
		if(driver->GetRightBumper())//Full speed mode
		{
			drive->HaloDrive(Deadband(driver->GetRightX())* 0.7, forward(Deadband(driver->GetLeftY())));
		}
		else if(driver->GetLeftBumper())//Targeting mode
		{
			vector<Target> targets;
			targets = targeting->GetTargets();
			if(targets.size() != 0)
			{
				Target closest = targets.at(0);
				for (unsigned int i = 0; i < targets.size(); i++)
				{
					if(targets.at(i).Distance() < closest.Distance())
					{
						closest = targets.at(i);
					}
				}
				drive->HaloDrive(-autoPanP * closest.Pan(), 0);
				//aimLoop->SetSetpoint(targetDegreeToTicks(closest.Tilt()) / 800);
			}
			else
			{
				drive->HaloDrive(0,0);
			}
		}
		else //normal drive mode
		{
			drive->HaloDrive(Deadband(driver->GetRightX()) * 0.7, forward(Deadband(driver->GetLeftY() * 0.6)));
		}

		//switch drive direction
		if(driver->GetDPad() == Gamepad::DPadDirection::kUp)
		{
			isForward = true;
		}
		else if(driver->GetDPad() == Gamepad::DPadDirection::kDown)
		{
			isForward = false;
		}

		//run the scoring flywheels
		if(fabs(op->GetRTriggerAxis()) >= .1 && !FlyLimit->Get())
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

		//load scoring
		if(driver->GetX())
		{
			score->Load();
		}
		//fire scoring
		if(driver->GetRTriggerAxis()>=.5)
		{
			score->Fire();
		}

		// If in SHOOT MODE
		//allow scoring aim PID
		if (op->GetRightBumper())
		{
			score->EnablePID(true);
		}
		else
		{
			score->EnablePID(false);
			aimer->Set(0);
		}

//		//scoring aim manual
//		if(fabs(driver->GetRightY()) >= 0.1)
//		{
//			aimer->Set(driver->GetRightY() * .5);
//		}
//		else
//		{
//			aimer->Set(0);
//		}

		//set scoring to predefined positions 0-3
		if(op->GetA())
		{
			score->SetPredefinedAngle(0);
		}
		else if(op->GetB())
		{
			score->SetPredefinedAngle(1);
		}
		else if(op->GetX())
		{
			score->SetPredefinedAngle(2);
		}
		else if(op->GetY())
		{
			score->SetPredefinedAngle(3);
		}

		//manual drive manipulation
		if(fabs(op->GetRightY())>.1 && !(fabs(op->GetLeftY())>.1))
		{
			cout << "Derp0\n";
			arm->GoToAngles(arm->BaseAngle() + (op->GetRightY() * 10),arm->ArmAngle());
		}
		if(fabs(op->GetLeftY())>.1 && !(fabs(op->GetRightY())>.1))
		{
			cout << "Derp1\n";
			arm->GoToAngles(arm->BaseAngle(),arm->ArmAngle() + (op->GetLeftY() * 15));
		}

		//reset training
		if(op->GetBack())
		{
			arm->ResetTrain();
		}

		//create a trained point
		if(trainTrigger->Check(op->GetStart()))
		{
			arm->Train();
		}

		//manipulation setpoints
		if(op->GetDPad() == Gamepad::kDown) //home
		{
			arm->Home();
		}
		if(op->GetDPad() == Gamepad::kLeft)
		{
			arm->Set(3);
		}
		if(op->GetDPad() == Gamepad::kRight)
		{
			arm->Set(1);
			//arm->GoToAngles(-140,70);
		}
		if(op->GetDPad() == Gamepad::kUp)
		{
			arm->Set(2);
		}

		if(op->GetBack())
		{
			arm->StartMotion(0);
		}

		if(arm->movementRunning && arm->isNear())
		{
			arm->ContinueMotion();
		}

		//update scoring and manipulation
		score->Update();
		arm->Process();

		//testing
//		cout << "Arm angles:\n";
//		cout << "base: " << arm->BaseAngle() << endl;
//		cout << "arm:  " << arm->ArmAngle() << endl;
//		cout << aimLoop->Get() << endl;
//		cout << "EncValue: " << aimer->GetEncPosition() << endl;
//		cout << "Setpoint: " << aimLoop->GetSetpoint() << endl;
		cout << "AbsEncValue: " << absenc->GetVoltage()* 800.0F << "\n";
		cout << "AbsEncVolt: " << absenc->GetVoltage() << "\n";
//		cout << "BaseEnc: " << motorBase->GetEncPosition() << endl;
		tempLogTime = logTime->Get();
		Log(tempLogTime);
		LogController(DriverLog, driver, tempLogTime);
		LogController(OpLog, op, tempLogTime);
	}

	void DisabledInit()
	{
		light->Set(Relay::Value::kOff);
		logger->Close();
	}

	void AutonomousInit()
	{
		StartLogging("auto", logger);
	}

	void AutonomousPeriodic()
	{
		tempLogTime = logTime->Get();
		Log(tempLogTime);
	}

	void TestInit()
	{
		StartLogging("test", logger);
	}

	void TestPeriodic()
	{
		tempLogTime = logTime->Get();
		Log(tempLogTime);
	}

	void StartLogging(string mode, Logger * l)
	{
		l->Close();
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
				std::to_string(now->tm_min) + "-\0" + std::to_string(now->tm_sec) + "-\0" + mode + ".csv";
		cout << name << endl;
		l->Open(name);
	}

	void SetupLogging()
	{
		logger->AddAttribute("Time");
		drive->SetupLogging(logger);
		score->SetupLogging(logger);
		arm->SetupLogging(logger);
		logger->WriteAttributes();
	}

	void Log(float time)
	{
		logger->Log("Time", time);
		drive->Log(logger);
		score->Log(logger);
		arm->Log(logger);
		logger->WriteLine();
	}

	void SetupControllerLog(Logger * l)
	{
		l->AddAttribute("Time");
		l->AddAttribute("LeftXAxis");
		l->AddAttribute("LeftYAxis");
		l->AddAttribute("RightXAxis");
		l->AddAttribute("RightYAxis");
		l->AddAttribute("LTriggerAxis");
		l->AddAttribute("RTriggerAxis");
		l->AddAttribute("A");
		l->AddAttribute("B");
		l->AddAttribute("X");
		l->AddAttribute("Y");
		l->AddAttribute("LBumper");
		l->AddAttribute("RBumper");
		l->AddAttribute("Back");
		l->AddAttribute("Start");
		l->AddAttribute("LStickButton");
		l->AddAttribute("RStickButton");
		l->AddAttribute("Dpad");
		l->WriteAttributes();
	}

	void LogController(Logger * l, Gamepad * g, float time)
	{
		l->Log("Time", time);
		l->Log("LeftXAxis", g->GetLeftX());
		l->Log("LeftYAxis", g->GetLeftY());
		l->Log("RightXAxis", g->GetRightX());
		l->Log("RightYAxis", g->GetRightY());
		l->Log("LTriggerAxis", g->GetLTriggerAxis());
		l->Log("RTriggerAxis", g->GetRTriggerAxis());
		l->Log("A", g->GetA());
		l->Log("B", g->GetB());
		l->Log("X", g->GetX());
		l->Log("Y", g->GetY());
		l->Log("LBumper", g->GetLeftBumper());
		l->Log("RBumper", g->GetRightBumper());
		l->Log("Back", g->GetBack());
		l->Log("Start", g->GetStart());
		l->Log("LStickButton", g->GetLeftPush());
		l->Log("RStickButton", g->GetRightPush());
		l->Log("Dpad", g->GetPOV());
		l->WriteLine();
	}

	void ReloadConfig()
	{
		Config::LoadFromFile("/home/lvuser/config.txt");
		cout << Config::GetSetting("wow", 0) << endl;
		autoPanP = Config::GetSetting("autoAimP", 0.08);
		encoderTicksPerDegree = Config::GetSetting("encodeTicksPerDegree", 10.78);
		bottomHardLimit = Config::GetSetting("bottomHardLimit", 3635);
		DegreesToZero = Config::GetSetting("degreesToZero", 10);
		arm->ReloadConfig();
		score->ReloadConfig();
	}

	bool DeadbandCheck(float in, float dead = 0.1)
	{
		if(fabs(in) >= dead)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	float Deadband(float in, float dead = 0.1)
	{
		if(fabs(in) >= dead)
		{
			return in;
		}
		else
		{
			return 0;
		}
	}

	int targetDegreeToTicks(float in)
	{
		return (bottomHardLimit - ((in + DegreesToZero) * encoderTicksPerDegree));
	}

	float forward(float in)
	{
		if(isForward)
		{
			return in;
		}
		else
		{
			return -in;
		}
	}
};

START_ROBOT_CLASS(Robot)
