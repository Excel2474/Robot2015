/*
 * Claw.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Bill
 */
#include "Wpilib.h"

#ifndef SRC_CLAW_H_
#define SRC_CLAW_H_

class Claw : public IterativeRobot
{
private:
	Solenoid clawExtend;
	Solenoid clawRetract;

public: //Claw Functions
	Claw(int claw_extend, int claw_retract);
	void OpenClaw();
	void CloseClaw();

};






#endif /* SRC_CLAW_H_ */
