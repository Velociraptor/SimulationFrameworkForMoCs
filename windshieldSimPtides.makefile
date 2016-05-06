WindshieldSimulationPtides: WindshieldSimulationPtides.o actors.o giotto.o ptides.o scheduler.o
	g++ -Wall -std=c++11 -g WindshieldSimulationPtides.o actors.o giotto.o ptides.o scheduler.o -o WindshieldSimulationPtides
WindshieldSimulationPtides.o: WindshieldSimulationPtides.cpp giotto.h ptides.h actors.h scheduler.h
	g++ -Wall -std=c++11 -c WindshieldSimulationPtides.cpp
giotto.o: giotto.cpp giotto.h actors.h scheduler.h
	g++ -Wall -std=c++11 -c giotto.cpp
ptides.o: ptides.cpp ptides.h actors.h
	g++ -Wall -std=c++11 -c ptides.cpp
actors.o: actors.cpp actors.h
	g++ -Wall -std=c++11 -c actors.cpp
scheduler.o: scheduler.cpp scheduler.h
	g++ -Wall -std=c++11 -c scheduler.cpp