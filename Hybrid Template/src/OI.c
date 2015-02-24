/*
 * OI.c
 *
 *  Created on: Feb 20, 2015
 *      Author: Erik
 */
#include "main.hpp"

int getDriveX()
{
	return joystickGetAnalog(1,4);
}

int getDriveY()
{
	return joystickGetAnalog(1,3);
}

int getDriveRotation()
{
	return joystickGetAnalog(1,1);
}
