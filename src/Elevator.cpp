/*
 /*
 * Elevator.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Elevator.h"

#define LIMIT_SWITCHES_ARE_MISSING

#define PULSES_PER_REVOLUTION 250
#define PITCH_DIAMETER 2.638F
#define PI 3.1415926F
#define DISTANCE_PER_PULSE ((2*PI*PITCH_DIAMETER) / PULSES_PER_REVOLUTION)
//Pitch Diameter 2.638

Elevator::Elevator(int elevator_extend, int elevator_retract, int elevator_brake_extend, int elevator_brake_retract, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_left_limit, int lower_right_limit, int upper_left_limit, int upper_right_limit):
elevatorExtend(0, elevator_extend),
elevatorRetract(0, elevator_retract),
elevatorBrakeExtend(0, elevator_brake_extend),
elevatorBrakeRetract(0, elevator_brake_retract),
elevatorMotor(elevator_motor),

elevatorEncoder(elevator_encoder_A, elevator_encoder_B, true),
leftLowerLimit(lower_left_limit),
rightLowerLimit(lower_right_limit),
leftUpperLimit(upper_left_limit),
rightUpperLimit(upper_right_limit),
destinationLevel(LEVEL_ONE),
destinationFloor(1),
elevatorPid(0.1, 0000.0000, 0.0, &elevatorEncoder, &elevatorMotor) //you must use a split pwm to drive both victors from one pwm output; then you just have an elevatorMotor victor declaration, which drives two motors
{
	elevatorEncoder.SetDistancePerPulse(DISTANCE_PER_PULSE);
//	elevatorEncoder.SetDistancePerPulse(0.044007429891485);
	elevatorEncoder.SetPIDSourceParameter(PIDSource::kDistance);
//	elevatorEncoder.SetDistancePerPulse((distPerPulse/pulsesPerRotation)) 256 pulses per rotation; ??? distance per rotation (compute this from gear ratios and
// pd = 1.751, ratio = 1:2, 2(pi)1.751
	//
	SmartDashboard::init();
	SmartDashboard::PutNumber("Current Elevator Level", destinationFloor);

	SmartDashboard::PutNumber("Distance Per Pulse", DISTANCE_PER_PULSE);

	elevatorPid.SetOutputRange(-0.2, 0.2);
	elevatorPid.SetAbsoluteTolerance(0.25);

}

void Elevator::ExtendElevator()
{
	elevatorExtend.Set(true);
	elevatorRetract.Set(false);

}

void Elevator::RetractElevator()
{
	elevatorExtend.Set(false);
	elevatorRetract.Set(true);
}

//void Elevator::LevelUp()
//{
//	elevatorEncoder.Reset();
//	if (elevatorEncoder.Get() < 42)
//	{
//		rightElevator.SetSpeed(0.5);
//		leftElevator.SetSpeed(0.5);
//	}
//	else if (elevatorEncoder.Get() >= 42)
//	{
//		rightElevator.SetSpeed(0.0);
//		leftElevator.SetSpeed(0.0);
//	}
//}
//
//void Elevator::LevelDown()
//{
//
//}

void Elevator::Execute()
{
	Timer elevatorRest;
	//SmartDashboard::PutNumber("Current Encoder Position", elevatorEncoder.GetDistance());
//#ifndef LIMIT_SWITCHES_ARE_MISSING
//	if (leftLowerLimit.Get() == true || rightLowerLimit.Get() == true)
//	{
//		elevatorEncoder.Reset();
//		elevatorPid.Reset();
//	}
//#endif
	elevatorRest.Start();
	if (IsAtLevel() == true)
	{
		elevatorPid.Disable();
		if (elevatorRest.Get() > 0.5)
		{
		BrakeOn();
		}
	}
	//This will have to keep track while the elevator is reseting watching for the limit switch, once it hits the switch then reset the encoder and pid, then set the pid setpoint to 0 and enable it
}

void Elevator::Reset()
{

//Send the motor down slow-ishly use execute function to check for hitting the lower limit switch
//This would need to be called before your first go to command, so you should keep track of whether or not it has been initialized with a boolean, and in the set routine call reset if it hasn't been done
}

void Elevator::BrakeOn()
{
//	if (elevatorBrakeExtend.Get() == false && elevatorBrakeRetract.Get() == true)
//	{
//		return;
//	}
//	else
//	{
	elevatorBrakeExtend.Set(false);
	elevatorBrakeRetract.Set(true);
//	}
}

void Elevator::BrakeOff()
{
//	if (elevatorBrakeExtend.Get() == true && elevatorBrakeRetract.Get() == false)
//	{
//		return;
//	}
//	else
//	{
	elevatorBrakeExtend.Set(true);
	elevatorBrakeRetract.Set(false);
//	}
}

void Elevator::SetLevel(int destinationLevel)
{
	if (destinationLevel >= 0 && destinationLevel <= 6) // Makes sure level exists -- because reasons
	{
		elevatorPid.Enable();
		switch (destinationLevel)
		{
		case 0:
			if (leftLowerLimit.Get() == false || rightLowerLimit.Get() == false)
			{
				elevatorPid.SetSetpoint(LEVEL_ZERO);
				destinationFloor = 0;
			}
			else
			{
				elevatorMotor.SetSpeed(0);	//Do we need this? It would be useful, wouldn't it?
//				elevatorEncoder.Reset();
			}
			break;

		case 1:
			elevatorPid.SetSetpoint(LEVEL_ONE); //This is a dummy value right now. We will need to determine the values for these constants
			destinationFloor = 1;
			break;
		case 2:
			elevatorPid.SetSetpoint(LEVEL_TWO); //This is a dummy value right now. We will need to determine the values for these constants
			destinationFloor = 2;
			break;
		case 3:
			elevatorPid.SetSetpoint(LEVEL_THREE); //This is a dummy value right now. We will need to determine the values for these constants
			destinationFloor = 3;
			break;
		case 4:
			elevatorPid.SetSetpoint(LEVEL_FOUR); //This is a dummy value right now. We will need to determine the values for these constants
			destinationFloor = 4;
			break;
		case 5:
			elevatorPid.SetSetpoint(LEVEL_FIVE); //This is a dummy value right now. We will need to determine the values for these constants
			destinationFloor = 5;
			break;
		case 6:
			if (leftUpperLimit.Get() == false && rightUpperLimit.Get() == false)
			{
				elevatorPid.SetSetpoint(LEVEL_SIX);
				destinationFloor = 6;
			}
			else
			{
				elevatorMotor.SetSpeed(0);	//Do we need this? It would be useful, wouldn't it?  NO!!!
			}
			break;
		}
	}
	else
	{
		elevatorMotor.SetSpeed(0);	//Do we need this? It would be useful, wouldn't it?  NO!!!
	}
}
//	else
//	{
//		elevatorEncoder.Reset();
//	}

	//Make sure the elevator has been reset, if it hasn't, do that instead and just return from this after calling it

	//Then, make sure the level exists, because reasons, or just use a switch case statement to map level number to setpoint value
	//your go up level and go down level functions should call this, to do that, just keep track of the current desired level, and add/subtract a level and call this function with the new desired level
	//If you do incremental level up/down, add feedback leds to robot to indicate what level it is currently trying to go to, otherwise you should just use a button for each level.

bool Elevator::IsAtLevel()
{
	return elevatorPid.OnTarget();
//	if (destinationLevel == 0)
//	{
//		destinationPulse = 0;
//	}
//	if (destinationLevel == 1)
//	{
//		destinationPulse = 136;
//	}
//	if (destinationLevel == 2)
//	{
//		destinationPulse = 409;
//	}
//	if (destinationLevel == 3)
//	{
//		destinationPulse = 682;
//	}
//	if (destinationLevel == 4)
//	{
//		destinationPulse = 954;
//	}
//	if (destinationLevel == 5)
//	{
//		destinationPulse = 1227;
//	}
//	if (destinationLevel == 6)
//	{
//		destinationPulse = 1500;
//	}
//	if ((destinationPulse < (elevatorEncoder.Get() + 2)) && (destinationPulse > (elevatorEncoder.Get() - 2))) //destinationLevel >= (elevatorEncoder.Get() - 1.0) && destinationLevel <= (elevatorEncoder.Get() + 1)
//	{
//		SmartDashboard::PutNumber("Current Elevator Level", destinationFloor);
//		return true;
//	}
//	else
//	{
//		return false;
//	}
}

void Elevator::TestElevatorMotor(float motorSpeed)
{
	SmartDashboard::PutNumber("Current Encoder Position", elevatorEncoder.Get());
	SmartDashboard::PutBoolean("Is it Clicking?", leftUpperLimit.Get());
	//logic to measure: "if it's at the top, it won't run up" and "if it's at the bottom, it won't run down"
	//I don't know how the motor is oriented, so the stick axis/motor direction correspondence may be wrong
	float aCertainFloat;
	if (motorSpeed > 0.02)
	{
		if (leftUpperLimit.Get() == false || rightUpperLimit.Get() == false)
		{
			elevatorMotor.SetSpeed(0);
		}
		else
		{
			if (elevatorEncoder.Get() > aCertainFloat)
			{
				elevatorMotor.SetSpeed(-motorSpeed * 0.6); //Buffer, just in case
				aCertainFloat = 0;
			}
			else
			{
				elevatorMotor.SetSpeed(motorSpeed);
				BrakeOff();
			}
		}
	}
	else if (motorSpeed < -0.02)
	{
		elevatorEncoder.Get();
		//if (elevatorEncoder.Get() == 1500 || leftUpperLimit.Get() == true || rightUpperLimit.Get() == true ) //We should define a constant for the maximum possible count the encoder can have. For now, I'm using the count value for Level Six
		if (leftUpperLimit.Get() == false || rightUpperLimit.Get() == false)
		{
			elevatorMotor.SetSpeed(0);
		}
		else
		{
			elevatorMotor.SetSpeed(motorSpeed * 0.6); //Buffer, just in case
		}
	}
	else
	{
		elevatorPid.SetSetpoint(elevatorEncoder.Get());
		elevatorMotor.SetSpeed(0);
		aCertainFloat = (elevatorEncoder.Get() + 5);
		BrakeOn();
	}
}

void Elevator::DisablePid()
{
	elevatorPid.Disable();
}

bool Elevator::IsCrashing()
{
	if (elevatorEncoder.Get() < 100 || elevatorExtend.Get() == true)
	{
		return true;
	}
	else if (elevatorEncoder.Get() >= 100 && elevatorExtend.Get() == false)
	{
		return false;
	}
	else
	{
		return false;
	}
}

/*
 * You'll also want a get desired level function and a is at desired level function
 * Ignore D, unless someone helps with tuning
 * Set Level, rather than Up + Down;
 */
