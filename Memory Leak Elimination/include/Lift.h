/*
 * Lift.h
 *
 *  Created on: Mar 12, 2015
 *      Author: Erik
 */

#ifndef LIFT_H_
#define LIFT_H_

struct Lift{

	PantherMotor topMotor;
	PantherMotor secondTopMotor;
	PantherMotor middleMotor;
	PantherMotor secondBottomMotor;
	PantherMotor bottomMotor;

	Encoder encoder;

	int limitSwitch;

}typedef Lift;

int liftSafety(int desiredSpeed);
void liftAtSpeed(int speed);
int liftToHeight(int heightSP, int deadBand);
void watchLift();
Lift initLift(PantherMotor topMotor, PantherMotor secondTopMotor,
	PantherMotor middleMotor, PantherMotor secondBottomMotor,
	PantherMotor bottomMotor, Encoder encoder, int limitSwitch);

Lift lift;

#endif /* LIFT_H_ */
