/*
 * DriveForTime.h
 *
 *  Created on: Mar 30, 2015
 *      Author: Erik
 */

#ifndef DRIVEFORTIME_H_
#define DRIVEFORTIME_H_

/**
 * Refence type for driving at a set speed based on time.
 */
struct DriveForTime{

	long int length;
	Drive drive;
	int direction;
	int magnitude;
	int rotation;
	int isFinished;

}typedef DriveForTime;

DriveForTime initDriveForTime(Drive, int, int, int, long int);
void driveForTime(DriveForTime *stepInfo);

#endif /* DRIVEFORTIME_H_ */
