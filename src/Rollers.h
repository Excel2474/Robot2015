/*
 * Rollers.h
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */

#ifndef SRC_ROLLERS_H_
#define SRC_ROLLERS_H_
#include "Wpilib.h"

class Rollers : public IterativeRobot
{
private:
	Solenoid rollersOpen;
	Solenoid rollersClose;
	Victor rollerPolarRight;
	Victor rollerPolarLeft;
	float rollerSpeed;

public:
Rollers(int rollers_extend, int rollers_retract, int roller_right_motor, int roller_left_motor, float roller_speed);
void OpenRollers();
void CloseRollers();
void Eat();
void Barf();
void PushRight();
void PushLeft();

};



#endif /* SRC_ROLLERS_H_ */
