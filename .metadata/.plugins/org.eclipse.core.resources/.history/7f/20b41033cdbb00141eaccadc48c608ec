#include "main.h"

/**
 * Initializes a new drive object composed of two PantherMotors.
 */
TestMotor initTestMotor(PantherMotor motor)
{
	TestMotor newTestMotor = {motor};

	return newTestMotor;
}

/**
 * Drives a two wheel drive robot at the left speed and right speed.
 */
void moveTestMotor(TestMotor motor, int speed)
{
	int speedToFeed = limit(speed, 127, -127);

	setPantherMotor(motor.motor, speedToFeed);
}
