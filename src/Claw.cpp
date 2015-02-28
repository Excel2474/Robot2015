/*
 * Claw.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Bill
 */
#include "Claw.h"

Claw::Claw(int claw_extend, int claw_retract):
clawExtend(1, claw_extend),
clawRetract(1, claw_retract)
{

}

void Claw::OpenClaw()
{
	clawExtend.Set(true);
	clawRetract.Set(false);
}

void Claw::CloseClaw()
{
	clawExtend.Set(false);
	clawRetract.Set(true);
}

