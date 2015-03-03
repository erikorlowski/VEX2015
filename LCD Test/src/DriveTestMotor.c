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
DriveTestMotor initDriveTestMotor(TestMotor motor, int speed, long int length)
{
	DriveTestMotor newStepInfo = {length, motor, speed, 0};
	return newStepInfo;
}

/**
 * Drives the robot at the speed dictated in the step variable and quits
 * the step if the time has elapsed.
 */
void driveTestMotor(DriveTestMotor *stepInfo)
{
	moveTestMotor((*stepInfo).motor, (*stepInfo).speed);

	if(autonomousInfo.elapsedTime > (*stepInfo).length)
	{
		moveTestMotor((*stepInfo).motor, 0);
		(*stepInfo).isFinished = 1;
	}
}
