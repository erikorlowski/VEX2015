/*
 * Lift.h
 *
 *  Created on: Mar 12, 2015
 *      Author: Erik
 */

#ifndef LIFT_H_
#define LIFT_H_

struct Lift{

	PantherMotor bottomMotor;
	PantherMotor middleMotor;
	PantherMotor topMotor;

	Encoder encoder;

	int limitSwitch;

}typedef Lift;

int liftSafety(Lift lift, int desiredSpeed);
void liftAtSpeed(Lift lift, int speed);
Lift initLift(PantherMotor bottomMotor, PantherMotor middleMotor,
		PantherMotor topMotor, Encoder encoder, int limitSwitch);

Lift lift;

#endif /* LIFT_H_ */
