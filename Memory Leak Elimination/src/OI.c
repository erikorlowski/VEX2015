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

/**
 * Returns the output from the joystick for the speed of the lift.
 */
int OIGetLiftSpeed()
{
	return joystickGetAnalog(2,2);
}

/**
 * Returns the output from the joystick of whether the lift should go to
 * the load height.
 */
int OIGetLiftLoad()
{
	return joystickGetDigital(2,6,JOY_UP);
}

/**
 * Returns the output from the joystick for whether to open the claw.
 */
int OIGetPickupOpen()
{
	return joystickGetDigital(1,5,JOY_DOWN);
}

/**
 * Returns the output from the joystick for whether to close the claw.
 */
int OIGetPickupClose()
{
	return joystickGetDigital(1,6,JOY_DOWN);
}
