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

int OIDisableDriveGyro()
{
	return joystickGetDigital(1, 6, JOY_DOWN);
}

int OIEnableDriveGyro()
{
	return joystickGetDigital(1, 6, JOY_UP);
}

/**
 * Returns the output from the joystick of if the lift should be moving
 * up.
 *
 * Note: Assumes digital control of the lift.
 */
int OIGetLiftUp()
{
	return joystickGetDigital(2, 7, JOY_UP);
}

/**
 * Returns the output from the joystick of if the lift should be moving
 * down.
 *
 * Note: Assumes digital control of the lift.
 */
int OIGetLiftDown()
{
	return joystickGetDigital(2, 7, JOY_DOWN);
}

/**
 * Returns the output from the joystick of if the pickup should be running
 * inwards.
 */
int OIGetPickupIn()
{
	return joystickGetDigital(1, 5, JOY_UP);
}

/**
 * Returns the output from the joystick of if the pickup should be running
 * outwards.
 */
int OIGetPickupOut()
{
	return joystickGetDigital(1, 5, JOY_DOWN);
}

int OIGetSpinnerUp()
{
	return joystickGetDigital(2,5,JOY_UP);
}

int OIGetSpinnerDown()
{
	return joystickGetDigital(2,5,JOY_DOWN);
}

int OIGetLiftToLowPost()
{
	return joystickGetDigital(2,8,JOY_DOWN);
}

int OIGetLiftToMediumPost()
{
	return joystickGetDigital(2,8,JOY_RIGHT);
}

int OIGetLiftToHighPost()
{
	return joystickGetDigital(2,8,JOY_UP);
}
