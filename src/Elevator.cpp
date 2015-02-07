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
elevatorEncoder(elevator_encoder_A, elevator_encoder_B, true)
{

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

