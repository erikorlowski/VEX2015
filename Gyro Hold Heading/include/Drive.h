/*
 * Drive.h
 *
 *  Created on: Feb 25, 2015
 *      Author: Erik
 */

#ifndef DRIVE_H_
#define DRIVE_H_

/**
 * Reference type for a two motor drive.
 */
struct Drive{

	PantherMotor leftMotor;
	PantherMotor rightMotor;
	Gyro gyro;
	int headingSetPoint;

}typedef Drive;

Drive initDrive(PantherMotor, PantherMotor, Gyro);
void tankDrive(Drive, int, int);
void arcadeDrive(Drive drive, int magnitude, int rotation, int useGyro);
int getGyroCorrection(Drive drive);
int getAngle(Drive drive);
void updateGyroSetPoint(Drive *drive);

Drive drive;

#endif /* DRIVE_H_ */
