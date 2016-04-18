#include <iostream>
#include "giotto.h"
#include "ptides.h"
#include "actors.h"

int main() {
	cout << "Windshield Simulation Program for EE249B Project" << endl;

	// Giotto simulation
	GiottoDirector giottoD = new GiottoDirector();
	cout << "Starting Simulation Using Giotto" << endl;
	// Fill in - initialize stuff and start the director running...

	// Ptides simulation
	PtidesDirector ptidesD = new PtidesDirector();
	cout << "Starting Simulation Using Ptides" << endl;
	// Fill in - initialize stuff and start the director running...

	return 0;
}