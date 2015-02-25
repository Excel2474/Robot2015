/*
 * Elevator.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Elevator.h"

Elevator::Elevator(int elevator_extend, int elevator_retract, int elevator_brake_extend, int elevator_brake_retract, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_left_limit, int lower_right_limit, int upper_left_limit, int upper_right_limit):
elevatorExtend(elevator_extend),
elevatorRetract(elevator_retract),
elevatorBrakeExtend(elevator_brake_extend),
elevatorBrakeRetract(elevator_brake_retract),
elevatorMotor(elevator_motor),
elevatorEncoder(elevator_encoder_A, elevator_encoder_B, true),
leftLowerLimit(lower_left_limit),
rightLowerLimit(lower_right_limit),
leftUpperLimit(upper_left_limit),
rightUpperLimit(upper_right_limit),
destinationLevel(LEVEL_ONE),
destinationFloor(1),
elevatorPid(0.1, 0.01, 0.0, &elevatorEncoder, &elevatorMotor) //you must use a split pwm to drive both victors from one pwm output; then you just have an elevatorMotor victor declaration, which drives two motors

{
	elevatorEncoder.SetDistancePerPulse(0.044007429891485);
//	elevatorEncoder.SetDistancePerPulse((distPerPulse/pulsesPerRotation)) 256 pulses per rotation; ??? distance per rotation (compute this from gear ratios and
// pd = 1.751, ratio = 1:2, 2(pi)1.751
	SmartDashboard::init();
	SmartDashboard::PutNumber("Current Elevator Level", destinationFloor);

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
		if (leftLowerLimit.Get() == true || rightLowerLimit.Get() == true)
		{
			elevatorEncoder.Reset();
			elevatorPid.Reset();
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
	if (elevatorBrakeExtend.Get() == false && elevatorBrakeRetract.Get() == true)
	{
		return;
	}
	else
	{
		elevatorBrakeExtend.Set(false);
		elevatorBrakeRetract.Set(true);
	}
}

void Elevator::BrakeOff()
{
	if (elevatorBrakeExtend.Get() == true && elevatorBrakeRetract.Get() == false)
	{
		return;
	}
	else
	{
		elevatorBrakeExtend.Set(true);
		elevatorBrakeRetract.Set(false);
	}
}

void Elevator::SetLevel(int destinationLevel)
{
	if (destinationLevel >= 0 && destinationLevel <= 6) // Makes sure level exists -- because reasons
	{
		switch (destinationLevel)
		{
		case 0:
			if (leftLowerLimit.Get() == false && rightLowerLimit.Get() == false)
			{
				elevatorPid.SetSetpoint(LEVEL_ZERO);
				destinationFloor = 0;
			}
			else
			{
				elevatorMotor.SetSpeed(0);	//Do we need this? It would be useful, wouldn't it?
				elevatorEncoder.Reset();
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
	if ((destinationLevel < (elevatorEncoder.Get() + 1)) && (destinationLevel > (elevatorEncoder.Get() - 1))) //destinationLevel >= (elevatorEncoder.Get() - 1.0) && destinationLevel <= (elevatorEncoder.Get() + 1)
	{
		SmartDashboard::PutNumber("Current Elevator Level", destinationFloor);
		return true;
	}
	else
	{
		return false;
	}
}

void Elevator::TestElevatorMotor(float motorSpeed)
{
	//logic to measure: "if it's at the top, it won't run up" and "if it's at the bottom, it won't run down"
	//I don't know how the motor is oriented, so the stick axis/motor direction correspondence may be wrong
	if (motorSpeed < -0.02)
	{
		if ( /* elevatorEncoder.Get() == 0 || */ leftLowerLimit.Get() == false /* || rightLowerLimit.Get() == true */)
		{
			elevatorMotor.SetSpeed(0);
		}
		else
		{
			elevatorMotor.SetSpeed(motorSpeed * 0.6); //Buffer, just in case
		}
	}
	else if (motorSpeed > 0.02)
	{
//		if (elevatorEncoder.Get() == 1500 || leftUpperLimit.Get() == true || rightUpperLimit.Get() == true ) //We should define a constant for the maximum possible count the encoder can have. For now, I'm using the count value for Level Six
//		{
//			elevatorMotor.SetSpeed(0);
//		}
//		else
//		{
			elevatorMotor.SetSpeed(motorSpeed * 0.6); //Buffer, just in case
//		}
	}
	else
	{
		elevatorMotor.SetSpeed(0);
	}
}

/*
 * You'll also want a get desired level function and a is at desied level function
 * Ignore D, unless someone helps with tuning
 * Set Level, rather than Up + Down;
 */
