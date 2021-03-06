/*
 * Auto.h
 *
 *  Created on: Feb 25, 2015
 *      Author: Erik
 */

#ifndef AUTO_H_
#define AUTO_H_

/**
 * Reference type for information about the progress of autonomous mode.
 * the only instance of this type that should be used is autonomousInfo.
 */
struct AutonomousInfo{

	int step;
	int lastStep;
	int elapsedTime;
	int isFinished;

}typedef AutonomousInfo;

/**
 * Struct containing information about the autonomous mode.
 */
AutonomousInfo autonomousInfo;

/**
 * Refence type for driving at a set speed based on time.
 */
struct DriveForTime{

	long int length;
	Drive drive;
	int leftSpeed;
	int rightSpeed;
	int isFinished;

}typedef DriveForTime;

DriveForTime initDriveForTime(Drive, int, int, long int);
void driveForTime(DriveForTime *stepInfo);

struct DriveTestMotor{

	long int length;
	TestMotor motor;
	int speed;
	int isFinished;

}typedef DriveTestMotor;

DriveTestMotor initDriveTestMotor(TestMotor, int, long int);
void driveTestMotor(DriveTestMotor*);

#endif /* AUTO_H_ */
