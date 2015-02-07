/*
 * Elevator.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Elevator.h"

Elevator::Elevator(int elevator_extend, int elevator_retract, int right_elevator, int left_elevator, int elevator_encoder_A, int elevator_encoder_B):
elevatorExtend(elevator_extend),
elevatorRetract(elevator_retract),
rightElevator(right_elevator),
leftElevator(left_elevator),
elevatorEncoder(elevator_encoder_A, elevator_encoder_B, true),
elevatorPid(0.1, 0.01, 0.0,  elevatorEncoder, leftElevator) //you must use a split pwm to drive both victors from one pwm output; then you just have an elevatorMotor victor declaration, which drives two motors
{
//	elevatorEncoder.SetDistancePerPulse((distPerPulse/pulsesPerRotation)) 256 pulses per rotation; ??? distance per rotation (compute ths from gear ratios and ),

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

void Elevator::LevelUp()
{
	elevatorEncoder.Reset();
	if (elevatorEncoder.Get() < 42)
	{
		rightElevator.SetSpeed(0.5);
		leftElevator.SetSpeed(0.5);
	}
	else if (elevatorEncoder.Get() >= 42)
	{
		rightElevator.SetSpeed(0.0);
		leftElevator.SetSpeed(0.0);
	}
}

void Elevator::LevelDown()
{

}

void Elevator::Execute()
{
	//This will have to keep track while the elevator is reseting whatching for the limit switch, once it hits the switch then reset the encoder and pid, then set the pid setpoint to 0 and enable it
}

void Elevator::Reset()
{
//Send the motor down slow-ishly use execute function to check for hitting the lower limit switch
//This would need to be called before your first go to command, so you should keep track of whether or not it has been initialized with a boolean, and in the set routine call reset if it hasn't been done
}

void Elevator::SetLevel()
{
	//Make sure the elevator has been reset, if it hasn't, do that instead and just return from this after calling it

	//Then, make sure the level exists, because reasons, or just use a switch case statement to map level number to setpoint value
	//your go up level and go down level functions should call this, to do that, just keep track of the current desired level, and add/subtract a level and call this function with the new desired level
	//If you do incremental level up/down, add feedback leds to robot to indicate what level it is currently trying to go to, otherwise you should just use a button for each level.
}

/*
 * You'll also want a get desired level function and a is at desied level function
 * Ignore D, unless someone helps with tuning
 * Set Level, rather than Up + Down;
 *
 */
