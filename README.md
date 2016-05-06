# SuperFunPartyMoCs
EE249B Project to implement Giotto and PTIDES Models of Computation in C++
Uma Balakrishnan and Hannah Sarver
EE 249B Embedded Systems Spring 2016

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

# Simulation - Giotto
The current windshield simulation consists of a simple actor model, a very naive scheduler, and a Giotto implementation (based on the model of computation as presented in https://www.hh.se/download/18.70cf2e49129168da0158000145742/henzinger-giotto-time-triggered.pdf) to drive execution based on scheduled periodic tasks.  The simulation can run with either a ramping or random weather model, exhibiting different mode-switching behavior between low, medium, and high rain modes (dictating the frequency of the wiper actuation task).

# Simulation - PTIDES
Simulation using our Actor model, periodic Scheduler, and very basic PTIDES class implementation based on the model of computation presented in: https://chess.eecs.berkeley.edu/pubs/529/RTASPtides-1.pdf with wrapper Giotto director providing a periodic trigger of rain generation to kick off event-based PTIDES execution. This simple test system includes separate platforms for sensing and actuation with data transferred across a network (represented by a logical time delay).
