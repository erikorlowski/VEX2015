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
DriveForTime initDriveForTime(Drive drive, int direction, int magnitude,
		int rotation, long int length)
{
	DriveForTime newStepInfo = {length, drive, direction, magnitude,
			rotation, 0};
	return newStepInfo;
}

/**
 * Drives the robot at the speed dictated in the step variable and quits
 * the step if the time has elapsed.
 */
void driveForTime(DriveForTime *stepInfo)
{
	holonomicDrive((*stepInfo).drive, (*stepInfo).direction,
			(*stepInfo).magnitude, (*stepInfo).rotation, 0);

	if(autonomousInfo.elapsedTime > (*stepInfo).length)
	{
		holonomicDrive((*stepInfo).drive, 0, 0, 0, 0);
		(*stepInfo).isFinished = 1;
	}
}
