/*
 * OI.c
 *
 *  Created on: Feb 20, 2015
 *      Author: Erik
 */
#include "main.h"

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

int OIDisableDriveGyro()
{
	return joystickGetDigital(1, 5, JOY_UP);
}

int OIEnableDriveGyro()
{
	return joystickGetDigital(1, 6, JOY_UP);
}
