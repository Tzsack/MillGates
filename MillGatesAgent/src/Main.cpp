/*
 * Main.cpp
 *
 *  Created on: Apr 21, 2018
 *      Author: Luca
 */

#include <stdio.h>
#include "State.h"
#include "connection.h"

int main(void) {

	printf("MillGatesAgent \n");

	State * s = new State();

	s->setPawnAt(1, 2, 0, PAWN_BLACK);
	s->setPawnAt(1, 2, 2, PAWN_WHITE);

	printf("%s\n",s->toString());

	State * s2 = s->clone();

	printf("%s\n",s2->toString());

	delete s2;

	printf("%s\n",s->toString());

	start_connection();

	send_data(s->toString(), 7);

}

