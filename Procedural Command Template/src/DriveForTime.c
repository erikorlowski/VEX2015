/*
 * DriveForTime.c
 *
 *  Created on: Feb 21, 2015
 *      Author: Erik
 */

#include "main.h"

DriveForTime initDriveForTime(Drive drive, int x, int y, int rotation, long int length)
{
	DriveForTime newStepInfo = {length, drive, x, y, rotation};
	return newStepInfo;
}

void driveForTime(DriveForTime *stepInfo)
{
	holonomicDrive((*stepInfo).drive, (*stepInfo).x, (*stepInfo).y,
		(*stepInfo).rotation);

	if(autonomousInfo.elapsedTime > (*stepInfo).length)
	{
		holonomicDrive((*stepInfo).drive, 0, 0, 0);
		autonomousInfo.isFinished = 1;
	}
}
