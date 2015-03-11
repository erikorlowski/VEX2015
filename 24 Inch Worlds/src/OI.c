/*
 * OI.c
 *
 *  Created on: Feb 20, 2015
 *      Author: Erik
 */
#include "main.h"

/**
 * Returns the output from the joystick for the direction of the drive.
 */
int OIGetDriveDirection()
{
	return joystickGetAnalog(1,4);
}

/**
 * Returns the output from the joystick for the magnitude of the drive.
 */
int OIGetDriveMagnitude()
{
	return joystickGetAnalog(1,3);
}

/**
 * Returns the output from the joystick for the rotation of the drive.
 */
int OIGetDriveRotation()
{
	return joystickGetAnalog(1,1);
}

int OIGetLiftUp()
{
	return joystickGetDigital(2, 7, JOY_UP);
}

int OIGetLiftDown()
{
	return joystickGetDigital(2, 7, JOY_DOWN);
}

int OIGetPickupIn()
{
	return joystickGetDigital(1, 5, JOY_UP);
}

int OIGetPickupOut()
{
	return joystickGetDigital(1, 5, JOY_DOWN);
}