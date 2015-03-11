/*
 * Pot.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Erik
 */

#include "main.h"

Pot initPot(int channel, int inverted)
{
	Pot newPot = {channel, inverted};
	return newPot;
}

int getPot(Pot pot)
{
	return (pot.inverted) ? (4095 - analogRead(pot.channel))
			: analogRead(pot.channel);
}
