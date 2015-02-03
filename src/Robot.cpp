#include "WPILib.h"

typedef enum
{
	AUTONOMOUS_ONE,
	AUTONOMOUS_TWO,
	AUTONOMOUS_THREE,
	AUTONOMOUS_FOUR
}AUTONOMOUS_OPTIONS;

AUTONOMOUS_OPTIONS autonOptions;


class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	SmartDashboard driverDashboard;

	LiveWindow *lw;
	Talon joeTalon;
	Talon kaylaTalon;
	Talon loretta;
	Talon rhonda;
	Solenoid numanumamaticExtend;
	Solenoid numanumamaticRetract;
	Encoder rightEncoder;
	//DigitalInput clicker = new DigitalInput(0);
	int autoLoopCounter;
	float lastCurve;
	bool curvyWurvy = false;
	bool extended = false;
	bool numanumamaticIsPressed = false;

public:
	Robot() :
		myRobot(joeTalon, kaylaTalon),	// these must be initialized in the same order
		stick(0),// as they are declared above.
		lw(NULL),
		joeTalon(7),
		kaylaTalon(6),
		loretta(9),
		rhonda(8),
		numanumamaticExtend(0),
		numanumamaticRetract(1),
		rightEncoder(0, 1, true),
		//clicker(0),
		autoLoopCounter(0)
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
		autoLoopCounter = 0;
	}

	void AutonomousPeriodic()
	{
		switch (autonOptions)
		{
		case AUTONOMOUS_ONE:
			AutonOne();
			break;
		case AUTONOMOUS_TWO:
			AutonTwo();
			break;
		case AUTONOMOUS_THREE:
			AutonThree();
			break;
		case AUTONOMOUS_FOUR:
			AutonFour();
			break;
		}



//		if(autoLoopCounter < 200) //Check if we've completed 100 loops (approximately 2 seconds)
//		{
//			if(autoLoopCounter < 50)
//			{
//				myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
//			}
//			else if(autoLoopCounter >= 50 && autoLoopCounter <= 100)
//			{
//				myRobot.Drive(0.25, -0.5);
//				if(autoLoopCounter == 75)
//				{
//					numanumamaticExtend.Set(true);
//				}
//			}
//			else
//			{
//				myRobot.Drive(0.5, 0.0);
//			}
//
//
//			autoLoopCounter++;
//		}
//		else
//		{
//			myRobot.Drive(0.0, 0.0); 	// stop robot
//		}
	}

	void TeleopInit()
	{
		lastCurve = 1;
	}

	void TeleopDisabled()
	{
		driverDashboard.PutBoolean("Extended", extended);
	}

	void TeleopPeriodic()
	{
		double rightEncoderRate = rightEncoder.GetRate();
		double rightRPM = (rightEncoderRate/256) * 60;
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)

		//Drive
		if ((stick.GetRawAxis(1) < 0.1) && (stick.GetRawAxis(1) > -0.1))
		{
			joeTalon.SetSpeed(stick.GetRawAxis(4));
			kaylaTalon.SetSpeed(-stick.GetRawAxis(4));
		}
		else
		{
			myRobot.Drive(stick.GetRawAxis(1), lastCurve);
		}


		if ((stick.GetRawAxis(4) - lastCurve) < -0.1 || (stick.GetRawAxis(4) - lastCurve) > 0.1)
		{
			lastCurve = stick.GetRawAxis(4);
		}

		//Solenoid Test
		if ((stick.GetRawButton(4) == true) && (extended == false) && (numanumamaticIsPressed == false))
		{
			numanumamaticExtend.Set(true);
			numanumamaticRetract.Set(false);
			extended = true;
			numanumamaticIsPressed = true;
		}
		else if ((stick.GetRawButton(4) == true) && (extended == true) && (numanumamaticIsPressed == false))
		{
			numanumamaticExtend.Set(false);
			numanumamaticRetract.Set(true);
			extended = false;
			numanumamaticIsPressed = true;
		}
		else if (stick.GetRawButton(4) == false)
		{
			numanumamaticIsPressed = false;
		}

		if ()



		//if (clicker.Get() == true)
		//	{
		//		joeTalon.SetSpeed(1.0);
		//	}
		//else
		//	{
		//		joeTalon.SetSpeed(0.0);
		//	}
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	void AutonOne(void)
	{

	}

	void AutonTwo(void)
	{

	}

	void AutonThree(void)
	{

	}

	void AutonFour(void)
	{

	}
};

START_ROBOT_CLASS(Robot);
