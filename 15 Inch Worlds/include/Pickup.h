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

void pickupAtSpeed(int speed);
void openPickup();
void closePickup();
void stopPickup();

Pickup initPickup(PantherMotor motor);

Pickup pickup;

#endif /* PICKUP_H_ */
