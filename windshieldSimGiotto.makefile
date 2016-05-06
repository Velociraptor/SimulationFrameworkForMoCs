WindshieldSimulationGiotto: WindshieldSimulationGiotto.o actors.o giotto.o ptides.o scheduler.o
	g++ -Wall -std=c++11 -g WindshieldSimulationGiotto.o actors.o giotto.o ptides.o scheduler.o -o WindshieldSimulationGiotto
WindshieldSimulationGiotto.o: WindshieldSimulationGiotto.cpp giotto.h ptides.h actors.h scheduler.h
	g++ -Wall -std=c++11 -c WindshieldSimulationGiotto.cpp
giotto.o: giotto.cpp giotto.h ptides.h actors.h scheduler.h
	g++ -Wall -std=c++11 -c giotto.cpp
ptides.o: ptides.cpp ptides.h actors.h
	g++ -Wall -std=c++11 -c ptides.cpp
actors.o: actors.cpp actors.h
	g++ -Wall -std=c++11 -c actors.cpp
scheduler.o: scheduler.cpp scheduler.h
	g++ -Wall -std=c++11 -c scheduler.cpp