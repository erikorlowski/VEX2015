/*
 * Drive.h
 *
 *  Created on: Feb 25, 2015
 *      Author: Erik
 */

#ifndef DRIVE_H_
#define DRIVE_H_

#include "main.h"

/**
 * Reference type for a two motor drive.
 */
struct Drive{

	PantherMotor frontLeftMotor;
	PantherMotor frontRightMotor;
	PantherMotor rearLeftMotor;
	PantherMotor rearRightMotor;

	IME frontLeftIME;
	IME frontRightIME;
	IME rearLeftIME;
	IME rearRightIME;

	Gyro gyro;
	int headingSetPoint;

}typedef Drive;

Drive initDrive(PantherMotor frontLeftMotor, PantherMotor frontRightMotor,
		PantherMotor rearLeftMotor, PantherMotor rearRightMotor,
		IME frontLeftIME, IME frontRightIME, IME rearLeftIME,
		IME rearRightIME, Gyro gyro);
void holonomicDrive(Drive, int, int, int, int);
int getGyroCorrection(Drive drive);
int getAngle(Drive drive);
void updateGyroSetPoint(Drive *drive);

Drive drive;

#endif /* DRIVE_H_ */
