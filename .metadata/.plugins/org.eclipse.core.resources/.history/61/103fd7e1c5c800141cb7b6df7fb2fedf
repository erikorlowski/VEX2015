/*
 * Lift.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#ifndef LIFT_H_
#define LIFT_H_

struct Lift{

	PantherMotor leftMotor;
	PantherMotor rightMotor;
	Pot leftPot;
	Pot rightPot;

}typedef Lift;

Lift initLift(PantherMotor leftMotor, PantherMotor rightMotor,
		Pot leftPot, Pot rightPot);

void liftAtSpeed(Lift lift, int speed);

Lift lift;

#endif /* LIFT_H_ */
