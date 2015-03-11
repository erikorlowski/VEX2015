/*
 * Lift.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#include "main.h"

Pickup initPickup(PantherMotor leftMotor, PantherMotor rightMotor)
{
	Pickup newPickup = {leftMotor, rightMotor};
	return newPickup;
}

void runPickup(Pickup pickup, int speed)
{
	int leftSpeed = limit(leftSpeed, 127, -127);
	int rightSpeed = limit(rightSpeed, 127, -127);

	setPantherMotor(lift.leftMotor, leftSpeed);
	setPantherMotor(lift.rightMotor, rightSpeed);
}
