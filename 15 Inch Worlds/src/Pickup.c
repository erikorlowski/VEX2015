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

void clawAtSpeed(Pickup pickup, int speed)
{
	setPantherMotor(pickup.motor, limit(speed, 127, -127));
}

void openPickup(Pickup pickup)
{
	clawAtSpeed(pickup, 100);
}

void closePickup(Pickup pickup)
{
	clawAtSpeed(pickup, -100);
}

void stopPickup(Pickup pickup)
{
	clawAtSpeed(pickup, 0);
}

