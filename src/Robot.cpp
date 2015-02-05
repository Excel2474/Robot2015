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
//	SmartDashboard driverDashboard;

	LiveWindow *lw;
	Talon joeTalon;
	Talon kaylaTalon;
	Talon loretta;
	Talon rhonda;
	Solenoid numanumamaticExtend;
	Solenoid numanumamaticRetract;
	//Encoder rightEncoder;
	DigitalInput clicker;
	int autoLoopCounter = 0;
	float lastCurve = 0;
	bool curvyWurvy = false;
	bool extended = false;
	bool clawOpen = false;
	bool elevatorExtended = false;
	bool dropRoutineStarted = false;
	bool dropRoutineFinished = true;
	int elevatorLevel = 1;
	bool yReleased;
	bool rollersOpen = true;
	bool numanumamaticIsPressed = false;

public:
	Robot() :
		myRobot(loretta, rhonda, joeTalon, kaylaTalon),	// these must be initialized in the same order
		stick(0),// as they are declared above.
		lw(NULL),
		joeTalon(7),
		kaylaTalon(6),
		loretta(9),
		rhonda(8),
		numanumamaticExtend(0),
		numanumamaticRetract(1),
		//rightEncoder(0, 1, true)
		clicker(0)
		//autoLoopCounter(0),
		//lastCurve(0)
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
		//driverDashboard.PutBoolean("Extended", extended);
	}

	void TeleopPeriodic()
	{
		//double rightEncoderRate = rightEncoder.GetRate();
		//double rightRPM = (rightEncoderRate/256) * 60;
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

		//Gear Box Motors
		if(stick.GetRawButton(3) == true)
		{
			myRobot.Drive(.1, 0);
		}

		//Open Claw
		if (stick.GetPOV(0) == true && clawOpen == false)
		{
			clawOpen = true;
		}

		//Close Claw
		if (stick.GetPOV(4) == true && clawOpen == true)
		{
			clawOpen = false;
		}

		//Extend Elevator
		if (stick.GetPOV(2) == true && elevatorExtended == false)
		{
			elevatorExtended = true;
		}

		//Retract Elevator
		if (stick.GetPOV(6) == true && elevatorExtended == true)
		{
			elevatorExtended = false;
		}

		//Drop Routine Start
		if (stick.GetRawButton(3) == true && dropRoutineStarted == false && dropRoutineFinished == true)
		{
			dropRoutineStarted = true;
			dropRoutineFinished = false;
			//Call drop routine
		}

		//Lower Elevator
		if (stick.GetRawButton(1) == true && elevatorLevel != 0)
		{
			elevatorLevel = elevatorLevel - 1;
			//Call lower elevator function
		}

		//Raise Elevator
		if (stick.GetRawButton(2) == true && elevatorLevel != 6)
		{
			elevatorLevel = elevatorLevel + 1;
			//Call raise elevator function
		}

		// Open Claw and rollers
		if (stick.GetRawButton(4) && yReleased == true)
		{
			if (stick.GetRawButton(5) == true && rollersOpen == false)
			{
				rollersOpen = true;
				//override left bumper
			}

			// call ignore our POV 4 when called

			clawOpen = true;
			// Call Open claw and rollers wide
		}

		//Solenoid Test
//		if ((stick.GetRawButton(4) == true) && (extended == false) && (numanumamaticIsPressed == false))
//		{
//			numanumamaticExtend.Set(true);
//			numanumamaticRetract.Set(false);
//			extended = true;
//			numanumamaticIsPressed = true;
//		}
//		else if ((stick.GetRawButton(4) == true) && (extended == true) && (numanumamaticIsPressed == false))
//		{
//			numanumamaticExtend.Set(false);
//			numanumamaticRetract.Set(true);
//			extended = false;
//			numanumamaticIsPressed = true;
//		}
//		else if (stick.GetRawButton(4) == false)
//		{
//			numanumamaticIsPressed = false;
//		}


// Claw Code
//		if ((clicker.Get() == true) && (extended == false) && (numanumamaticIsPressed == false))
//			{
//			numanumamaticExtend.Set(true);
//			numanumamaticRetract.Set(false);
//			extended = true;
//			numanumamaticIsPressed = true;
//			}
//		else if ((clicker.Get() == true) && (extended == true) && (numanumamaticIsPressed == false))
//			{
//			numanumamaticExtend.Set(false);
//			numanumamaticRetract.Set(true);
//			extended = false;
//			numanumamaticIsPressed = true;
//
//			}
//		else if (clicker.Get() == false)
//			{
//				numanumamaticIsPressed = false;
//			}
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
