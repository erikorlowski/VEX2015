/*
 * Pickup.h
 *
 *  Created on: Mar 15, 2015
 *      Author: Erik
 */

#ifndef PICKUP_H_
#define PICKUP_H_

struct Pickup{

	PantherMotor motor;

}typedef Pickup;

void pickupAtSpeed(Pickup, int speed);
void openPickup(Pickup);
void closePickup(Pickup);
void stopPickup(Pickup);

Pickup initPickup(PantherMotor motor);

Pickup pickup;

#endif /* PICKUP_H_ */
