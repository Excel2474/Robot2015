/*
 * Elevator.h
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Wpilib.h"

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

// v v These values will be the corresponding encoder counts for each level. We don't know them yet.
#define LEVEL_ZERO 0 // Except maybe this one; the encoder count for the very lowest level is probably 0--right? (8 inches from floor)
#define LEVEL_ONE 136 // ^(14 in, +6) ^
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
	DigitalInput lowerLimit;
	DigitalInput upperLimit;
	// = new DigitalInput(0)
	int destinationLevel;

	PIDController elevatorPid;

public: //Elevator Extend

	Elevator(int elevator_extend, int elevator_retract, int elevator_brake, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_limit, int upper_limit);
	void ExtendElevator();
	void RetractElevator();
	void Execute();
	void Reset();
	void BrakeOn();
	void BrakeOff();
	void SetLevel(int destinationLevel);
	bool IsAtLevel();
	void TestElevatorMotor(int motorSpeed);

};





#endif /* SRC_ELEVATOR_H_ */
