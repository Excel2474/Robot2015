/*
 * Elevator.h
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Wpilib.h"

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

// v v These values are the corresponding encoder counts for each level.
#define LEVEL_ZERO 0 // (8 inches from floor)
#define LEVEL_ONE 136 // ^(14 in because +6) ^
#define LEVEL_TWO 409// ^ ^
#define LEVEL_THREE 682// ^ ^
#define LEVEL_FOUR 954// ^ ^
#define LEVEL_FIVE 1227// ^ ^
#define LEVEL_SIX 1500// ^ ^
// 1 inch = 22.7234 pulses

class Elevator : public IterativeRobot
{

private:
	Solenoid elevatorExtend;
	Solenoid elevatorRetract;
	Solenoid elevatorBrake;
	Victor elevatorMotor;
	Encoder elevatorEncoder;
	DigitalInput leftLowerLimit;
	DigitalInput rightLowerLimit;
	DigitalInput leftUpperLimit;
	DigitalInput rightUpperLimit;
	// = new DigitalInput(0)
	int destinationLevel;

	PIDController elevatorPid;

public: //Elevator Extend

	Elevator(int elevator_extend, int elevator_retract, int elevator_brake, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_left_limit, int lower_right_limit, int upper_left_limit, int upper_right_limit);
	void ExtendElevator();
	void RetractElevator();
	void Execute();
	void Reset();
	void BrakeOn();
	void BrakeOff();
	void SetLevel(int destinationLevel);
	bool IsAtLevel();
	void TestElevatorMotor(float motorSpeed);

};





#endif /* SRC_ELEVATOR_H_ */
