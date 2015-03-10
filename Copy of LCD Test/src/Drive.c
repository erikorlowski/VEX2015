#include "main.h"

/**
 * Initializes a new drive object composed of two PantherMotors.
 */
Drive initDrive(PantherMotor leftMotor, PantherMotor rightMotor)
{
	Drive newDrive = {leftMotor, rightMotor};

	return newDrive;
}

/**
 * Drives a two wheel drive robot at the left speed and right speed.
 */
void tankDrive(Drive drive, int leftSpeed, int rightSpeed)
{
	int leftSpeedToFeed = limit(leftSpeed, 127, -127);
	int rightSpeedToFeed = limit(rightSpeed, 127, -127);

	setPantherMotor(drive.leftMotor, leftSpeedToFeed);
	setPantherMotor(drive.rightMotor, rightSpeedToFeed);
}
