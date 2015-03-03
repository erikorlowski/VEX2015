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

}typedef Drive;

Drive initDrive(PantherMotor, PantherMotor);
void tankDrive(Drive, int, int);

Drive drive;

#endif /* DRIVE_H_ */
