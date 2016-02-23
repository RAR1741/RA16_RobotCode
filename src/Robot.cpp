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
#include "Targeting.h"

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
    DigitalInput * FlyLimit;
    DigitalInput * ArmLimit;
    DigitalInput * BaseLimit;
    CANTalon * aimer;
    AnalogInput * absenc;
    PIDController * aimLoop;
    Victor * lin;
    Victor * rin;
    CameraServer * m_server;
    //////////////////////////
    Scoring * score;
    OutputLog * logthing = new OutputLog();
    //Targeting * targeting;

    bool triggerToggle;
    bool lastTrigger;
    bool isForward;


    std::string profile = "everything";
    char * nope = '\0';

    uint32_t jim = false;//This is stupid


    bool getPos = false;

    float autoPanP;
    float encoderTicksPerDegree;
    int bottomHardLimit;
    int DegreesToZero;
    //string *nope = NULL;

    //std::string GRIP_ARGS = "java -jar /home/lvuser/GRIP.jar /home/lvuser/" + profile + ".grip";

public:
	Robot()
	{
		absenc = NULL;
		aimLoop = NULL;
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
		FlyLimit = NULL;
		gyro = new AnalogGyro(1);// = NULL;
		targeting = NULL;
		light = NULL;
		ArmLimit = NULL;
		BaseLimit = NULL;
		m_server = NULL;
		autoPanP = 0;
		encoderTicksPerDegree = 0;
		bottomHardLimit = 0;
		DegreesToZero = 0;
		triggerToggle = false;
		lastTrigger = false;
		isForward = true;
		//targeting = NULL;
	};

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		light = new Relay(0);

//		m_server = CameraServer::GetInstance();
//		m_server->StartAutomaticCapture("cam1");
//		m_server->SetQuality(2);
//		m_server->StartAutomaticCapture();

		driver = new Gamepad(0);
		op = new Gamepad(1);

#if !TESTBED

		motorFL = new CANTalon(7);
		motorFL->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateQuadEncoder,6);
		motorBL = new CANTalon(9);
		motorBL->SetControlMode(CANTalon::ControlMode::kFollower);

		motorFR = new CANTalon(8);
		motorBR = new CANTalon(10);
		motorBR->SetControlMode(CANTalon::ControlMode::kFollower);

		drive = new Drive(motorFR, motorBR, motorFL, motorBL);

		targeting = new Targeting();
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
		FlyLimit = new DigitalInput(1);

		puncher = new CANTalon(4);
		puncher->SetControlMode(CANTalon::kPercentVbus);
		//puncher->SetAllowableClosedLoopErr(5000);
		puncher->EnableZeroSensorPositionOnIndex(true, false);
		//puncher->SetPID(.1,0,0);
		puncher->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateQuadEncoder, 6);
		puncher->Enable();
		aimer = new CANTalon(3);
		aimer->SetInverted(false);
		aimer->SetControlMode(CANTalon::kPercentVbus);
		rin = new Victor(0);
		lin = new Victor(1);

		absenc = new AnalogInput(0);

		aimLoop = new PIDController(19, 2, 0, absenc, aimer ,0.05);
		aimLoop->SetContinuous(false);
		aimLoop->SetPIDSourceType(PIDSourceType::kDisplacement);
		aimLoop->SetInputRange(0,4.8);
		aimLoop->SetOutputRange(-.9,.9);
		aimLoop->Disable();

		score = new Scoring(aimer,puncher,lin,rin,Index,aimLoop,NULL,NULL);

		cout << "Arm initializing";
		motorBase = new CANTalon(2);
		motorArm = new CANTalon(1);
		ArmLimit = new DigitalInput(6);
		BaseLimit = new DigitalInput(3);
		arm = new Manipulation(motorBase, motorArm, BaseLimit, ArmLimit);
		cout << "done" << endl;
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
	}

	void AutonomousPeriodic()
	{
		Log();
	}

	void TeleopInit()
	{
		score->SetPredefinedAngle(4);
		aimLoop->Disable();
		//light->Set(Relay::Value::kOn);
		StartLogging("teleop");
		cout << "thing" << endl;

		score->ReloadConfig();
		arm->ReloadConfig();
	}

	void DisabledInit()
	{
		light->Set(Relay::Value::kOff);
		logger->Close();
	}

	void TeleopPeriodic()
	{
//		xServo->Set((driver->GetRightX() + 1) / 2);
//		yServo->Set((driver->GetRightY() + 1) / 2);
		if(driver->GetRightBumper())
		{
			drive->HaloDrive(Deadband(driver->GetLeftX())* 0.7, forward(Deadband(driver->GetLeftY())));
		}
		else if(driver->GetLeftBumper())
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
		else
		{
			drive->HaloDrive(Deadband(driver->GetLeftX()) * 0.7, forward(Deadband(driver->GetLeftY() * 0.6)));
		}

		if(DeadbandCheck(driver->GetRightX()) || forward(DeadbandCheck(driver->GetLeftY())))
		{
			//aimer->SetControlMode(CANTalon::ControlMode::kPercentVbus);
//			aimLoop->Disable();
//			aimer->Set(0);
		}
		else
		{
//			aimLoop->Enable();
		}

		if(driver->GetDPad() == Gamepad::DPadDirection::kUp)
		{
			isForward = true;
			//m_server->StartAutomaticCapture("cam0");
		}
		else if(driver->GetDPad() == Gamepad::DPadDirection::kDown)
		{
			isForward = false;
			//m_server->StartAutomaticCapture("cam1");
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


		if(driver->GetA())
		{
			score->Load();
		}

		if(driver->GetB())
		{
			score->Fire();
		}


		// If in SHOOT MODE
		if (op->GetRightBumper())
		{
			aimLoop->Enable();
		}
		else
		{
			aimLoop->Disable();
			aimer->Set(0);
		}
//
//		if(fabs(driver->GetRightY()) >= 0.1)
//		{
//			aimer->Set(driver->GetRightY() * .5);
//		}
//		else
//		{
//			aimer->Set(0);
//		}

			if(op->GetA())
			{
				score->SetPredefinedAngle(1);
			}
			else if(op->GetB())
			{
				score->SetPredefinedAngle(2);
			}
			else if(op->GetX())
			{
				score->SetPredefinedAngle(3);
			}
			else if(op->GetY())
			{
				score->SetPredefinedAngle(4);
			}

//			if(op->GetA())
//			{
//				score->SetPreIncrementalPos(1);
//			}
//			else if(op->GetB())
//			{
//				score->SetPreIncrementalPos(2);
//			}
//			else if(op->GetX())
//			{
//				score->SetPreIncrementalPos(3);
//			}
//			else if(op->GetY())
//			{
//				score->SetPreIncrementalPos(4);
//			}

			// MANIP MODE XD
#if 0 // NOTHING
			if (op->GetA()) {
				arm->Set(0);
			} else if (op->GetB()) {
				arm->Set(1);
			} else if (op->GetX()) {
				arm->Set(2);
			} else if (op->GetY()) {
				arm->Set(3);
			}
#endif


//		if(op->GetDPad() == Gamepad::kLeft)
//		{
//			score->Home();
//		}

//		if(fabs(op->GetRightY())>.1)
//		{
//			arm->SetPositionBase(arm->GetPositionBase() + (op->GetRightY() * 10));
//			//motorBase->Set(op->GetRightY()*.75);
//		}
//		else
//		{
		//arm->GoToAngles(10, 0);
		//arm->ManualDrive(op->GetRightY(), op->GetLeftY());
		//arm->Process();
//		}

//		if(fabs(op->GetLeftY())>.1)
//		{
//			aimer->Set(-op->GetLeftY()* 0.5);
//			cout << absenc->GetVoltage() * 800 << endl;
//			//motorArm->Set(op->GetLeftY()*.75);
//		}
//

		//puncher->Set(3000);
//		cout << puncher->GetPinStateQuadIdx() << endl;
//		cout << puncher->GetEncPosition() << endl;
//		cout << score->GetState() << endl;
		score->Update();
		//score->AngleHomeLoop();
		arm->Process();

		if(fabs(op->GetRightY())>.1)
		{
			arm->GoToAngles(arm->BaseAngle() + (op->GetRightY() * .5),arm->ArmAngle());
		}
		if(fabs(op->GetLeftY())>.1)
		{
			arm->GoToAngles(arm->BaseAngle(),arm->ArmAngle() + (op->GetLeftY() * .5));
		}

		if(op->GetBack())
		{
			arm->ResetTrain();
		}
		if(op->GetStart())
		{
			arm->Train();
		}
		if(op->GetDPad() == Gamepad::kDown)
		{
			arm->Home();
		}
		if(op->GetDPad() == Gamepad::kLeft)
		{
			arm->Set(0);
		}
		if(op->GetDPad() == Gamepad::kRight)
		{
			arm->Set(1);
		}
		if(op->GetDPad() == Gamepad::kUp)
		{
			arm->Set(2);
		}

		cout << aimLoop->Get() << endl;
		cout << "EncValue: " << aimer->GetEncPosition() << endl;
		cout << "Setpoint: " << aimLoop->GetSetpoint() << endl;
		cout << "AbsEncValue: " << absenc->GetVoltage()* 800.0F << "\n";
		cout << "AbsEncVolt: " << absenc->GetVoltage() << "\n";
		cout << "BaseEnc: " << motorBase->GetEncPosition() << endl;
		//cout << aimer->GetPinStateQuadIdx();
		Log();
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
		drive->SetupLogging(logger);
		score->SetupLogging(logger);
		arm->SetupLogging(logger);
		logger->AddAttribute("AbsEnc");
		logger->WriteAttributes();
	}

	void Log()
	{
		drive->Log(logger);

		logger->Log("Time", logTime->Get());
#if !TESTBED
		drive->Log(logger);
		score->Log(logger);
		arm->Log(logger);
		logger->Log("AbsEnc", absenc->GetVoltage());
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
		autoPanP = Config::GetSetting("autoAimP", 0.08);
		encoderTicksPerDegree = Config::GetSetting("encodeTicksPerDegree", 10.78);
		bottomHardLimit = Config::GetSetting("bottomHardLimit", 3635);
		DegreesToZero = Config::GetSetting("degreesToZero", 10);
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
