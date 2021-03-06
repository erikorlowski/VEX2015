/*
 * AutoLiftToHeight.c
 *
 *  Created on: Mar 18, 2015
 *      Author: Erik
 */

#include "main.h"

AutoLiftToHeight initAutoLiftToHeight(int height)
{
	AutoLiftToHeight newStep = {height, 0};
	return newStep;
}

void autoLiftToHeight(AutoLiftToHeight *stepInfo)
{
	int error = liftToHeight((*stepInfo).desiredHeight, 0);

	(*stepInfo).isFinished = (abs(error) < 25);
}
