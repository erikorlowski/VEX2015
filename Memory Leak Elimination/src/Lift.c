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

int liftSafety(int desiredSpeed)
{
	/*if(!digitalRead(lift.limitSwitch))
		return limit(desiredSpeed, 127, 0);
	else return limit(desiredSpeed, 127, -127);*/
	return limit(desiredSpeed, 127, -127);
}

void liftAtSpeed(int speed)
{
	int speedToFeed = liftSafety(speed);

	setPantherMotor(lift.bottomMotor, speedToFeed);
	setPantherMotor(lift.secondBottomMotor, speedToFeed);
	setPantherMotor(lift.middleMotor, speedToFeed);
	setPantherMotor(lift.secondTopMotor, speedToFeed);
	setPantherMotor(lift.topMotor, speedToFeed);
}

/**
 * Returns the error (pv - sp). Enter 0 for default deadband of 25.
 */
int liftToHeight(int heightSP, int deadBand)
{
	/*deadBand = (deadBand == 0) ? 25 : deadBand;

	int heightPV = encoderGet(lift.encoder);
	int error = heightPV - heightSP;

	if(inDeadBand(error, 0, deadBand)) liftAtSpeed(0);
	else if(error > 0) liftAtSpeed(-100);
	else liftAtSpeed(100);

	return error;*/

	return 0;
}

void watchLift()
{
	/*if(!digitalRead(lift.limitSwitch))
	{
		encoderReset(lift.encoder);
	}*/
}
