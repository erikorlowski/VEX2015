/*
 * Spinner.h
 *
 *  Created on: Mar 15, 2015
 *      Author: Erik
 */

#ifndef SPINNER_H_
#define SPINNER_H_

struct Spinner{

	int servo;

}typedef Spinner;

Spinner initSpinner(int servo);
void spinnerUp(Spinner spinner);
void spinnerDown(Spinner spinner);

Spinner spinner;


#endif /* SPINNER_H_ */
