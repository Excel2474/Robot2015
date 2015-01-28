#include "WPILib.h"

class Robot: public IterativeRobot
{
	RobotDrive myRobot;
	Joystick opStick;
	Joystick drStick;
	Talon rightWheels;
	Talon leftWheels;
	Solenoid clawOpen;
	Solenoid clawClose;
	Solenoid shiftUp;
	Solenoid shiftDown;
	Solenoid elevatorExtend;
	Solenoid elevatorRetract;
	bool TBA;

	LiveWindow *lw;

public:
	Robot() :
		myRobot(0,1),
		opStick(TBA),
		drStick(TBA),
		rightWheels(TBA),
		leftWheels(TBA),
		lw(NULL)
{
		myRobot.SetExpiration(0.1);
}

private:


	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		myRobot.ArcadeDrive(drStick);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);

