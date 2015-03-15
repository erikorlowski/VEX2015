/*
 * Lift.c
 *
 *  Created on: Mar 12, 2015
 *      Author: Erik
 */

#include "main.h"

Lift initLift(PantherMotor bottomMotor, PantherMotor middleMotor,
		PantherMotor topMotor, Encoder encoder, int limitSwitch)
{
	Lift newLift = {bottomMotor, middleMotor, topMotor, encoder, limitSwitch};
	return newLift;
}

int liftSafety(Lift lift, int desiredSpeed)
{
	if(digitalRead(lift.limitSwitch))
		return limit(desiredSpeed, 127, 0);
	else return limit(desiredSpeed, 127, -127);
}

void liftAtSpeed(Lift lift, int speed)
{
	int speedToFeed = liftSafety(lift, speed);

	setPantherMotor(lift.bottomMotor, speedToFeed);
	setPantherMotor(lift.middleMotor, speedToFeed);
	setPantherMotor(lift.topMotor, speedToFeed);
}

/**
 * Returns the error (pv - sp). Enter 0 for default deadband of 25.
 */
int liftToHeight(Lift lift, int heightSP, int deadBand)
{
	deadBand = (deadBand == 0) ? 25 : deadBand;

	int heightPV = encoderGet(lift.encoder);
	int error = heightPV - heightSP;

	if(inDeadBand(error, 0, deadBand)) liftAtSpeed(lift, 0);
	else if(error > 0) liftAtSpeed(lift, -100);
	else liftAtSpeed(lift, 100);

	return error;
}
