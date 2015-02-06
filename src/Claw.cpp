/*
 * Claw.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Bill
 */
#include "Claw.h"

Claw::Claw(int right_claw_extend, int right_claw_retract, int left_claw_extend, int left_claw_retract):
rightClawExtend(right_claw_extend),
rightClawRetract(right_claw_retract),
leftClawExtend(left_claw_extend),
leftClawRetract(left_claw_retract)
{

}

void Claw::OpenClaw()
{
	rightClawExtend.Set(true);
	leftClawExtend.Set(true);
	rightClawRetract.Set(false);
	leftClawRetract.Set(false);
}

void Claw::CloseClaw()
{
	rightClawExtend.Set(false);
	leftClawExtend.Set(false);
	rightClawRetract.Set(true);
	leftClawRetract.Set(true);
}

