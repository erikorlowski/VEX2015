/*
 * Pickup.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#ifndef PICKUP_H_
#define PICKUP_H_

/**
 * The data structure for a Pickup. Currently includes two motors.
 */
struct Pickup{

	PantherMotor leftMotor;
	PantherMotor rightMotor;

}typedef Pickup;

Pickup initPickup(PantherMotor leftMotor, PantherMotor rightMotor);

void runPickup(Pickup pickup, int speed);
void pickupIn(Pickup pickup);
void pickupOut(Pickup pickup);
void pickupStop(Pickup pickup);

Pickup pickup;

#endif /* PICKUP_H_ */
