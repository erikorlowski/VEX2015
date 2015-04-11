/*
 * Lift.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#ifndef LIFT_H_
#define LIFT_H_

/**
 * The data type for the Lift. Currently includes two motors and two
 * potentiometers.
 */
struct Lift{

	PantherMotor leftMotor;
	PantherMotor rightMotor;
	Pot leftPot;
	Pot rightPot;
	int limitSwitch;

}typedef Lift;

Lift initLift(PantherMotor leftMotor, PantherMotor rightMotor,
		Pot leftPot, Pot rightPot, int limitSwitch);

void liftAtSpeed(Lift lift, int speed);
int liftToHeight(Lift lift, int heightSP, int deadBand);
int getLiftHeight(Lift lift);

Lift lift;

#endif /* LIFT_H_ */
