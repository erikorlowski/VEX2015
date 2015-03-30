/*
 * Lift.c
 *
 *  Created on: Mar 12, 2015
 *      Author: Erik
 */

#include "main.h"

Pickup initPickup(PantherMotor motor)
{
	Pickup newPickup = {motor};
	return newPickup;
}

void clawAtSpeed(int speed)
{
	setPantherMotor(pickup.motor, limit(speed, 127, -127));
}

void openPickup()
{
	clawAtSpeed(127);
}

void closePickup()
{
	clawAtSpeed(-100);
}

void stopPickup()
{
	clawAtSpeed(0);
}

