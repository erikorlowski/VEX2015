#include "main.hpp"

Drive initDrive(PantherMotor frontLeft, PantherMotor frontRight,
		PantherMotor rearLeft, PantherMotor rearRight)
{
	Drive newDrive = {frontLeft, frontRight, rearLeft, rearRight};

	return newDrive;
}

void holonomicDrive(Drive drive, int x, int y, int rotation)
{
	int frontLeft;
	int frontRight;
	int rearLeft;
	int rearRight;

	frontLeft = y - x + rotation;
	frontRight = y + x - rotation;
	rearLeft = y + x + rotation;
	rearRight = y - x - rotation;

	if(drive.frontLeft.inverted) frontLeft *= -1;
	if(drive.frontRight.inverted) frontRight *= -1;
	if(drive.rearLeft.inverted) rearLeft *= -1;
	if(drive.rearRight.inverted) rearRight *= -1;

	frontLeft = limit(frontLeft, 127, -127);
	frontRight = limit(frontRight, 127, -127);
	rearLeft = limit(rearLeft, 127, -127);
	rearRight = limit(rearRight, 127, -127);

	motorSet(drive.frontLeft.port, frontLeft);
	motorSet(drive.frontRight.port, frontRight);
	motorSet(drive.rearLeft.port, rearLeft);
	motorSet(drive.rearRight.port, rearRight);
}
