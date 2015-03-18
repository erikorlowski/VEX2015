/*
 * DriveToWayPoint.h
 *
 *  Created on: Mar 15, 2015
 *      Author: Erik
 */

#ifndef DRIVETOWAYPOINT_H_
#define DRIVETOWAYPOINT_H_

struct DriveToWayPoint{

	Drive drive;
	double direction;
	double magnitude;
	int rotation;
	int maxSpeed;
	double gyroStart;

}typedef DriveToWayPoint;

DriveToWayPoint initDriveToWayPoint(Drive drive, double direction,
		double magnitude, int rotation, int maxSpeed);
int getAxisCorrection(int error, int maxSpeed, int deadBand);
void driveToWayPoint(DriveToWayPoint *stepInfo);

#endif /* DRIVETOWAYPOINT_H_ */
