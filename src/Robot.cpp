#include "WPILib.h"

class Robot: public IterativeRobot
{
	RobotDrive myRobot;
	Joystick opStick;
	Joystick drStick;
	Talon rightWheels;
	Talon leftWheels;
	Solenoid clawRight;
	Solenoid clawLeft;
	Solenoid shiftGear;
	Solenoid elevatorExtend;
	Solenoid elevatorRetract;
	bool TBA;
	bool clawOpen = true;
	bool clawIsPressed = false;

	LiveWindow *lw = LiveWindow::GetInstance();

public:
	Robot() :
		myRobot(0,1),
		opStick(TBA),
		drStick(TBA),
		rightWheels(TBA),
		leftWheels(TBA),
		clawRight(TBA),
		clawLeft(TBA),
		shiftGear(TBA),
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

	}

	void TestPeriodic()
	{
		lw->Run();
	}

#if Container_Claw
	void ToggleClaw() {
		if ((drStick.GetRawButton(4) == true) && (clawOpen == true) && (clawIsPressed == false))
				{
					clawRight.Set(false);
					clawLeft.Set(false);
					clawOpen = false;
					clawIsPressed = true;
				}

		else if ((drStick.GetRawButton(4) == true) && (clawOpen == false) && (clawIsPressed == false))
				{
					clawRight.Set(true);
					clawLeft.Set(true);
					clawOpen = true;
					clawIsPressed = true;
				}
		else if (drStick.GetRawButton(4) == false)
				{
					clawIsPressed = false;
				}
	}
#endif

	void CheesyDrive() {
		if (drStick.GetRawAxis(1) > 0 || drStick.GetRawAxis(1) < 0)
			{
				rightWheels.SetSpeed(drStick.GetRawAxis(1));
				leftWheels.SetSpeed(drStick.GetRawAxis(1));
			}
		else
			{
				rightWheels.SetSpeed(0.0);
				leftWheels.SetSpeed(0.0);
			}

		if (drStick.GetRawAxis(4) > 0)
			{
				rightWheels.SetSpeed(2*drStick.GetRawAxis(1));
				leftWheels.SetSpeed(.5*drStick.GetRawAxis(1)) ;
			}
		else if (drStick.GetRawAxis(4) < 0)
			{
				rightWheels.SetSpeed(0.5*drStick.GetRawAxis(1));
				leftWheels.SetSpeed(2*drStick.GetRawAxis(1));
			}
	}

	void GearShift() {
		if (drStick.GetRawButton(5) == true)
		{
			shiftGear.Set(true);
		}
		else
		{
			shiftGear.Set(false);
		}
	}

};

START_ROBOT_CLASS(Robot);

