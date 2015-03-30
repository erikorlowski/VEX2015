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
	int leftSpeed = limit(speed, 127, -127);
	int rightSpeed = limit(speed, 127, -127);

	setPantherMotor(pickup.leftMotor, leftSpeed);
	setPantherMotor(pickup.rightMotor, rightSpeed);
}

void pickupIn(Pickup pickup)
{
	runPickup(pickup, 127);
}

void pickupOut(Pickup pickup)
{
	runPickup(pickup, -127);
}

void pickupStop(Pickup pickup)
{
	runPickup(pickup, 0);
}
