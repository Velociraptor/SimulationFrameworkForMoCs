WindshieldSimulation: WindshieldSimulation.o actor.o giotto.o ptides.o
	g++ -Wall -std=c++11 -g WindshieldSimulation.o actor.o giotto.o ptides.o -o WindshieldSimulation
WindshieldSimulation.o: WindshieldSimulation.cpp giotto.h ptides.h actors.h
	g++ -Wall -std=c++11 -c WindshieldSimulation.cpp
giotto.o: giotto.cpp giotto.h actors.h
	g++ -Wall -std=c++11 -c giotto.cpp
ptides.o: ptides.cpp ptides.h actors.h
	g++ -Wall -c ptides.cpp
actors.o: actors.cpp actors.h
	g++ -Wall -c actors.cpp