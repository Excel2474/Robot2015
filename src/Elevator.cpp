/*
 * Elevator.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Elevator.h"

Elevator::Elevator(int elevator_extend, int elevator_retract, int elevator_brake, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_limit, int upper_limit):
elevatorExtend(elevator_extend),
elevatorRetract(elevator_retract),
elevatorBrake(elevator_brake),
elevatorMotor(elevator_motor),
elevatorEncoder(elevator_encoder_A, elevator_encoder_B, true),
lowerLimit(lower_limit),
upperLimit(upper_limit),
destinationLevel(LEVEL_ONE),
elevatorPid(0.1, 0.01, 0.0, &elevatorEncoder, &elevatorMotor) //you must use a split pwm to drive both victors from one pwm output; then you just have an elevatorMotor victor declaration, which drives two motors

{
	elevatorEncoder.SetDistancePerPulse(0.044007429891485);
//	elevatorEncoder.SetDistancePerPulse((distPerPulse/pulsesPerRotation)) 256 pulses per rotation; ??? distance per rotation (compute this from gear ratios and
// pd = 1.751, ratio = 1:2, 2(pi)1.751

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
		if (lowerLimit.Get() == true)
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
	if (elevatorBrake.Get() == false)
	{
		return;
	}
	else
	{
		elevatorBrake.Set(false);
	}
}

void Elevator::BrakeOff()
{
	if (elevatorBrake.Get() == true)
	{
		return;
	}
	else
	{
		elevatorBrake.Set(true);
	}
}

void Elevator::SetLevel(int destinationLevel)
{
	if (destinationLevel >= 0 && destinationLevel <= 6) // Makes sure level exists -- because reasons
	{
		switch (destinationLevel)
		{
		case 0:
			if (lowerLimit.Get() == false)
			{
				elevatorPid.SetSetpoint(LEVEL_ZERO);
				destinationLevel = LEVEL_ZERO;
			}
			else
			{
				elevatorMotor.SetSpeed(0);	//Do we need this? It would be useful, wouldn't it?
				elevatorEncoder.Reset();
			}
			break;

		case 1:
			elevatorPid.SetSetpoint(LEVEL_ONE); //This is a dummy value right now. We will need to determine the values for these constants
			destinationLevel = LEVEL_ONE;
			break;
		case 2:
			elevatorPid.SetSetpoint(LEVEL_TWO); //This is a dummy value right now. We will need to determine the values for these constants
			destinationLevel = LEVEL_TWO;
			break;
		case 3:
			elevatorPid.SetSetpoint(LEVEL_THREE); //This is a dummy value right now. We will need to determine the values for these constants
			destinationLevel = LEVEL_THREE;
			break;
		case 4:
			elevatorPid.SetSetpoint(LEVEL_FOUR); //This is a dummy value right now. We will need to determine the values for these constants
			destinationLevel = LEVEL_FOUR;
			break;
		case 5:
			elevatorPid.SetSetpoint(LEVEL_FIVE); //This is a dummy value right now. We will need to determine the values for these constants
			destinationLevel = LEVEL_FIVE;
			break;
		case 6:
			if (upperLimit.Get() == false)
			{
				elevatorPid.SetSetpoint(LEVEL_SIX);
				destinationLevel = LEVEL_SIX;
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

bool Elevator::IsAtLevel() //desiredLevel is in encoder counts
{
	if (destinationLevel < (elevatorEncoder.Get() + 0.5) && (destinationLevel > (elevatorEncoder.Get() - 0.5))) //destinationLevel >= (elevatorEncoder.Get() - 1.0) && destinationLevel <= (elevatorEncoder.Get() + 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Elevator::TestElevatorMotor(int motorSpeed)
{
	//logic to measure: "if it's at the top, it won't run up" and "if it's at the bottom, it won't run down"
	//I don't know how the motor is oriented, so the stick axis/motor direction correspondence may be wrong
	if (motorSpeed < 0)
	{
		if (elevatorEncoder.Get() == 0)
		{
			elevatorMotor.SetSpeed(0);
		}
		else
		{
			elevatorMotor.SetSpeed(motorSpeed);
		}
	}
	else if (motorSpeed > 0)
	{
		if (elevatorEncoder.Get() == 1500) //We should define a constant for the maximum possible count the encoder can have. For now, I'm using the count value for Level Six
		{
			elevatorMotor.SetSpeed(0);
		}
		else
		{
			elevatorMotor.SetSpeed(motorSpeed);

		}
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
