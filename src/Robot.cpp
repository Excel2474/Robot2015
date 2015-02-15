#include "WPILib.h"
#include "Claw.h"
#include "Elevator.h"
#include "Rollers.h"

#define DRIVEWHEEL_CIRCUMFERENCE 18.8495559215 //In inches, given 6-wheels
//Robot drives 18.8495559215 inches/rotation
#define DRIVEWHEEL_INCHES_PER_PULSE 0.07363107781
// (circumference/pulses per rotation)
#define DRIVEWHEEL_PULSES_PER_INCH 13.5812218105
// (256 pulses / circumference)

typedef enum
{
	ELEVATOR_ENCODER_A,
	ELEVATOR_ENCODER_B,
	RIGHT_ENCODER,
	LEFT_ENCODER,
	LOWER_LEFT_LIMIT_SWITCH,
	LOWER_RIGHT_LIMIT_SWITCH,
	UPPER_LEFT_LIMIT_SWITCH,
	UPPER_RIGHT_LIMIT_SWITCH
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
	//SmartDashboard driverDashboard;

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
	bool droveStraight = true;
	bool turnt = true;
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
	bool goingUp = false;
	bool goingDown = false;
	Timer brakeTime;
	Timer autonTimer;
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
		elevator(SOLENOID_ELEVATOR_EXTEND, SOLENOID_ELEVATOR_RETRACT, SOLENOID_ELEVATOR_BRAKE, ELEVATOR_VICTOR, ELEVATOR_ENCODER_A, ELEVATOR_ENCODER_B, LOWER_LEFT_LIMIT_SWITCH, LOWER_RIGHT_LIMIT_SWITCH, UPPER_LEFT_LIMIT_SWITCH, UPPER_RIGHT_LIMIT_SWITCH),
		compressor(5),
		rollers(SOLENOID_ROLLERS_EXTEND, SOLENOID_ROLLERS_RETRACT, VICTOR_ROLLER_RIGHT, VICTOR_ROLLER_LEFT, rollerSpeed)
		//autoLoopCounter(0),
		//lastCurve(0)
	{
		myRobot.SetExpiration(0.1);
		SmartDashboard::init();
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		goingUp = false;
		goingDown = false;
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
		autonTimer.Reset();
		autonTimer.Start();
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
		if (stick.GetPOV(0) == 180 && clawOpen == true && override == false)
		{
			clawOpen = false;
			claws.CloseClaw();
		}

		//Extend Elevator
		if (stick.GetPOV(0) == 90 && elevatorExtended == false)
		{
			elevatorExtended = true;
			rollersOpen = true;
			rollers.OpenRollers();
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
			goingDown = true;
		}
		if (goingDown == true && goingUp == false) //This logic here with the && needs to be checked. -Ben
		{
			elevator.BrakeOff();
			brakeTime.Reset();
			brakeTime.Start();
			if (brakeTime.Get() > 0.1)
			{
				elevatorLevel = elevatorLevel - 1;
				elevator.SetLevel(elevatorLevel);
				//^Call LOWER elevator function
				if (elevator.IsAtLevel() == true)
				{
					elevator.BrakeOn();
					goingDown = false;
					SmartDashboard::PutNumber("Stinky", elevatorLevel);
				}
			}
		}

		//Raise Elevator Level
		if (stick.GetRawButton(2) == true && elevatorLevel != 6)
		{
			goingUp = true;
		}
		if (goingUp == true && goingDown == false) //This logic right here with the && needs to be checked. -Ben
		{
			elevator.BrakeOff();
			brakeTime.Reset();
			brakeTime.Start();
			if (brakeTime.Get() > 0.1)
			{
				elevatorLevel = elevatorLevel + 1;
				elevator.SetLevel(elevatorLevel);
				//^Call RAISE elevator function
				if (elevator.IsAtLevel() == true)
				{
					elevator.BrakeOn();
					goingUp = false;
				}
			}
		}

		//TEST CODE - Control elevator motor speed
		if (stick.GetRawAxis(0) < -0.1 || stick.GetRawAxis(0) > 0.1)
		{
			elevator.TestElevatorMotor(stick.GetRawAxis(0));
		}
		else
		{
			elevator.TestElevatorMotor(0);
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
		if (stick.GetRawButton(5) == true && rollersOpen == true && override == false && elevatorLevel != 0 && elevatorExtended)
		{
			rollersOpen = false;
			rollers.CloseRollers();
			//Call close rollers function
		}

		//Open Rollers
		else if (stick.GetRawButton(5) == false && rollersOpen == false && override == false)
		{
			rollersOpen = true;
			rollers.OpenRollers();
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

		SmartDashboard::PutString("DB/String0", "Pooping");

	}

	void TestPeriodic()
	{
		lw->Run();
	}


	//Autonomous functions zone start

	void Auto_DriveStraightDistance(float distance, float speed) //distance in inches
	{
		rightEncoder.Reset();
		leftEncoder.Reset();
//		if(droveStraight == true)
//		{
//			rightEncoder.Reset();
//			leftEncoder.Reset();
//			droveStraight = false;
//		}
		if (rightEncoder.Get() < (distance * DRIVEWHEEL_PULSES_PER_INCH) && leftEncoder.Get() < (distance * DRIVEWHEEL_PULSES_PER_INCH)) //Might need a buffer here
		{
			myRobot.Drive(speed, 0.0);
		}
		else
		{
			myRobot.Drive(0.0, 0.0);
			droveStraight = true;
		}
	}
	void Auto_ZeroPointTurn(float degrees, float speed)
	{
		//TODO: figure out rotations/degree for the drive wheels when both sides are moving opposite directions (i.e. stationary turning)

		/**
		if (turnt == true)
		{
			rightEncoder.Reset();
			leftEncoder.Reset();
			turnt = false;
		}
		if (encoder angle check stuff)
		{
			myRobot.ArcadeDrive(0, angle conversion stuff);
		}
		else
		{
			myRobot.ArcadeDrive(0.0, 0.0);
			turnt = true;
		}
		*/
	}

	//Autonomous functions zone end

	void AutonOne(void) //Drives forward.
	{
		if (autonTimer.Get() < 10) //10 seconds is more than enough. We just don't yet know how long it will take for the robot to drive however far forward. This is how we're going to sequence autonomous stuff: giving functions set completion times. -Ben
		{
			Auto_DriveStraightDistance(12, 0.5); //Forward one foot, for now
		}
	}

	void AutonTwo(void) //Grasps a yellow tote, turns, drives into the Auto Zone, and goes and sets it on the Landmark
	{
		//For now, we'll code this in regards to starting at the middle yellow tote


		if (autonTimer.Get() <= 2) //GUESSED TIME
		{
			claws.CloseClaw();
			elevator.SetLevel(1);
			if (elevator.IsAtLevel() == true)
			{
				elevator.BrakeOn();
			}
		}
		if (autonTimer.Get() > 2 && autonTimer.Get() <= 3) //GUESSED TIMES
		{
			Auto_DriveStraightDistance(3, -1.0);
		}
		//^^^vvv We could probably combine these two movements: moving back and turning towards the auto zone. -Ben
		if (autonTimer.Get() > 3 && autonTimer.Get() <= 5) //GUESSED TIMES
		{
			Auto_ZeroPointTurn(90, 1.0);
		}
		if (autonTimer.Get() > 5 && autonTimer.Get() <= 9) //GUESSED TIMES
		{
			Auto_DriveStraightDistance(107, 1.0);
		}
		if (autonTimer.Get() > 9 && autonTimer.Get() <= 11) //GUESSED TIMES
		{
			Auto_ZeroPointTurn(-90, 1.0);
		}
		//^^^vvv Same here. We could combine these.
		if (autonTimer.Get() > 11 && autonTimer.Get() <= 12) //GUESSED TIMES
		{
			Auto_DriveStraightDistance(24, 1.0);
		}
		if (autonTimer.Get() > 12 && autonTimer.Get() <= 14) //GUESSED TIMES
		{
			elevator.BrakeOff();
			elevator.SetLevel(0);
			if (elevator.IsAtLevel() == true)
			{
				elevator.BrakeOn();
				claws.OpenClaw();
			}
		}
		if (autonTimer.Get() > 14)
		{
			Auto_DriveStraightDistance(36, -1.0);
		}
	}

	void AutonThree(void)
	{

	}

	void AutonFour(void)
	{

	}
};

START_ROBOT_CLASS(Robot);
