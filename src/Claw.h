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
	Solenoid rightClawExtend;
	Solenoid rightClawRetract;
	Solenoid leftClawExtend;
	Solenoid leftClawRetract;

public: //Claw Functions
	Claw(int right_claw_extend, int right_claw_retract, int left_claw_extend, int left_claw_retract);
	void OpenClaw();
	void CloseClaw();

};






#endif /* SRC_CLAW_H_ */
