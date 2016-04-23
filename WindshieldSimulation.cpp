#include <iostream>
#include "giotto.h"
#include "ptides.h"
#include "actors.h"
#include "scheduler.h"

int main() {
	cout << "Windshield Simulation Program for EE249B Project" << endl;

	// Start a scheduler
	Scheduler scheduler = new Scheduler();

	// Giotto simulation
	GiottoDirector giottoD = new GiottoDirector(scheduler);
	cout << "Starting Simulation Using Giotto" << endl;
	// Fill in - initialize stuff and start the director running...

	// Ptides simulation
	// PtidesDirector ptidesD = new PtidesDirector();
	// cout << "Starting Simulation Using Ptides" << endl;
	// Fill in - initialize stuff and start the director running...

	return 0;
}