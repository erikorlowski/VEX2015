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

}typedef DriveToWayPoint;

#endif /* DRIVETOWAYPOINT_H_ */
