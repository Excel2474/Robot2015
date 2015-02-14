#include "WPILib.h"
#include "Claw.h"
#include "Elevator.h"
#include "Rollers.h"

typedef enum
{
	ELEVATOR_ENCODER_A,
	ELEVATOR_ENCODER_B,
	RIGHT_ENCODER,
	LEFT_ENCODER,
	LOWER_LIMIT_SWITCH,
	UPPER_LIMIT_SWITCH
}DIGITAL_OUTPUT_CHANNEL;


typedef enum
{
	VICTOR_ROLLER_RIGHT,
	VICTOR_ROLLER_LEFT,
	ELEVATOR_VICTOR
//Add your drive-train motors to this enumeration, and have joeTalon(DRIVE_TRAIN_MOTOR)
}VICTOR_CHANNEL;

typedef enum
{
	AUTONOMOUS_ONE,
	AUTONOMOUS_TWO,
	AUTONOMOUS_THREE,
	AUTONOMOUS_FOUR
}AUTONOMOUS_OPTIONS;

AUTONOMOUS_OPTIONS autonOptions;

typedef enum
{
	SOLENOID_CLAW_EXTEND,
	SOLENOID_CLAW_RETRACT,
	SOLENOID_ELEVATOR_EXTEND,
	SOLENOID_ELEVATOR_RETRACT,
	SOLENOID_ROLLERS_EXTEND,
	SOLENOID_ROLLERS_RETRACT,
	SOLENOID_ELEVATOR_BRAKE
}SOLENOID_CHANNEL;



class Robot: public IterativeRobot
{

	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
//	SmartDashboard driverDashboard;

	LiveWindow *lw;
	Victor joeTalon;
	Victor kaylaTalon;
	Victor loretta;
	Victor rhonda;
	Victor thing1;
	Victor thing2;
//	Solenoid numanumamaticExtend;
//	Solenoid numanumamaticRetract;
	Solenoid shiftUpExtend;
	Solenoid shiftUpRetract;
	Encoder rightEncoder;
	Encoder leftEncoder;
	DigitalInput clicker;
	float lastCurve = 0;
	bool curvyWurvy = false;
	bool extended = false;
	bool clawOpen = false;
	bool elevatorExtended = false;
	bool dropRoutineStarted = false;
	bool dropRoutineFinished = true;
	int autoLoopCounter = 0;
	int elevatorLevel = 1;
	int encoderCountNow = 0;
	int encoderCountPrev = 0;
	bool yReleased;
	bool rollersOpen = true;
	bool numanumamaticIsPressed = false;
	bool shiftUp = false;
	float rollerSpeed = 0;
	bool override;
	bool goingToLevel = false;
	Timer brakeTime;
	Claw claws;
	Elevator elevator;
	Compressor compressor;
	Rollers rollers;

public:
	Robot() :
		myRobot(loretta, rhonda, joeTalon, kaylaTalon),	// these must be initialized in the same order
		stick(0),// as they are declared above.
		lw(NULL),
		joeTalon(7),
		kaylaTalon(6),
		loretta(9),
		rhonda(8),
		thing1(5),
		thing2(4),
//		numanumamaticExtend(0),
//		numanumamaticRetract(1),
		shiftUpExtend(3),
		shiftUpRetract(5),
		rightEncoder(0, 1, true),
		leftEncoder(2, 3, true),
		clicker(0),
		claws(SOLENOID_CLAW_EXTEND, SOLENOID_CLAW_RETRACT),
		elevator(SOLENOID_ELEVATOR_EXTEND, SOLENOID_ELEVATOR_RETRACT, SOLENOID_ELEVATOR_BRAKE, ELEVATOR_VICTOR, ELEVATOR_ENCODER_A, ELEVATOR_ENCODER_B, LOWER_LIMIT_SWITCH, UPPER_LIMIT_SWITCH),
		compressor(5),
		rollers(SOLENOID_ROLLERS_EXTEND, SOLENOID_ROLLERS_RETRACT, VICTOR_ROLLER_RIGHT, VICTOR_ROLLER_LEFT, rollerSpeed)
		//autoLoopCounter(0),
		//lastCurve(0)
	{
		myRobot.SetExpiration(0.1);
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		goingToLevel = false;
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
		compressor.Start();
		rightEncoder.Reset();
		leftEncoder.Reset();
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
		compressor.Start();
	}

	void TeleopDisabled()
	{
		//driverDashboard.PutBoolean("Extended", extended);
	}

	void TeleopPeriodic()
	{
		//double rightEncoderRate = rightEncoder.GetRate();
		//double rightRPM = (rightEncoderRate/256) * 60;
//		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)

		myRobot.ArcadeDrive(stick.GetRawAxis(1), 0.7 * stick.GetRawAxis(4)); //0.7 dampens the steering sensitivity, modify to taste
//		//Drive don't do this, use arcade drive
//		if ((stick.GetRawAxis(1) < 0.1) && (stick.GetRawAxis(1) > -0.1))
//		{
//			joeTalon.SetSpeed(stick.GetRawAxis(4));
//			kaylaTalon.SetSpeed(-stick.GetRawAxis(4));
//		}
//		else
//		{
//		}
//
//
//		if ((stick.GetRawAxis(4) - lastCurve) < -0.1 || (stick.GetRawAxis(4) - lastCurve) > 0.1)
//		{
//			lastCurve = stick.GetRawAxis(4);
//		}

//		//Gear Box Motors Test
//		if(stick.GetRawButton(3) == true)
//		{
//			myRobot.Drive(.1, 0);
//		}

		//Open Claw
		if (stick.GetPOV(0) == 0 && clawOpen == false && override == false)
		{
			clawOpen = true;
			claws.OpenClaw();
		}

		//Close Claw
		if (stick.GetPOV(0) == 180 && clawOpen == true && override == false && elevatorExtended == false)
		{
			clawOpen = false;
			claws.CloseClaw();
		}

		//Extend Elevator
		if (stick.GetPOV(0) == 90 && elevatorExtended == false)
		{
			elevatorExtended = true;
			clawOpen = true;
			claws.OpenClaw();
			elevator.ExtendElevator();

		}

		//Retract Elevator
		if (stick.GetPOV(0) == 270 && elevatorExtended == true)
		{
			elevatorExtended = false;
			elevator.RetractElevator();
		}

		//Drop Routine Start
		if (stick.GetRawButton(3) == true && dropRoutineStarted == false && dropRoutineFinished == true)
		{
			dropRoutineStarted = true;
			dropRoutineFinished = false;
			//Call drop routine
		}

		//Lower Elevator Level
		if (stick.GetRawButton(1) == true && elevatorLevel != 0)
		{
			elevatorLevel = elevatorLevel - 1;
			elevator.SetLevel(elevatorLevel);
			//Call lower elevator function
		}

		//Raise Elevator Level
		if (stick.GetRawButton(2) == true && elevatorLevel != 6)
		{
			goingToLevel = true;
		}

		if (goingToLevel == true)
		{
			elevator.BrakeOff();
					brakeTime.Reset();
					brakeTime.Start();
					if (brakeTime.Get() > 0.1)
					{
						elevatorLevel = elevatorLevel + 1;
						elevator.SetLevel(elevatorLevel);
						if (elevator.IsAtLevel() == true)
						{
							elevator.BrakeOn();
							goingToLevel = false;
						}
					}
		}
			//Call raise elevator function

		//TEST CODE - Control elevator motor speed
		if (stick.GetRawAxis(1) != 0) // I don't know if Axis 1 is being used yet, or what it is. We should check and change this
		{
			elevator.TestElevatorMotor(stick.GetRawAxis(1));
		}

		// Open Claw and rollers
		if (stick.GetRawButton(4) == true && yReleased == true)
		{
			override = true;
//			if (stick.GetRawButton(5) == true && rollersOpen == false)
//			{
//				rollersOpen = true;
//				//override left bumper
//			}

			// call ignore our POV 4 when called

			clawOpen = true;
			rollersOpen = false;
			rollers.OpenRollers();
			claws.OpenClaw();
			// Call Open claw and rollers wide
		}
		else
		{
			override = false;
		}

		//Close Rollers
		if (stick.GetRawButton(5) == true && rollersOpen == true && override == false)
		{
			rollersOpen = false;
			rollers.OpenRollers();
			//Call close rollers function
		}

		//Open Rollers
		else if (stick.GetRawButton(5) == false && rollersOpen == false && override == false)
		{
			rollersOpen = true;
			rollers.CloseRollers();
			//Call open rollers function
		}

		//Shift Up Gear
		if (stick.GetRawButton(6) == true && shiftUp == false)
		{
			shiftUp = true;
			shiftUpExtend.Set(true);
			shiftUpRetract.Set(false);
		}

		//Shift Down Gear
		else if (stick.GetRawButton(6) == false && shiftUp == true)
		{
			shiftUp = false;
			shiftUpExtend.Set(false);
			shiftUpRetract.Set(true);
		}

		//Spin Rollers In
		if (stick.GetRawAxis(2) >= 0.05)
		{
			rollerSpeed = (stick.GetRawAxis(2));
			rollers.Eat();
//			thing1.SetSpeed(stick.GetRawAxis(2));
//			thing2.SetSpeed(-stick.GetRawAxis(2));
		}

		//Spin Rollers Out
		else if (stick.GetRawAxis(3) >= 0.05)
		{
			rollerSpeed = (stick.GetRawAxis(3));
			rollers.Barf();
//			thing1.SetSpeed(-stick.GetRawAxis(2));
//			thing2.SetSpeed(stick.GetRawAxis(3));
		}

		//Stop Rollers
		else
		{
//			thing2.SetSpeed(0.0);
//			thing1.SetSpeed(0.0);
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

	void AutonOne(void) //Drives forward.
	{
		if (autoLoopCounter <= 120)
		{
			myRobot.Drive(1, 0);
			autoLoopCounter++;
		}
		else
		{
			myRobot.Drive(0, 0);
		}
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
