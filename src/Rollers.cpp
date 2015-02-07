/*
 * Rollers.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Bill
 */
#include "Rollers.h"

Rollers::Rollers(int roller_extend, int roller_retract, int roller_right_motor, int roller_left_motor, float roller_speed):
rollersOpen(roller_extend),
rollersClose(roller_retract),
rollerPolarRight(roller_right_motor),
rollerPolarLeft(roller_left_motor),
rollerSpeed(roller_speed)
{

}

void Rollers::OpenRollers()
{
	rollersOpen.Set(true);
	rollersClose.Set(false);
}

void Rollers::CloseRollers()
{
	rollersOpen.Set(false);
	rollersClose.Set(true);
}

void Rollers::Eat()
{
	rollerPolarRight.SetSpeed(rollerSpeed);
	rollerPolarLeft.SetSpeed(rollerSpeed);
}

void Rollers::Barf()
{
	rollerPolarRight.SetSpeed(-rollerSpeed);
	rollerPolarLeft.SetSpeed(-rollerSpeed);
}

void Rollers::PushRight()
{
	rollerPolarRight.SetSpeed(-rollerSpeed);
	rollerPolarLeft.SetSpeed(rollerSpeed);
}

void Rollers::PushLeft()
{
	rollerPolarRight.SetSpeed(-rollerSpeed);
	rollerPolarLeft.SetSpeed(rollerSpeed);
}
