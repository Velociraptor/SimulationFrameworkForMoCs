# SuperFunPartyMoCs
EE249B Project to implement Giotto (and maybe someday Ptides) models of Computation in C++

# Files
windshieldsim.makefile - Make all the things!  
WindshieldSimulation.cpp - runs main simulation(s)  
giotto.h - header file for Giotto-related classes  
giotto.cpp - implementation of Giotto-related classes  
ptides.h - header file for Ptides-related classes  
ptides.cpp - implementation of Ptides-related classes  
actors.h - header file for Actor classes  
actors.cpp - implementation of Actor classes  
scheduler.h - header file for Scheduler
scheduler.cpp - implementation thereof

# Simulation
The current windshield simulation consists of a simple actor model, a very naive scheduler, and a Giotto implementation to drive execution based on scheduled periodic tasks.  The simulation can run with either a ramping or random weather model, exhibiting different mode-switching behavior between low, medium, and high rain modes (dictating the frequency of the wiper task).