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

#define button_a  1 // 1
#define	button_b  2 // 2
#define button_x  3 // 3
#define	button_y  4 // 4
#define	button_left_trigger  5 // 5
#define	button_right_trigger  6 // 6
#define	button_back  7 // 7
#define	button_start  8 // 8
// (256 pulses / circumference)

typedef enum
{
	ELEVATOR_ENCODER_A, //0
	ELEVATOR_ENCODER_B, //1
	LEFT_ENCODER_A, //2
	LEFT_ENCODER_B, //3
	RIGHT_ENCODER_A, //4
	RIGHT_ENCODER_B, //5
	PAUSE, //6
	LOWER_LEFT_LIMIT_SWITCH, //7
	UPPER_LEFT_LIMIT_SWITCH, //8
	UPPER_RIGHT_LIMIT_SWITCH, //9
	LOWER_RIGHT_LIMIT_SWITCH //10
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
	AUTONOMOUS_ONE_CONTAINER
}AUTONOMOUS_OPTIONS;


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

}SOLENOID_CHANNEL_MOD_0;

typedef enum
{
	SOLENOID_CLAW_EXTEND,
	SOLENOID_CLAW_RETRACT

}SOLENOID_CHANNEL_MOD_1;



class Robot: public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Joystick opStick;
//	Joystick driver;
//	Joystick manipulator;
	//SmartDashboard driverDashboard;

	LiveWindow *lw;
	SendableChooser *mendableBruiser;
	Victor joeTalon;
//	Victor leftdrive;
//	Victor rightdrive;
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
	//DigitalInput clicker;
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
	int elevatorLevel = 0; // Defines the level of the elevator
	int encoderCountNow = 0;
	int encoderCountPrev = 0;
	int pov_up = 0;
	int pov_down = 180;
	int pov_left = 90;
	int pov_right = 270;
//	float rolyPolySpeed = 0;
	float intakeSpeed = 0.0;
	float kaylaSmart = 0;
	float throttledamper = .65;
	float turningdamper = .7;
	float joeSmart = 0;
	bool yReleased;
	bool rollersOpen = true;
	bool firstTime = true;
	bool numanumamaticIsPressed = false;
	bool shiftUp = false;
//	bool override = false; // Is true of claw will crash
	bool collision = false; // Is true of claw will crash
	bool goingUp = false;
	bool goingDown = false;
	bool isBrakeOn = false;	//Should this start off as true or false?
	bool activatedElevatorDown = true;
	bool activatedElevatorUp = true;
	bool setLevel = false; // Variable that is true if the PID is set at a setpoint
//	bool setLevel0 = false;
//	bool setLevel1 = false;
//	bool setLevel2 = false;
//	bool setLevel3 = false;
	bool button7 = false;
	bool button8 = false;

	bool x_pressed = false;
	bool y_pressed = false;
	bool b_pressed = false;
	bool a_pressed = false;
	bool back_pressed = false;
	bool start_pressed = false;
	bool left_trigger_pressed = false;
	bool right_trigger_pressed = false;
	float gyroValue = 0;

	Timer brakeTime;
	Timer autonTimer;
	Timer dropRoutineTimer;
	Claw claws;
	Elevator elevator;
	Compressor compressor;
	Rollers rollers;
	Gyro gyro1;
	AUTONOMOUS_OPTIONS selectedAutonomousRoutine;


public:
	Robot() : //Define the robot
		myRobot(joeTalon, kaylaTalon),	// these must be initialized in the same order
//		myRobot(leftdrive, rightdrive),	// these must be initialized in the same order
		stick(0),// as they are declared above.
		opStick(1),
		lw(NULL),
		mendableBruiser(),
		joeTalon(LEFT_DRIVE_MOTOR),
		kaylaTalon(RIGHT_DRIVE_MOTOR),
//		leftdrive(LEFT_DRIVE_MOTOR),
//		rightdrive(RIGHT_DRIVE_MOTOR),
		shiftUpExtend(0, SOLENOID_DRIVE_SHIFT_EXTEND),
		shiftUpRetract(0, SOLENOID_DRIVE_SHIFT_RETRACT),
		rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B, true),
		leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B, true),
//		clicker(0),
		claws(SOLENOID_CLAW_EXTEND, SOLENOID_CLAW_RETRACT),
		elevator(SOLENOID_ELEVATOR_EXTEND, SOLENOID_ELEVATOR_RETRACT, SOLENOID_ELEVATOR_BRAKE_EXTEND, SOLENOID_ELEVATOR_BRAKE_RETRACT, ELEVATOR_MOTOR, ELEVATOR_ENCODER_A, ELEVATOR_ENCODER_B, LOWER_LEFT_LIMIT_SWITCH, LOWER_RIGHT_LIMIT_SWITCH, UPPER_LEFT_LIMIT_SWITCH, UPPER_RIGHT_LIMIT_SWITCH),
		compressor(5),
		rollers(SOLENOID_ROLLERS_EXTEND, SOLENOID_ROLLERS_RETRACT, RIGHT_ROLLER_MOTOR, LEFT_ROLLER_MOTOR /* , rollerSpeed */),
		gyro1(0)
		//autoLoopCounter(0),
		//lastCurve(0)
	{
		myRobot.SetExpiration(0.1);
		SmartDashboard::init();
		SmartDashboard::PutBoolean("High gear on?", shiftUpExtend.Get());
//		SmartDashboard::GetBoolean("Left Motor - Max Percentage SET", 0);
//		SmartDashboard::GetBoolean("Right Motor - Max Percentage SET", 0);
	}

private:
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();

		//vvv This is the code for changing the autonomous mode through SmartDashboard
		mendableBruiser = new SendableChooser();
		mendableBruiser->AddDefault("Drive Forward One Foot", (void *) AUTONOMOUS_JUST_DRIVE);
		mendableBruiser->AddObject("Single Tote", (void *)(AUTONOMOUS_SINGLE_TOTE));
		mendableBruiser->AddObject("Three Tote Stack", (void *)(AUTONOMOUS_THREE_TOTE_STACK));
		mendableBruiser->AddObject("Two Containers", (void *)(AUTONOMOUS_TWO_CONTAINERS));
		mendableBruiser->AddObject("One Container Into Auto Zone?", (void *)(AUTONOMOUS_ONE_CONTAINER));
		SmartDashboard::PutData("Autonomous Modes", mendableBruiser);
		//^^^

		SmartDashboard::PutNumber("Elevator_P",0.1);
		SmartDashboard::PutNumber("Elevator_I",0.0);
		SmartDashboard::PutNumber("Elevator_D",0.0);
		SmartDashboard::PutNumber("Throttle Dampening", 0.5);
		SmartDashboard::PutNumber("Steering Dampening", 0.5);
		SmartDashboard::PutNumber("Is Gyro?", gyro1.GetAngle());
	}

	void AutonomousInit()
	{
		//vvv SmartDashboard autonomous-choose thingy
		selectedAutonomousRoutine = (AUTONOMOUS_OPTIONS)((int) mendableBruiser->GetSelected());

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

		elevator.Execute();
		SmartDashboard::PutString("Choose an Autonomous!", "autonOptions");

		//TODO: Is this it?!?!: switch (autonomousCommand)
		//(Command *) autonomousCommand
		switch (selectedAutonomousRoutine)
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
		case AUTONOMOUS_ONE_CONTAINER:
			AutonOneContainer();
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
		compressor.Start();
		shiftUpRetract.Set(true);
		shiftUpExtend.Set(false);
		elevatorExtended = false;

	}

	void TeleopDisabled()
	{
		//driverDashboard.PutBoolean("Extended", extended);
		elevator.Execute();
	}

	void TeleopPeriodic()
	{
		gyroValue = gyro1.GetAngle();

		elevator.Execute();
		SmartDashboard::PutBoolean("High gear ON?", shiftUpExtend.Get());
		SmartDashboard::PutNumber("Angle", gyroValue);
		SmartDashboard::PutBoolean("OverRide", collision);
		SmartDashboard::PutString("Do it work?!", "Aw yeah");
//		SmartDashboard::PutNumber("Left Side Speed", leftdrive.Get());
//		SmartDashboard::PutNumber("Right Side Speed", rightdrive.Get());
		SmartDashboard::PutBoolean("button 7",stick.GetRawButton(7));
		//double rightEncoderRate = rightEncoder.GetRate();
		//double rightRPM = (rightEncoderRate/256) * 60;

		//vvv SmartDashboard motor buffer setter
		joeSmart = SmartDashboard::GetNumber("Throttle Dampening", 0.5);
		kaylaSmart = SmartDashboard::GetNumber("Steering Dampening", 0.5);

		//PID test helper concept

		//Drop Routine Start
//		float p = SmartDashboard::GetNumber("Elevator_P");
//		float i = SmartDashboard::GetNumber("Elevator_I");
//		float d = SmartDashboard::GetNumber("Elevator_D");

//		static bool wasButton8Pressed = false;//static means that the variable is saved even after you leave the function
//		if (stick.GetRawButton(8) == true)
//		{
//
//			if (wasButton8Pressed == false)
//			{
//				elevator.SetPID(p,i,d);
//				wasButton8Pressed = true;
//			}
//		}
//		else
//		{
//			wasButton8Pressed = false;
//		}

		//^^^
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


//NOTE: Change all buttons to the enum values

// Drivetrain motor values. FIX THIS

		if ((stick.GetRawAxis(4) < -0.1 || stick.GetRawAxis(4) > 0.1) || (stick.GetRawAxis(1) < -0.1 || stick.GetRawAxis(1) > 0.1))
		{
			myRobot.ArcadeDrive((stick.GetRawAxis(4) * turningdamper), (stick.GetRawAxis(1) * throttledamper)); //0.7 dampens the steering sensitivity, modify to taste
		}

// Calibrate elevator or cancel target position. Check this whole function. I'm concerned about button7=true
		if ((stick.GetRawButton(button_start) == true || opStick.GetRawButton(button_start) == true)  || start_pressed==true) //Calibrate Elevator
		{
			if (elevator.Calibrate()==true) //Runs the "Calibrate" function and returns true when complete
			{
				start_pressed=false;
				elevatorLevel = 0;
			}
			else
			{
//				setLevel0=false;
//				setLevel1=false;
//				setLevel2=false;
//				setLevel3=false;

				setLevel=false;
				start_pressed=true;
			}
		}

		if (stick.GetRawButton(button_back) == true || opStick.GetRawButton(button_back) == true) //Cancels set point
		{
//			setLevel0=false;
//			setLevel1=false;
//			setLevel2=false;
//			setLevel3=false;

			setLevel=false;
			elevator.stopPID();
		}

// Check to see if collision case is true
		if (elevator.IsCrashing() == true || elevatorExtended == true)
		{
			collision = true; //override is a bad name // unless you understand what it means
		}
		else if (elevator.IsCrashing() == false)
		{
			collision = false;
		}


// Open the claw
		if ((stick.GetPOV(0) == pov_right || opStick.GetPOV(0) == pov_right) && clawOpen == false)
		{
			clawOpen = true;
			claws.OpenClaw();
		}

		if ((stick.GetPOV(0) == pov_left || opStick.GetPOV(0)) && clawOpen == true)
		{
			clawOpen = false;
			claws.CloseClaw();
		}

//Extend Elevator
		if ((stick.GetPOV(0) == pov_up || opStick.GetPOV(0) == pov_up) && elevatorExtended == false)
		{
			elevator.ExtendElevator();
			elevatorExtended = true;
		}

//Retract Elevator
		if ((stick.GetPOV(0) == pov_down || opStick.GetPOV(0) == pov_down) && elevatorExtended == true)
		{
			elevator.RetractElevator();
			elevatorExtended = false;
		}


// Set elevator level setpoint

		// Increment Elevator by 1
		if ((stick.GetRawButton(button_x) == true || opStick.GetRawButton(button_x)) && x_pressed == false)
		{
			x_pressed = true; //set button status to "pressed"
		}

		else if (x_pressed == true && (stick.GetRawButton(button_x) == false && opStick.GetRawButton(button_x) == false)) // If the elevator button was pressed and released
		{

			//Increment the elevator level
			if(elevatorLevel < elevator.maxLevel())
			{
				elevatorLevel = elevatorLevel + 1; // elevatorLevel+1
				// setLevel0 = true;
				setLevel = true;
			}
			else
			{
				//do not increment the elevator level
			}

			x_pressed = false; //reset button status to "released"
		}
//		else
//		{
//			b_pressed = false; // Any other condition results in button pressed set to false
//		}


		if (((stick.GetRawButton(button_a) || opStick.GetRawButton(button_a)) == true) && a_pressed == false)
		{
			a_pressed = true; //set button status to "pressed"
		}

		else if (a_pressed == true && (stick.GetRawButton(button_a) == false && opStick.GetRawButton(button_a) == false)) // If the elevator button was pressed and released
		{

			//Increment the elevator level
			if(elevatorLevel > elevator.minLevel())
			{
				elevatorLevel = elevatorLevel - 1; // elevatorLevel+1
				// setLevel0 = true;
				setLevel = true;
			}
			else
			{
				//do not increment the elevator level
			}

			a_pressed = false; //reset button status to "released"
		}

		if (((stick.GetRawButton(button_b) || opStick.GetRawButton(button_b)) == true) && b_pressed == false)
		{
			b_pressed = true; //set button status to "pressed"
		}

		else if (b_pressed == true && (stick.GetRawButton(button_b) == false && opStick.GetRawButton(button_b) == false)) // If the elevator button was pressed and released
		{

			//Set elevator level to minimum elevator level
			elevatorLevel = elevator.minLevel();
			setLevel = true;
			b_pressed = false; //reset button status to "released"
		}

		if ((stick.GetRawButton(button_y) || opStick.GetRawButton(button_y)) == true && y_pressed == false)
		{
			y_pressed = true; //set button status to "pressed"
		}

		else if (y_pressed == true && (stick.GetRawButton(button_y) == false && opStick.GetRawButton(button_y) == false)) // If the elevator button was pressed and released
		{

			//Set elevator level to minimum elevator level
			elevatorLevel = elevator.maxLevel();
			setLevel = true;
			y_pressed = false; //reset button status to "released"
		}

		// Decrement Elevator by 1
		SmartDashboard::PutNumber(" my inc", elevatorLevel);
		SmartDashboard::PutBoolean(" Set Level", setLevel);
// Elevator pre-set levels
		/* Using increment method		 */

//	   switch(elevatorLevel)
//	   {
//	   	   case 0:
//	   		   setLevel0=true;
//
//				setLevel1=false;
//				setLevel2=false;
//				setLevel3=false;
//		   break;
//
//	   	   case 1:
//	   		   setLevel1=true;
//
//				setLevel0=false;
//				setLevel2=false;
//				setLevel3=false;
//		   break;
//
//	   	   case 2:
//	   		   setLevel2=true;
//
//				setLevel0=false;
//				setLevel1=false;
//				setLevel3=false;
//		   break;
//
//	   	   case 3:
//	   		   setLevel3=true;
//
//				setLevel0=false;
//				setLevel1=false;
//				setLevel2=false;
//		   break;
//	   }

		//Elevator to level 0
		if (setLevel == true) // setLevel is a bool that is true if the PID is set at a level and false if the PID is not set at a level
		{
//			setLevel1=false;
//			setLevel2=false;
//			setLevel3=false;

			setLevel = elevator.setPID(.6,-.4, elevatorLevel); //SetPID(max_out,min_out, setpoint) where setpoint is an integer corresponding to preset locations in our switch case. SetPID also returns true if it reaches it's target
		}
		/* Stop Using increment method		 */

//		//Elevator to level 0
//		if (setLevel0==true)
//		{
//			setLevel1=false;
//			setLevel2=false;
//			setLevel3=false;
//
//			setLevel0 = elevator.setPID(.6,-.4, 0); //SetPID(max_out,min_out, setpoint) where setpoint is an integer corresponding to preset locations in our switch case. SetPID also returns true if it reaches it's target
//		}
//
//		//Elevator to level 1
//		if (setLevel1==true)
//		{
//			setLevel0=false;
//			setLevel2=false;
//			setLevel3=false;
//
//			setLevel1 = elevator.setPID(.6,-.4, 1);
//		}
//
//		//Elevator to level 2
//		if (setLevel2==true)
//		{
//			setLevel0=false;
//			setLevel1=false;
//			setLevel3=false;
//
//			setLevel2 = elevator.setPID(.6,-.4, 2);
//		}
//
//		//Elevator to level 3
//		if (setLevel3==true)
//		{
//			setLevel0=false;
//			setLevel1=false;
//			setLevel2=false;
//
//			setLevel3 = elevator.setPID(.5,-.2, 3); // if elevator reaches set point, it will return "false" (to disable) and stop the motor
//		}


// Drivetrain shifting. Change this to shift only when the button is held

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

//Control Rollers

		SmartDashboard::PutBoolean("Collision", collision);

		//Spin Rollers In
		if (((stick.GetRawAxis(2) || opStick.GetRawAxis(2)) >= 0.1)  && ((stick.GetRawAxis(3) && opStick.GetRawAxis(3)) == 0.0) && collision == false) // Those triggers are probably variable
		{
			intakeSpeed = 1.0;
			rollers.Barf(intakeSpeed);
			rollers.CloseRollers();
		}

		//Spin Rollers Out
		else if (((stick.GetRawAxis(3) || opStick.GetRawAxis(3)) >= 0.1)  && ((stick.GetRawAxis(2) && opStick.GetRawAxis(2)) == 0.0) && collision == false)
		{
			intakeSpeed = 1.0;
			rollers.Eat(intakeSpeed);
			rollers.CloseRollers();
		}

		//Stop Rollers
		else
		{
			//rollers.OpenRollers();
			rollers.RollersIdle();
		}



		//Drop Routine Start TODO: Is drop routine coopertition related? or is it for any stack? Check for time values
	/*	if (stick.GetRawButton(3) == true && dropRoutineStarted == false && dropRoutineFinished == true)
		{
			if (firstTime == true)
			{
				dropRoutineTimer.Reset();
				dropRoutineTimer.Start();
				elevatorLevel = elevatorLevel - 1;
				firstTime = false;
			}
			dropRoutineStarted = true;
			dropRoutineFinished = false;
			if (dropRoutineTimer.Get() < 2)
			{
				elevator.ExtendElevator();
			}
			else if (dropRoutineTimer.Get() >= 2 && dropRoutineTimer.Get() < 4)
			{
				elevator.SetLevel(elevatorLevel);
			}

			else if (dropRoutineTimer.Get() >= 4 && dropRoutineTimer.Get() < 6)
			{
				claws.OpenClaw();
			}

			else if (dropRoutineTimer.Get() >= 6 && dropRoutineTimer.Get() < 8)
			{
				elevator.RetractElevator();
			}

			else
			{
				claws.CloseClaw();
				firstTime = true;
				dropRoutineStarted = false;
				dropRoutineFinished = true;
			}
			//Call drop routine
		}

		//Lower Elevator Level
		if (stick.GetRawButton(1) == true && elevatorLevel != 0)
		{
			goingDown = true;
		}

		if (goingDown == true && goingUp == false) //This logic here with the && needs to be checked. -Ben
		{
			if (activatedElevatorDown == true)
			{
				elevator.BrakeOff();
				//brakeTime.Reset();
				//brakeTime.Start();
				//if (brakeTime.Get() > 0.1)
				//{
				elevatorLevel = elevatorLevel - 1;
				elevator.SetLevel(elevatorLevel);
			//^Call LOWER elevator function
				activatedElevatorDown = false;
			}
			if (elevator.IsAtLevel() == true)
			{
				elevator.BrakeOn();
				goingDown = false;
				activatedElevatorDown = true;
			}
		}
*/









//		if (goingUp == true && goingDown == false) //This logic right here with the && needs to be checked. -Ben
//		{
//			if (activatedElevatorUp == true)
//			{
//				elevator.BrakeOff();
//	//			brakeTime.Reset();
//	//			brakeTime.Start();
//	//			if (brakeTime.Get() > 0.1)
//	//			{
//				elevatorLevel = elevatorLevel + 1;
//				elevator.SetLevel(elevatorLevel);
//				//^Call RAISE elevator function
//				activatedElevatorUp = false;
//			}
//			if (elevator.IsAtLevel() == true)
//			{
//				elevator.BrakeOn();
//				goingUp = false;
//				activatedElevatorUp = true;
//			}
//		}


		//TEST CODE - Control elevator motor speed
//		if (stick.GetRawAxis(0) < -0.1 || stick.GetRawAxis(0) > 0.1)
//		{
//			elevator.DisablePid();
////			brakeTime.Start();
////			if (brakeTime.Get() < .3)
////			{
////				elevator.TestElevatorMotor(-0.5);
////				elevator.BrakeOff();
////			}
////			else if (brakeTime.Get() >= .3)
////			{
////				elevator.TestElevatorMotor(stick.GetRawAxis(0));
////			}
//			elevator.TestElevatorMotor(stick.GetRawAxis(0));
//		}
//		else
//		{
//			elevator.TestElevatorMotor(0);
////			elevator.BrakeOn();
////			brakeTime.Reset();
//		}

		//TEST CODE - Toggle elevator brake
//		static bool wasButton7Pressed = false;
//		if (stick.GetRawButton(7) == true)
//		{
//			if (wasButton7Pressed == false)
//			{
//				if ((elevator.elevatorBrakeExtend.Get() == true) && (elevator.elevatorBrakeRetract.Get() == false))
//				{
//					elevator.BrakeOn();
//				}
//				if ((elevator.elevatorBrakeExtend.Get() == false) && (elevator.elevatorBrakeRetract.Get() == true))
//				{
//					elevator.BrakeOff();
//				}
//				wasButton7Pressed = true;
//			}
//		}
//		else
//		{
//			wasButton7Pressed = false;
//		}


// I don't think we need any of this
		// Change buttons to enum values
/*		if (stick.GetRawButton(4) == true && yReleased == true && collision == false) // Change yReleased to use the correct button_pressed var
		{
			override = true; // What? How can you call this true?

//			if (stick.GetRawButton(5) == true && rollersOpen == false)
//			{
//				rollersOpen = true;
//				//override left bumper
//			}

			// call ignore our POV 4 when called

			rollers.CloseRollers();
			rollersOpen = false;
			// Call Open claw and rollers wide
		}

		else
		{
			rollers.OpenRollers();
			rollersOpen = true;
		}

*/



//		//Close Rollers
//		if (stick.GetRawButton(5) == true /* && rollersOpen == true && override == false && elevatorLevel != 0*/)
//		{
//			rollersOpen = false;
//			rollers.CloseRollers();
//			//Call close rollers function
//		}
//		//Else Open Rollers
//		else //if (stick.GetRawButton(5) == false && rollersOpen == false && override == false)
//		{
//			rollersOpen = true;
//			rollers.OpenRollers();
//			//Call open rollers function
//		}


//		if (stick.GetRawAxis(3) >= 0.1 && stick.GetRawAxis(2) == 0.0)
//		{
//			SmartDashboard::Put
//			thing1.SetSpeed(-stick.GetRawAxis(2));
//			thing2.SetSpeed(stick.GetRawAxis(3));
//		}


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
	//TODO: Add in the gyro angle to allow going straight
	void Auto_DriveStraightForwardDistance(float distance, float speed) //distance in inches
	{
//Clean up this AUTO
		float angle = gyro1.GetAngle();
		float gyroConstant = 0.03;
		//JOE: You can't reset these here, or you'll always Get() zero
//		rightEncoder.Reset();
//		leftEncoder.Reset();
//		if(droveStraight == true)
//		{
//			rightEncoder.Reset();
//			leftEncoder.Reset();
//			droveStraight = false;
//		}

		//JOE: DO this rightEncoder.SetDistancePerPulse(or whatever that function is), then here, use rightEncoder.GetDistance
		if (rightEncoder.Get() < (distance * DRIVEWHEEL_PULSES_PER_INCH) && leftEncoder.Get() < (distance * DRIVEWHEEL_PULSES_PER_INCH)) //Might need a buffer here
		{
			myRobot.Drive(speed, -angle * gyroConstant);
			Wait(0.005);
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
//			rightEncoder.Reset();
//			leftEncoder.Reset();
			float backAngle = gyro1.GetAngle();
			float gyroBackConstant = 0.03;
	//		if(droveStraight == true)
	//		{
	//			rightEncoder.Reset();
	//			leftEncoder.Reset();
	//			droveStraight = false;
	//		}
			if (rightEncoder.Get() > (distance * DRIVEWHEEL_PULSES_PER_INCH) && leftEncoder.Get() > (distance * DRIVEWHEEL_PULSES_PER_INCH)) //Might need a buffer here
			{
				myRobot.Drive(-speed, -backAngle * gyroBackConstant);
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
		//gyro1.Reset();

		//if (gyro1.GetAngle() != degrees || gyro1.GetAngle() != (degrees + 1) ||gyro1.GetAngle() != (degrees - 1))
		if (gyro1.GetAngle() < (degrees - 1) || gyro1.GetAngle() > (degrees + 1))
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

	void AutonJustDrive() //Drives forward. Could push tote/container
	{
		if (autonTimer.Get() < 10) //10 seconds is more than enough. We just don't yet know how long it will take for the robot to drive however far forward. This is how we're going to sequence autonomous stuff: giving functions set completion times. -Ben
		{
			Auto_DriveStraightForwardDistance(12, 0.2); //Forward one foot, for now
		}
		else
		{
			myRobot.Drive(0, 0);
			rightEncoder.Reset();
			leftEncoder.Reset();
		}
	}

	void AutonSingleTote() //Grasps a yellow tote, turns, drives into the Auto Zone, and goes and sets it on the Landmark
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
				Auto_ZeroPointTurn(90, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				gyro1.Reset();
			}
			break;

		case 2:
			if (autonTimer.Get() < 4)
			{
				Auto_DriveStraightForwardDistance(107, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		case 3:
			if (autonTimer.Get() < 2)
			{
				Auto_ZeroPointTurn(-90, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				gyro1.Reset();
			}
			break;
		case 4:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(24, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
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
				Auto_DriveStraightBackwardDistance(36, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		case 7:
			myRobot.Drive(0, 0);
			break;
		}
	}

	void AutonThreeTote()  //Three tote stack autonomous
	{

		float gyro_angle = gyro1.GetAngle();
		switch (autoLoopCounter)
		{
		case 0:
			if (autonTimer.Get() < 2)
			{
				rollers.CloseRollers();
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
				Auto_DriveStraightForwardDistance(6, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		case 3:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(57, 0.2);
				myRobot.Drive(0.8, 0.0);
				rollers.Eat(0.7);
			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
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
				Auto_DriveStraightForwardDistance(6, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		case 7:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(57, 0.2);
				rollers.Eat(0.7);
			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
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
				Auto_ZeroPointTurn(90, 0.2);
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
				gyro1.Reset();
			}
			break;
		case 11:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(65, 0.2);
			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();

			}
			break;
		case 12:
			if (autonTimer.Get() < 2)
			{
				Auto_ZeroPointTurn(90, 0.2);
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
				gyro1.Reset();
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
				Auto_DriveStraightBackwardDistance(57, 0.2);
				rollers.Barf(0.7);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		}
	}

	void AutonTwoContainers() //Grabs 2 containers off the step TODO: need to figure out how hooks work so we can program them
	{
		switch (autoLoopCounter)
		{
		case 0:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(48, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
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
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		case 2:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightBackwardDistance(68, 0.2);
			}
			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		}
	}

	void AutonOneContainer() //Grab 1 container and goes into auto zone
	{
		switch (autoLoopCounter)
		{
		case 0:
			if (autonTimer.Get() < 2)
			{
				Auto_DriveStraightForwardDistance(6, 0.2);
				rollers.CloseRollers();
				rollers.Eat(1.0);
			}

			else
			{
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;
		case 2:
			if (autonTimer.Get() < 2)
			{
				elevator.SetLevel(1);
				rollers.OpenRollers();
				rollers.Eat(0.0);
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
				Auto_DriveStraightForwardDistance(50, 0.2);

			}
			else
			{
				Auto_DriveStraightForwardDistance(0, 0.0);
				autoLoopCounter++;
				autonTimer.Reset();
				rightEncoder.Reset();
				leftEncoder.Reset();
			}
			break;

		}
	}


};

START_ROBOT_CLASS(Robot);
//six more
// five more
// four more
//teh chezy pofs more
// two more
//juan more
//100 twiddies, 10 burcks, 1 shafer, .000000000000000000001 jacobson
