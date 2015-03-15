/*
 * Lift.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#include "main.h"

/**
 * Returns a new Lift object.
 */
Lift initLift(PantherMotor leftMotor, PantherMotor rightMotor,
		Pot leftPot, Pot rightPot)
{
	Lift newLift = {leftMotor, rightMotor, leftPot, rightPot};
	return newLift;
}

/**
 * Stops the lift at the bottom of its travel and slows it down as it
 * approaches the top of its travel.
 *
 * This function probably should not be used outside of Lift.c.
 */
int liftSafety(int desiredSpeed, int potValue, int max)
{
	const int MIN_HEIGHT = 0;
	const int SLOW_HEIGHT = 1000;

	printf("Value: %d", potValue);

	if(potValue < MIN_HEIGHT)
	{
		puts("Lower Limit");
		return limit(desiredSpeed, 127, 0);
	}
	else if(potValue > SLOW_HEIGHT && potValue < max)
	{
		puts("Slow");
		return limit(desiredSpeed, 40, -127);
	}
	else if(potValue > max)
	{
		puts("Upper Limit");
		printf("Returns: %d", limit(127, 0, -127));
		return limit(desiredSpeed, 0, -127);
	}
	else
	{
		return desiredSpeed;
	}
}

/**
 * Lifts at a specified speed including safeties.
 */
void liftAtSpeed(Lift lift, int speed)
{
	//10000 to bypass max height safety
	int leftSpeed = liftSafety(speed, getPot(lift.leftPot), 10000);
	int rightSpeed = liftSafety(speed, getPot(lift.rightPot), 10000);

	leftSpeed = limit(leftSpeed, 127, -127);
	rightSpeed = limit(rightSpeed, 127, -127);

	printf("Left Speed: %d\n", leftSpeed);
	printf("Right Speed: %d\n", rightSpeed);

	setPantherMotor(lift.leftMotor, leftSpeed);
	setPantherMotor(lift.rightMotor, rightSpeed);
}

/**
 * Returns the error (pv - sp). Enter 0 for default deadband of 25.
 */
/*int liftToHeight(Lift lift, int heightSP, int deadBand)
{
	deadBand = (deadBand == 0) ? 25 : deadBand;

	int heightPV = (getPot(lift.leftPot) + getPot(lift.rightPot))/2;
	int error = heightPV - heightSP;

	if(inDeadBand(error, 0, deadBand)) liftAtSpeed(lift, 0);
	else if(error > 0) liftAtSpeed(lift, -100);
	else liftAtSpeed(lift, 100);

	return error;
}*/
