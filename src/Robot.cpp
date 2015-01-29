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

	LiveWindow *lw = LiveWindow::GetInstance();

public:
	Robot() :
		myRobot(0,1),
		opStick(TBA),
		drStick(TBA),
		rightWheels(TBA),
		leftWheels(TBA),
		clawOpen(TBA),
		clawClose(TBA),
		shiftUp(TBA),
		shiftDown(TBA),
		elevatorExtend(TBA),
		elevatorRetract(TBA),
		lw(NULL)
{
//		//Potential SmartDashBoard component inclusion code- START
//		Solenoid *elevatorExtend = elevatorExtend;
//		lw->AddActuator("Solenoid", 2, elevatorExtend);
//		//SmartDashboard thing END

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

#if Container_Claw

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

