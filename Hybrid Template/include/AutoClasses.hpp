/*
 * AutoClasses.hpp
 *
 *  Created on: Feb 21, 2015
 *      Author: Erik
 */

#ifndef AUTOCLASSES_HPP_
#define AUTOCLASSES_HPP_

#include "main.hpp"

class Step
{
	public:
		virtual void start();
		virtual void excectute();
		virtual bool isFinished();
		virtual void kill();
};

extern int testVariable;

#endif /* AUTOCLASSES_HPP_ */
