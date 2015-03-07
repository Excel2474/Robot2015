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
#define LEVEL_ZERO .25*29.15 // (8 inches from floor)
#define LEVEL_ONE 2.5*29.15 // ^(14 in because +6) ^
#define LEVEL_TWO 7*29.15// ^ ^
#define LEVEL_THREE 10.5*29.15// ^ ^
#define LEVEL_FOUR 16*29.15// ^ ^
#define LEVEL_FIVE 19*29.15// ^ ^
#define LEVEL_SIX 24.5*29.15// ^ ^
#define LEVEL_SEVEN 31.25*29.15// ^ ^
#define MAX_LEVEL 7// ^ ^
#define MIN_LEVEL 0
// 1 inch = 29.15 pulses

class Elevator : public IterativeRobot
{

private:
	Solenoid elevatorExtend;
	Solenoid elevatorRetract;
	Solenoid elevatorBrakeExtend;
	Solenoid elevatorBrakeRetract;
	Victor elevatorMotor;
	Encoder elevatorEncoder;
	DigitalInput leftLowerLimit;
	DigitalInput rightLowerLimit;
	DigitalInput leftUpperLimit;
	DigitalInput rightUpperLimit;
	// = new DigitalInput(0)
	int destinationLevel;
	int destinationFloor;
	bool isCountSet;
	bool isStopChecked;
	//float destinationPulse;
	PIDController elevatorPid;

public: //Elevator Extend

	Elevator(int elevator_extend, int elevator_retract, int elevator_brake_extend, int elevator_brake_retract, int elevator_motor, int elevator_encoder_A, int elevator_encoder_B, int lower_left_limit, int lower_right_limit, int upper_left_limit, int upper_right_limit);
	void ExtendElevator();
	void RetractElevator();
	void Execute();
	void Reset();
	void BrakeOn();
	void BrakeOff();
	void TwitchFromDown();
	void SetLevel(int destinationLevel);
	bool IsAtLevel();
	void TestElevatorMotor(float motorSpeed);
	void DisablePid();
	bool IsCrashing();


	void SetPID(float p, float i, float d){elevatorPid.SetPID(p,i,d);}

	int maxLevel();
	int minLevel();
	double PID(double max_out, double min_out, double m_setpoint);
	void stopPID();
	bool setPID(double max_out, double min_out, double setpoint);
	float SetHeight(int destinationLevel);
	bool AtPosition(int setpoint);
	void ResetPID();
	bool ToplimitHit();
	bool BottomlimitHit();
	bool Calibrate();
	//Elevator PID Initial Values
	double p_error=0.0;
	double d_error=0.0;
	double i_error=0.0;
	double error_prev=0.0;
//	float t0 = 0.0;
//	float dt = 0.0;
//	float error_deriv=0;
	double PID_out=0;
	double elevatormotorspeed;
};





#endif /* SRC_ELEVATOR_H_ */
