/*
 * DriveToWayPoint.c
 *
 *  Created on: Mar 15, 2015
 *      Author: Erik
 */

#include "main.h"

//TODO put DriveToWayPoint into main header file
#include "DriveToWayPoint.h"

DriveToWayPoint initDriveToWayPoint(Drive drive, double direction,
		double magnitude, int rotation, int maxSpeed)
{
	DriveToWayPoint newDriveToWayPoint = {drive, direction, magnitude,
			rotation, maxSpeed};
	return newDriveToWayPoint;
}

void driveToWayPoint(DriveToWayPoint *stepInfo)
{

}
