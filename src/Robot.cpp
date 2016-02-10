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
    CANTalon * aimer;
    Victor * lin;
    Victor * rin;
    //////////////////////////
    Scoring * score;


    std::string profile = "everything";
    char * nope = '\0';

    uint32_t jim = false;


    bool getPos = false;
    //string *nope = NULL;

    //std::string GRIP_ARGS = "java -jar /home/lvuser/GRIP.jar /home/lvuser/" + profile + ".grip";

public:
	Robot()
	{
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

		light = new Relay(0);

		// turn this on once we have a camera connected
		// targeting = new Targeting(light);

		driver = new Gamepad(0);
		op = new Gamepad(1);
		//gyro = new AnalogGyro(1);

		//chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		//chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);

		//SmartDashboard::PutData("Auto Modes", chooser);
		//lw->AddSensor((const std::string)"Main", 0, gyro);
//		cout << "Arm initializing\n";
//		motorBase = new CANTalon(1);
//		motorBase->SetControlMode(CANSpeedController::kPosition);
//		motorBase->SetPID(5,.0001,0);
//		motorArm = new CANTalon(3);
//		motorArm->SetControlMode(CANSpeedController::kPosition);
//		motorArm->SetPID(1,0,0);
//		cout << "Arm set to pos: " << motorArm->GetEncPosition();
//		arm = new Manipulation(motorBase, motorArm, NULL, NULL);

#if !TESTBED
		motorFL = new CANTalon(7);
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

		//Scoring THINGS//////////////////////////////////////////////////////////////////////////
		//motor->EnableZeroSensorPositionOnIndex(true, true);
		//motor->SetAllowableClosedLoopErr(20);//This is the error for the scoring encoder in PID
		//////////////////////////////////////////////////////////////////////////////////////////

		puncher = new CANTalon(4);
		puncher->SetControlMode(CANTalon::kPercentVbus);
		puncher->Enable();
		aimer = new CANTalon(3);
		aimer->SetControlMode(CANTalon::kPercentVbus);
		rin = new Victor(0);
		lin = new Victor(1);

		score = new Scoring(aimer,puncher,lin,rin,NULL);

		puncher->SetControlMode(CANTalon::kPercentVbus);
		aimer->SetControlMode(CANTalon::kPercentVbus);

		//scoring->SetControlMode();
		ReloadConfig();
		//StartLogging("init");
		OutputTroll(cout);
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
#if !TESTBED
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

		if(op->GetA())
		{
			getPos = true;
		}
		else
		{
			getPos = false;
		}

		if(fabs(op->GetRTriggerAxis()) >= .1)
		{
			score->SetFlySpeed(op->GetRTriggerAxis());
		}
		else if(fabs(op->GetLTriggerAxis()) >= .1)
		{
			score->SetFlySpeed(-(op->GetLTriggerAxis()));
		}
		else
		{
			score->SetFlySpeed(0);
		}

		if(fabs(op->GetRightX()) >= 0.1)//fabs(op->GetRightX()) >= 0.1)
		{
			puncher->Set(op->GetRightX() *.5);//op->GetRightX() *
		}
		else
		{
			puncher->Set(0);
		}

		if(fabs(op->GetLeftX()) >= 0.1)
		{
			aimer->Set(op->GetLeftX() * .5);
		}
		else
		{
			aimer->Set(0);
		}
#endif
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

	void OutputTroll(ostream & out)
	{
		out
		<< "                                      ....... .\n"
		"                      ,7IB@MMMMMMMMMMMMMMMMMMM@@@MMMMMMMMMM,               \n"
		"                ,I#MMMMMMM$ZI7Yii,,.,....     ..         :#MM              \n"
		"             YMMMWoci,.      ..,:iii,,.........,,:ii.       MMo            \n"
		"           EMM9       ,,:,...:;;ci,    ...::;;YYY;;icY;      2M#           \n"
		"          MM;     .;:,.,,ii;;         Ui     ...,vSUv;iY;.    AMz          \n"
		"         ,M      SI        Yi        Ai            .v;;,..     @M          \n"
		"         MM      .          S            ,C6SotXi.     .        MM         \n"
		"        MM.        ioAU1;   ,         YMMMbiMMMMMMM@i            MM;       \n"
		"      2MM  ,..,; :MMMMMMMMM@.       .MM,   ;MMMMMM#@MM iSY    .i,iMMMv     \n"
		"     MMv.YI.      ,iYt68B@MMMMMb    cMMt@MM$Y.C.  iobC   .,CZEb;   .bMM.   \n"
		"    MM.,:Y..YoY,            Mo        S$:     CM#.     nMMM@1zbMMQ    nMt  \n"
		"    M@, , vMM#E@MMX.@2      M.                 .$MMMMMMMZ.  @    MM  . 7M, \n"
		"    MMi Y     I iMMMM,    MMM         vi                 .UMM$    MM i  M@ \n"
		"    SM9  C   :M.       iMMb          :@MMMvi:.,.      9MMMZ oMMMn MM ,  MM \n"
		"     MMC.7,  MMC    .cCMMMz      YMMMM, #M      .7$MMMMo    MM i;tM. 7 ;M7 \n"
		"      MM .Y MM@M$.        MM; A. .;. . .M   7WMMMM$,M$    7MMY   X, C iM#  \n"
		"       M@   MMMCMMM@i      .#MW      .vE$MMM#A:     MM1MMMMM7       ,MMo   \n"
		"       zM   MME M1 SMMMMMM@o8@$@MMMMMM#Ii.Mt     ;MMMMM: MM        0MM     \n"
		"        MY .MM6 Mi  MC    cM8    M@       ZMi$MMMMMQM;  MM        MM,      \n"
		"        Mz  MMMMMM6EMz.   ,M.    M#  .;z@MMMMMM#7   MoYMM        MM        \n"
		"        Mz  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMCM@       tMMS        oM        \n"
		"        M;  EMAMMMMMMMMMMMMMMMMMMMMMZ7i    ZM     8MM8         XMC     \n"
		"       .M.   Mb9M MM :M@ .YMM     .M        @M ;MMMz          MM7   \n"
		"       SM    ,MMM  M7 ,M    M     .M       ,$MMMb       i.  @MM  \n"
		"       MM      6MMMMM$nMMc. MM.:::IMMW#MMMMMW,   .:;c1SYioMMM.\n"
		"       M@          .c6WE#MMMMMMMMMMQ$Q8C:     iYCYvi,,X#MM9\n"
		"       M2  S.  i8,                       .;2Uo;,..,2@MMZ\n"
		"       Mc  .X,   v1Xv:.           ,,vXoUASv.   v$MM@Y\n"
		"       M;    7Iz;............,.,,:::..     .8MMM2.\n"
		"       MM       .,:::........           ;@MM$:\n"
		"        MMz                     .z@MMMMMMb.\n"
		"         iMMME7.       .,i1Z#MMMMMMES;.\n"
		"            C@MMMMMMMMMMMM@Wn;.\n"
		"\n";
	}
};

START_ROBOT_CLASS(Robot)
