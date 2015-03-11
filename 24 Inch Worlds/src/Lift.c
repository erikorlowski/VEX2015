/*
 * Lift.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#include "main.h"

Lift initLift(PantherMotor leftMotor, PantherMotor rightMotor,
		Pot leftPot, Pot rightPot)
{
	Lift newLift = {leftMotor, rightMotor, leftPot, rightPot};
	return newLift;
}

int liftSafety(int desiredSpeed, int potValue, int max)
{
	const int MIN_HEIGHT = 200;
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

void liftAtSpeed(Lift lift, int speed)
{
	//10000 to bypass max height safety
	int leftSpeed = liftSafety(speed, getPot(lift.leftPot), 10000);
	int rightSpeed = liftSafety(speed, getPot(lift.rightPot), 10000);

	leftSpeed = limit(leftSpeed, 127, -127);
	rightSpeed = limit(rightSpeed, 127, -127);

	setPantherMotor(lift.leftMotor, leftSpeed);
	setPantherMotor(lift.rightMotor, rightSpeed);
}
