/*
 * Lift.c
 *
 *  Created on: Mar 12, 2015
 *      Author: Erik
 */

#include "main.h"

Lift initLift(PantherMotor topMotor, PantherMotor secondTopMotor,
		PantherMotor middleMotor, PantherMotor secondBottomMotor,
		PantherMotor bottomMotor, Encoder encoder, int limitSwitch)
{
	Lift newLift = {topMotor, secondTopMotor,
			middleMotor, secondBottomMotor, bottomMotor, encoder,
			limitSwitch};
	return newLift;
}

int liftSafety(Lift lift, int desiredSpeed)
{
	if(!digitalRead(lift.limitSwitch))
		return limit(desiredSpeed, 127, 0);
	else return limit(desiredSpeed, 127, -127);
}

void liftAtSpeed(Lift lift, int speed)
{
	int speedToFeed = liftSafety(lift, speed);

	setPantherMotor(lift.bottomMotor, speedToFeed);
	setPantherMotor(lift.secondBottomMotor, speedToFeed);
	setPantherMotor(lift.middleMotor, speedToFeed);
	setPantherMotor(lift.secondTopMotor, speedToFeed);
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

void watchLift(Lift lift)
{
	if(!digitalRead(lift.limitSwitch))
	{
		encoderReset(lift.encoder);
	}
}
