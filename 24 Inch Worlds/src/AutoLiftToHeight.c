/*
 * AutoLiftToHeight.c
 *
 *  Created on: Mar 18, 2015
 *      Author: Erik
 */

#include "main.h"

AutoLiftToHeight initAutoLiftToHeight(Lift lift, int height, int deadBand)
{
	AutoLiftToHeight newStep = {height, 0, lift, deadBand};
	return newStep;
}

void autoLiftToHeight(AutoLiftToHeight *stepInfo)
{
	int error = liftToHeight((*stepInfo).lift, (*stepInfo).desiredHeight, (*stepInfo).deadBand);

	printf("Auto Error: %d\nFinished: %d\n", error, (abs(error) <
			((*stepInfo).deadBand == 0) ? 10 : (*stepInfo).deadBand));

	(*stepInfo).isFinished = (abs(error) < 50);
}
