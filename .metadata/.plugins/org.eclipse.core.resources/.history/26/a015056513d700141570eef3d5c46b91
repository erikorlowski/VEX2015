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
			rotation, maxSpeed, 0.0};
	return newDriveToWayPoint;
}

int getAxisCorrection(int error, int maxSpeed, int deadBand)
{
	if(inDeadBand(error, 0, deadBand)) return 0;
	else if(error > 0) return -maxSpeed;
	else return maxSpeed;
}

void driveToWayPoint(DriveToWayPoint *stepInfo)
{
	if(autonomousInfo.lastStep != autonomousInfo.step)
	{
		imeReset((*stepInfo).drive.frontLeftIME);
		imeReset((*stepInfo).drive.frontRightIME);
		imeReset((*stepInfo).drive.rearLeftIME);
		imeReset((*stepInfo).drive.rearRightIME);

		(*stepInfo).gyroStart = gyroGet((*stepInfo).drive.gyro);
	}

	int directionRevs = holonomicInchesToIME((*stepInfo).direction, 4);
	int magnitudeRevs = holonomicInchesToIME((*stepInfo).magnitude, 4);

	int frontLeftDistance;
	int frontRightDistance;
	int rearLeftDistance;
	int rearRightDistance;

	imeGet((*stepInfo).drive.frontLeftIME, &frontLeftDistance);
	imeGet((*stepInfo).drive.frontRightIME, &frontRightDistance);
	imeGet((*stepInfo).drive.rearLeftIME, &rearLeftDistance);
	imeGet((*stepInfo).drive.rearRightIME, &rearRightDistance);

	int directionPV = (frontLeftDistance -frontRightDistance -
			rearLeftDistance +rearRightDistance)/4;
	int magnitudePV = (frontLeftDistance + frontRightDistance +
			rearLeftDistance + rearRightDistance)/4;

	int directionError = directionPV - directionRevs;
	int magnitudeError = magnitudePV - magnitudeRevs;
	int rotationError = gyroGet((*stepInfo).drive.gyro) -
			(*stepInfo).gyroStart - (*stepInfo).rotation;

	holonomicDrive((*stepInfo).drive,
			getAxisCorrection(directionError, (*stepInfo).maxSpeed, 10),
			getAxisCorrection(magnitudeError, (*stepInfo).maxSpeed, 10),
			getAxisCorrection(rotationError, (*stepInfo).maxSpeed, 4), 0);
}
