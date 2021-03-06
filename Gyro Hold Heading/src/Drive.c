#include "main.h"

/**
 * Initializes a new drive object composed of two PantherMotors.
 */
Drive initDrive(PantherMotor leftMotor, PantherMotor rightMotor, Gyro gyro)
{
	Drive newDrive = {leftMotor, rightMotor, gyro, gyroGet(gyro)};

	return newDrive;
}

/**
 * Drives a two wheel drive robot at the left speed and right speed.
 */
void tankDrive(Drive drive, int leftSpeed, int rightSpeed)
{
	int leftSpeedToFeed = limit(leftSpeed, 127, -127);
	int rightSpeedToFeed = limit(rightSpeed, 127, -127);

	setPantherMotor(drive.leftMotor, leftSpeedToFeed);
	setPantherMotor(drive.rightMotor, rightSpeedToFeed);
}

void arcadeDrive(Drive drive, int magnitude, int rotation, int useGyro)
{
	if(!useGyro) tankDrive(drive, magnitude + rotation, magnitude - rotation);
	else arcadeDrive(drive, magnitude, getGyroCorrection(drive), 0);
}

int getGyroCorrection(Drive drive)
{
	double kP = 3.81;

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
