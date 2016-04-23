WindshieldSimulation: WindshieldSimulation.o actor.o giotto.o ptides.o scheduler.o
	g++ -Wall -std=c++11 -g WindshieldSimulation.o actor.o giotto.o ptides.o scheduler.o -o WindshieldSimulation
WindshieldSimulation.o: WindshieldSimulation.cpp giotto.h ptides.h actors.h scheduler.h
	g++ -Wall -std=c++11 -c WindshieldSimulation.cpp
giotto.o: giotto.cpp giotto.h actors.h schduler.h
	g++ -Wall -std=c++11 -c giotto.cpp
ptides.o: ptides.cpp ptides.h actors.h
	g++ -Wall -c ptides.cpp
actors.o: actors.cpp actors.h
	g++ -Wall -c actors.cpp
scheduler.o: scheduler.cpp scheduler.h
	g++ -Wall -c scheduler.cpp