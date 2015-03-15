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

int liftSafety(Lift lift, int desiredSpeed);
void liftAtSpeed(Lift lift, int speed);
Lift initLift(PantherMotor topMotor, PantherMotor secondTopMotor,
	PantherMotor middleMotor, PantherMotor secondBottomMotor,
	PantherMotor bottomMotor, Encoder encoder, int limitSwitch);

Lift lift;

#endif /* LIFT_H_ */
