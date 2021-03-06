/*
 * Pot.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#include "main.h"

/**
 * Returns an initialized Pot object.
 */
Pot initPot(int channel, int inverted)
{
	Pot newPot = {channel, inverted};
	return newPot;
}

/**
 * Returns the value of a potentiometer from 0 to 4095 factoring in any
 * inversions.
 */
int getPot(Pot pot)
{
	return (pot.inverted) ? (4095 - analogRead(pot.channel))
			: analogRead(pot.channel);
}
