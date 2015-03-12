/*
 * Lift.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#include "main.h"

/**
 * Returns an initialized Pickup object.
 */
Pickup initPickup(PantherMotor leftMotor, PantherMotor rightMotor)
{
	Pickup newPickup = {leftMotor, rightMotor};
	return newPickup;
}

/**
 * Runs the pickup at a specified speed.
 */
void runPickup(Pickup pickup, int speed)
{
	int leftSpeed = limit(leftSpeed, 127, -127);
	int rightSpeed = limit(rightSpeed, 127, -127);

	setPantherMotor(lift.leftMotor, leftSpeed);
	setPantherMotor(lift.rightMotor, rightSpeed);
}
