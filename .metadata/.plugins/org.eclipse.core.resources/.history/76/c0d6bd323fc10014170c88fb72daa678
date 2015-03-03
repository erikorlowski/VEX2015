/*
 * DriveForTime.c
 *
 *  Created on: Feb 21, 2015
 *      Author: Erik
 */

#include "main.h"

/**
 * Returns an initialized DriveForTime variable.
 */
DriveForTime initDriveForTime(Drive drive, int leftSpeed, int rightSpeed, long int length)
{
	DriveForTime newStepInfo = {length, drive, leftSpeed, rightSpeed, 0};
	return newStepInfo;
}

/**
 * Drives the robot at the speed dictated in the step variable and quits
 * the step if the time has elapsed.
 */
void driveForTime(DriveForTime *stepInfo)
{
	tankDrive((*stepInfo).drive, (*stepInfo).leftSpeed, (*stepInfo).rightSpeed);

	if(autonomousInfo.elapsedTime > (*stepInfo).length)
	{
		tankDrive((*stepInfo).drive, 0, 0);
		(*stepInfo).isFinished = 1;
	}
}
