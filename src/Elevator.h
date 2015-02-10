/*
 * Elevator.h
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Wpilib.h"

#ifndef SRC_ELEVATOR_H_
#define SRC_ELEVATOR_H_

class Elevator : public IterativeRobot
{


private:
	Solenoid elevatorExtend;
	Solenoid elevatorRetract;
	Victor elevatorMotor;
	Encoder elevatorEncoder;
	DigitalInput lowerLimit;
	DigitalInput upperLimit;
	// = new DigitalInput(0)

	PIDController elevatorPid;

public: //Elevator Extend

	Elevator(int elevator_extend, int elevator_retract, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_limit, int upper_limit);
	void ExtendElevator();
	void RetractElevator();
	void Execute();
	void Reset();
	void SetLevel();

};





#endif /* SRC_ELEVATOR_H_ */
