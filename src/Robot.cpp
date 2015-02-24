#include "WPILib.h"
#include "Claw.h"
#include "Elevator.h"
#include "Rollers.h"
#include "Donuts.h"

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
	RIGHT_ENCODER_A,
	RIGHT_ENCODER_B,
	LEFT_ENCODER_A,
	LEFT_ENCODER_B,
	GYRO,
	LOWER_LEFT_LIMIT_SWITCH,
	LOWER_RIGHT_LIMIT_SWITCH,
	UPPER_LEFT_LIMIT_SWITCH,
	UPPER_RIGHT_LIMIT_SWITCH
}DIGITAL_OUTPUT_CHANNEL;

typedef enum
{
	POINTLESS_DUMB_UNOCCUPIED_MOTOR, //0
	LEFT_ROLLER_MOTOR, //1
	LEFT_DRIVE_MOTOR, //2
	ELEVATOR_MOTOR, //3
	RIGHT_ROLLER_MOTOR, //4
	RIGHT_DRIVE_MOTOR //5
//Add your drive-train motors to this enumeration, and have joeTalon(DRIVE_TRAIN_MOTOR)
}VICTOR_CHANNEL;

typedef enum
{
	AUTONOMOUS_JUST_DRIVE,
	AUTONOMOUS_SINGLE_TOTE,
	AUTONOMOUS_THREE_TOTE_STACK,
	AUTONOMOUS_TWO_CONTAINERS,
	AUTONOMOUS_FIVE
}AUTONOMOUS_OPTIONS;

AUTONOMOUS_OPTIONS autonOptions;

typedef enum
{
	SOLENOID_DRIVE_SHIFT_EXTEND, //Gear shift up
	SOLENOID_DRIVE_SHIFT_RETRACT, //Gear shift down
	SOLENOID_ELEVATOR_EXTEND,
	SOLENOID_ELEVATOR_RETRACT,
	SOLENOID_ROLLERS_EXTEND,
	SOLENOID_ROLLERS_RETRACT,
	SOLENOID_ELEVATOR_BRAKE_EXTEND, //Release brake
	SOLENOID_ELEVATOR_BRAKE_RETRACT //Engage brake

}SOLENOID_CHANNEL;

typedef enum
{
	SOLENOID_CLAW_EXTEND, //These two are sketchy, what with being on the second PCM. -Ben 2/17/15
	SOLENOID_CLAW_RETRACT
}CLAW_SOLENOID_CHANNEL;


class Robot: public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	//SmartDashboard driverDashboard;

	LiveWindow *lw;
	SendableChooser *mendableBruiser;
	Victor joeTalon;
	Victor kaylaTalon;
//	Victor loretta;  Rest in peace Loretta, you will be missed.
//	Victor rhonda; Rest in peace Rhonda, you will be missed more. Don't tell Loretta.
//	Victor thing1;
//	Victor thing2;
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
	bool clawOpen = true;
	bool elevatorExtended;
	bool dropRoutineStarted = false;
	bool dropRoutineFinished = true;
	bool droveStraight = true;
	bool turnt = true;
	//bool gyroReset = false;
	int autoLoopCounter = 0;
	int elevatorLevel = 1;
	int encoderCountNow = 0;
	int encoderCountPrev = 0;
	bool yReleased;
	bool rollersOpen = true;
	bool numanumamaticIsPressed = false;
	bool shiftUp = false;
	float rolyPolySpeed = 0;
	bool override;
	bool goingUp = false;
	bool goingDown = false;
	Timer brakeTime;
	Timer autonTimer;
	Claw claws;
	Elevator elevator;
	Compressor compressor;
	Rollers rollers;
	Gyro gyro1;

public:
	Robot() :
		myRobot(joeTalon, kaylaTalon),	// these must be initialized in the same order
		stick(0),// as they are declared above.
		lw(NULL),
		joeTalon(LEFT_DRIVE_MOTOR),
		kaylaTalon(RIGHT_DRIVE_MOTOR),
//		loretta(9),
//		rhonda(8),
//		thing1(5),
//		thing2(4),
//		numanumamaticExtend(0),
//		numanumamaticRetract(1),
		shiftUpExtend(SOLENOID_DRIVE_SHIFT_EXTEND),
		shiftUpRetract(SOLENOID_DRIVE_SHIFT_RETRACT),
		rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B, true),
		leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B, true),
		clicker(0),
		claws(SOLENOID_CLAW_EXTEND, SOLENOID_CLAW_RETRACT),
		elevator(SOLENOID_ELEVATOR_EXTEND, SOLENOID_ELEVATOR_RETRACT, SOLENOID_ELEVATOR_BRAKE_EXTEND, SOLENOID_ELEVATOR_BRAKE_RETRACT, ELEVATOR_MOTOR, ELEVATOR_ENCODER_A, ELEVATOR_ENCODER_B, LOWER_LEFT_LIMIT_SWITCH, LOWER_RIGHT_LIMIT_SWITCH, UPPER_LEFT_LIMIT_SWITCH, UPPER_RIGHT_LIMIT_SWITCH),
		compressor(5),
		rollers(SOLENOID_ROLLERS_EXTEND, SOLENOID_ROLLERS_RETRACT, RIGHT_ROLLER_MOTOR, LEFT_ROLLER_MOTOR /* , rollerSpeed */),
		gyro1(GYRO)
		//autoLoopCounter(0),
		//lastCurve(0)
	{
		myRobot.SetExpiration(0.1);
		SmartDashboard::init();
		SmartDashboard::PutBoolean("High gear on?", shiftUpExtend.Get());
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		mendableBruiser = new SendableChooser();
		mendableBruiser->AddDefault("Choose Your Autonomous!", typedef enum autonOptions.work);
		goingUp = false;
		goingDown = false;
	}

	void AutonomousInit()
	{
		autoLoopCounter = 0;
		autonTimer.Reset();
		autonTimer.Start();
		shiftUpRetract.Set(true);
		shiftUpExtend.Set(false);
		compressor.Start();
		rightEncoder.Reset();
		leftEncoder.Reset();
		gyro1.Reset();
	}

	void AutonomousPeriodic()
	{

		SmartDashboard::PutString("Choose an Autonomous!", "autonOptions");

		switch (autonOptions)
		{
		case AUTONOMOUS_JUST_DRIVE:
			AutonJustDrive();
			break;
		case AUTONOMOUS_SINGLE_TOTE:
			AutonSingleTote();
			break;
		case AUTONOMOUS_THREE_TOTE_STACK:
			AutonThreeTote();
			break;
		case AUTONOMOUS_TWO_CONTAINERS:
			AutonTwoContainers();
			break;
		case AUTONOMOUS_FIVE:
			AutonFive();
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
		shiftUpRetract.Set(true);
		shiftUpExtend.Set(false);
		elevatorExtended = false;

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
//		HAAAAAAAANNNNDDDDSSSS
		myRobot.ArcadeDrive((stick.GetRawAxis(1) * 1.0), (stick.GetRawAxis(4) * 1.0)); //0.7 dampens the steering sensitivity, modify to taste
		SmartDashboard::PutString("Do it work?!", "Aw yeah");
		//Drive don't do this, use arcade drive
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
		//Else Open Rollers
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
			SmartDashboard::PutBoolean("High gear ON?", shiftUpExtend.Get());
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
			rolyPolySpeed = (stick.GetRawAxis(2));
			rollers.Eat(rolyPolySpeed);
//			thing1.SetSpeed(stick.GetRawAxis(2));
//			thing2.SetSpeed(-stick.GetRawAxis(2));
		}

		//Spin Rollers Out
		else if (stick.GetRawAxis(3) >= 0.05)
		{
			rolyPolySpeed = (stick.GetRawAxis(3));
			rollers.Barf(rolyPolySpeed);
//			SmartDashboard::Put
//			thing1.SetSpeed(-stick.GetRawAxis(2));
//			thing2.SetSpeed(stick.GetRawAxis(3));
		}
		//Stop Rollers
		else
		{
			rolyPolySpeed = 0;
			rollers.Eat(rolyPolySpeed);
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


	//Autonomous functions zone start

	void Auto_DriveStraightForwardDistance(float distance, float speed) //distance in inches
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
			return;
		}
	}

	void Auto_DriveStraightBackwardDistance(float distance, float speed) //distance in inches
		{
			rightEncoder.Reset();
			leftEncoder.Reset();
	//		if(droveStraight == true)
	//		{
	//			rightEncoder.Reset();
	//			leftEncoder.Reset();
	//			droveStraight = false;
	//		}
			if (rightEncoder.Get() > (distance * DRIVEWHEEL_PULSES_PER_INCH) && leftEncoder.Get() > (distance * DRIVEWHEEL_PULSES_PER_INCH)) //Might need a buffer here
			{
				myRobot.Drive(-speed, 0.0);
			}
			else
			{
				myRobot.Drive(0.0, 0.0);
				droveStraight = true;
				return;
			}
		}

	void Auto_ZeroPointTurn(float degrees, float speed)
	{
		//TODO: figure out rotations/degree for the drive wheels when both sides are moving opposite directions (i.e. stationary turning)
		//TODO edit: I do believe that we can do this with gyro code which is pretty simple.

		gyro1.Reset();

		if (gyro1.GetAngle() != degrees || gyro1.GetAngle() != (degrees + 1) ||gyro1.GetAngle() != (degrees - 1))
		{
			myRobot.Drive(0.0, speed);
		}
		else
		{
			myRobot.Drive(0.0, 0.0);
			return;
		}

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

	void AutonJustDrive(void) //Drives forward. Could push tote/container
	{
		if (autonTimer.Get() < 10) //10 seconds is more than enough. We just don't yet know how long it will take for the robot to drive however far forward. This is how we're going to sequence autonomous stuff: giving functions set completion times. -Ben
		{
			Auto_DriveStraightForwardDistance(12, 0.5); //Forward one foot, for now
		}
	}

	void AutonSingleTote(void) //Grasps a yellow tote, turns, drives into the Auto Zone, and goes and sets it on the Landmark
	{
		//For now, we'll code this in regards to starting at the middle yellow tote
		switch (autoLoopCounter)
		{

		case 0:
			if (autonTimer.Get() < 2)
			{
				if (clawOpen == true)
				{
					claws.CloseClaw();
					clawOpen = false;

					elevator.BrakeOff();
					elevator.SetLevel(4);
				}

				if (elevator.IsAtLevel() == true)
				{
					elevator.BrakeOn();
				}
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
	//			Do we need to use implement something like this Joe Code vvv for anything?
	//            if (autonReset)
	//            {
	//                autonDrivingForward.Reset();
	//                autonDrivingForward.Start();
	//            }
	//            myRobot.Drive(0.0,0.0);
	//            if (autonDrivingForward.Get() >=  0.2)
	//            {
	//                autonReset = true;
	//                autonStepCount++;
	//            }
		case 1:
			if (autonTimer.Get() < 2)
			{
				Auto_ZeroPointTurn(90, 0.6);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;

		case 2:
			if (autonTimer.Get() < 4)
			{
				Auto_DriveStraightForwardDistance(107, 1.0);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 3:
			if (autonTimer.Get() < 2)
			{
				Auto_ZeroPointTurn(-90, 0.6);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 4:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(24, 1.0);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 5:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOff();
				elevator.SetLevel(0);
				if (elevator.IsAtLevel() == true)
				{
					elevator.BrakeOn();
					claws.OpenClaw();
					clawOpen = true;
				}
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 6:
			if (autonTimer.Get() < 3)
			{
				Auto_DriveStraightBackwardDistance(36, 1.0);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 7:
			myRobot.Drive(0, 0);
			break;
		}
	}

	void AutonThreeTote(void)  //Three tote stack autonomous
	{

		float gyro_angle = gyro1.GetAngle();
		switch (autoLoopCounter)
		{
		case 0:
			if (autonTimer.Get() < 2)
			{
				claws.CloseClaw();
				rollers.Eat(0.7);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 1:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOff();
				elevator.SetLevel(0);
				rollers.OpenRollers();
				rollers.RollersIdle();
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 2:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOn();
				elevator.SetLevel(2);
				rollers.PushLeft(0.7);
				Auto_DriveStraightForwardDistance(6, 0.3);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 3:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(57, 0.8);
				myRobot.Drive(0.8, 0.0);
				rollers.Eat(0.7);
			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 4:
			if (autonTimer.Get() < 2)
			{
				myRobot.Drive(0, 0);
				rollers.CloseRollers();
				rollers.Eat(0.5);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 5:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOff();
				elevator.SetLevel(0);
				rollers.OpenRollers();
				rollers.RollersIdle();
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 6:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOn();
				elevator.SetLevel(2);
				rollers.PushLeft(0.7);
				Auto_DriveStraightForwardDistance(6, 0.3);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 7:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(57, 0.8);
				rollers.Eat(0.7);
			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 8:
			if (autonTimer.Get() < 2)
			{
				myRobot.Drive(0, 0);
				rollers.CloseRollers();
				rollers.Eat(0.5);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 9:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOff();
				elevator.SetLevel(0);
				rollers.OpenRollers();
				rollers.RollersIdle();
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 10:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOn();
				elevator.SetLevel(1);
				Auto_ZeroPointTurn(90, 0.5);
//				if (gyro_angle < 90 || gyro_angle < -90)
//				{
//					myRobot.Drive(0.0, 0.5);
//				}
//				else if (gyro_angle == 90 || gyro_angle == -90)
//				{
//					myRobot.Drive(0.0, 0.0);
//				}
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 11:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(65, 0.8);
			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();

			}
			break;
		case 12:
			if (autonTimer.Get() < 2)
			{
				Auto_ZeroPointTurn(90, 0.5);
//				if (gyro_angle < 180 || gyro_angle < -180)
//				{
//					myRobot.Drive(0.0, 0.5);
//				}
//				else if (gyro_angle == 180 || gyro_angle == -180)
//				{
//					myRobot.Drive(0.0, 0.0);
//				}
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 13:
			if (autonTimer.Get() < 2)
			{
				elevator.BrakeOff();
				rollers.OpenRollers();
				elevator.SetLevel(0);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 14:
			if (autonTimer.Get() < 2)
			{
				rollers.CloseRollers();
				Auto_DriveStraightBackwardDistance(57, 0.8);
				rollers.Barf(0.7);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		}
	}

	void AutonTwoContainers(void) //Grabs 2 containers off the step TODO: need to figure out how hooks work so we can program them
	{
		switch (autoLoopCounter)
		{
		case 0:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(48, 0.5);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 1:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				// do hooky stuff
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		case 2:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightBackwardDistance(68, 0.5);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
			}
			break;
		}
	}

	void AutonFive(void) //
	{
		switch (autoLoopCounter)
		{
		case 0:
			if (autonTimer.Get() < 2)
			{

			}
		}
	}


};

START_ROBOT_CLASS(Robot);
//six more
// five more
//(What do you do if you are still hungry after dinner?  You go back) four more
//teh chezy pofs more
// two more
//juan more
//10 burcks, 100 twiddies, 1 shafer, .000000000000000000001 jacobson
