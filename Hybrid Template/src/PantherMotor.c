/*
 * Motor.c
 *
 *  Created on: Feb 20, 2015
 *      Author: Erik
 */
#include "main.hpp"

PantherMotor initPantherMotor(int port, int inverted)
{
	PantherMotor newPantherMotor = {port, inverted};
	return newPantherMotor;
}

void setPantherMotor(PantherMotor motor, int speed)
{
	motorSet(motor.port, (motor.inverted) ? -speed : speed);
}

int getPantherMotor(PantherMotor motor)
{
	return (motor.inverted) ? -motorGet(motor.port) : motorGet(motor.port);
}
