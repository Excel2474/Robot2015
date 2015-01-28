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
	bool clawExtended;
	bool clawIsPressed;

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

#if Container Claw

		if ((drStick.GetRawButton(4) == true) && (clawExtended == false) && (clawIsPressed == false))
				{
					clawOpen.Set(true);
					clawClose.Set(false);
					clawExtended = true;
					clawIsPressed = true;
				}

		else if ((drStick.GetRawButton(4) == true) && (clawExtended == true) && (clawIsPressed == false))
				{
					clawOpen.Set(false);
					clawClose.Set(true);
					clawExtended = false;
					clawIsPressed = true;
				}
		else if (drStick.GetRawButton(4) == false)
				{
					clawIsPressed = false;
				}
#endif
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);

