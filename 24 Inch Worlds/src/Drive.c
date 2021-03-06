#include "main.h"

/**
 * Initializes a new drive object composed of two PantherMotors.
 */
Drive initDrive(PantherMotor frontLeftMotor, PantherMotor frontRightMotor,
		PantherMotor rearLeftMotor, PantherMotor rearRightMotor,
		IME frontLeftIME, IME frontRightIME, IME rearLeftIME,
		IME rearRightIME, Gyro gyro)
{
	Drive newDrive = {frontLeftMotor, frontRightMotor, rearLeftMotor,
		rearRightMotor, frontLeftIME, frontRightIME, rearLeftIME,
		rearRightIME, gyro, gyroGet(gyro)};

	return newDrive;
}

/**
 * Drives a holonomic drive robot at the specified direction, magnitude,
 * and rotation.
 */
void holonomicDrive(Drive drive, int direction, int magnitude, int rotation,
		int useGyro)
{
	if(!useGyro)
	{
		puts("No Gyro");

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
	else
	{
		puts("Using Gyro");
		holonomicDrive(drive, direction, magnitude, getGyroCorrection(drive),
			0);
	}

}

int getGyroCorrection(Drive drive)
{
	double kP = -3.81;

	int error = getAngle(drive) - drive.headingSetPoint;

	if(inDeadBand(error, 0, 4)) return 0;
	else return (int) (error * kP);
}

int getAngle(Drive drive)
{
	return gyroGet(drive.gyro);
}

void updateGyroSetPoint(Drive *drive)
{
	(*drive).headingSetPoint = getAngle(*drive);
}
