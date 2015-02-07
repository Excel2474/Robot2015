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
	Victor rightElevator;
	Victor leftElevator;
	Encoder elevatorEncoder;

	PIDController elevatorPid;

public: //Elevator Extend

	Elevator(int elevator_extend, int elevator_retract, int right_elevator, int left_elevator, int elevator_encoder_A, int elevator_encoder_B);
	void ExtendElevator();
	void RetractElevator();
	void LevelUp();
	void LevelDown();

};





#endif /* SRC_ELEVATOR_H_ */
