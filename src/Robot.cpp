#include "WPILib.h"
//#include <string>
#include <iostream>
#include "CameraServer.h"
#include <unistd.h>
#include <ctime>
#include "Logger.h"

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
    Logger *logger;
    Timer *logTime;
    const char *JAVA = "/usr/local/frc/JRE/bin/java";
    std::string profile = "everything";
    char* nope = '\0';
    //string *nope = NULL;
//    char * GRIP_ARGS[5] = {"java" , "-jar" , "/home/lvuser/GRIP.jar" ,
//    		"/home/lvuser/everything.grip",
//			nope};
    //std::string GRIP_ARGS = "java -jar /home/lvuser/GRIP.jar /home/lvuser/" + profile + ".grip";

    //std::shared_ptr<NetworkTable> grip = NetworkTable::GetTable("GRIP");

public:
	Robot()
	{
            //wpi_setErrnoErrorWithContext("Failed to run GRIP");
		motor1 = NULL;
		joy1=NULL;
		lw = NULL;
		chooser = NULL;
		cameraUSB = NULL;
		logger = NULL;
		logTime = NULL;
		gyro = new AnalogGyro(1);// = NULL;
	};

	void RobotInit()
	{

        /*if (fork() == 0) {
            if (execv(JAVA, GRIP_ARGS) == -1) {
                perror("Error running GRIP");
            }
        }*/
		//lw = LiveWindow::GetInstance();
		chooser = new SendableChooser();
		cameraUSB = CameraServer::GetInstance();
		cameraUSB->StartAutomaticCapture("cam0");
		cameraUSB->SetQuality(2);
		cameraUSB->StartAutomaticCapture();

		//gyro = new AnalogGyro(1);

		//chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		//chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);

		SmartDashboard::PutData("Auto Modes", chooser);
		//lw->AddSensor((const std::string)"Main", 0, gyro);
		joy1 = new Joystick(0);
		motor1 = new Victor(0);
		logTime = new Timer;
		logTime->Start();
		logger = new Logger();
		time_t t = time(0);
		struct tm *now = localtime(&t);
		//don't touch it
		std::string name = "/media/sda/log-" +
				std::to_string(now->tm_year + 1900) +
				"-\0" +
				std::to_string(now->tm_mon + 1) +
				"-\0" + std::to_string(now->tm_mday) + "_\0" +
				std::to_string(now->tm_hour) + ":\0" + std::to_string(now->tm_min) + ":\0" + std::to_string(now->tm_sec) + ".csv";
		cout << name << endl;
		logger->Open(name);
		SetupLogging();
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
		cout << "thing" << endl;
	}

	void TeleopPeriodic()
	{
		Log();
		//motor1->Set(joy1->GetX());
		//cout << gyro->GetAngle() << endl;
		//ewhfu
//		auto areas = grip->GetNumberArray("myContoursReport/area", llvm::ArrayRef<double>()),
//				xs    = grip->GetNumberArray("myContoursReport/x",    llvm::ArrayRef<double>()),
//				ys    = grip->GetNumberArray("myContoursReport/y",    llvm::ArrayRef<double>());
//
//		for (auto area : areas) {
//			std::cout << "Got contour with area=" << area << std::endl;
//		}
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	void SetupLogging()
	{
		logger->AddAttribute("Time");
		logger->WriteAttributes();
	}

	void Log()
	{
		logger->Log("Time", logTime->Get());
		cout << logTime->Get() << endl;
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
