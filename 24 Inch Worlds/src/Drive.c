#include "main.h"

/**
 * Initializes a new drive object composed of two PantherMotors.
 */
Drive initDrive(PantherMotor frontLeftMotor, PantherMotor frontRightMotor,
		PantherMotor rearLeftMotor, PantherMotor rearRightMotor,
		int frontLeftIME, int frontRightIME, int rearLeftIME,
		int rearRightIME, Gyro gyro)
{
	Drive newDrive = {frontLeftMotor, frontRightMotor, rearLeftMotor,
		rearRightMotor, frontLeftIME, frontRightIME, rearLeftIME,
		rearRightIME, gyro};

	return newDrive;
}

/**
 * Drives a holonomic drive robot at the specified direction, magnitude,
 * and rotation.
 */
void holonomicDrive(Drive drive, int direction, int magnitude, int rotation)
{
	int frontLeft = direction + magnitude + rotation;
	int frontRight = -direction + magnitude - rotation;
	int rearLeft = -direction + magnitude + rotation;
	int rearRight = direction + magnitude - rotation;

	frontLeft = limit(frontLeft, 127, -127);
	frontRight = limit(frontRight, 127, -127);
	rearLeft = limit(rearLeft, 127, -127);
	rearRight = limit(rearRight, 127, -127);

	setPantherMotor(drive.frontLeftMotor, frontLeft);
	setPantherMotor(drive.frontRightMotor, frontRight);
	setPantherMotor(drive.rearLeftMotor, rearLeft);
	setPantherMotor(drive.rearRightMotor, rearRight);
}
