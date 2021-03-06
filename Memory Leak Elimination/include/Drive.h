/*
 * Drive.h
 *
 *  Created on: Feb 25, 2015
 *      Author: Erik
 */

#ifndef DRIVE_H_
#define DRIVE_H_

#include "IME.h"

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

}typedef Drive;

Drive initDrive(PantherMotor frontLeftMotor, PantherMotor frontRightMotor,
		PantherMotor rearLeftMotor, PantherMotor rearRightMotor,
		IME frontLeftIME, IME frontRightIME, IME rearLeftIME,
		IME rearRightIME, Gyro gyro);
void holonomicDrive(int, int, int);

Drive drive;

#endif /* DRIVE_H_ */
