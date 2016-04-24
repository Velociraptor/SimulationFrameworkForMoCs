#include <iostream>
#include "giotto.h"
#include "ptides.h"
#include "actors.h"
#include "scheduler.h"

int main() {
	cout << "Windshield Simulation Program for EE249B Project" << endl;

	// Giotto simulation
	cout << "Starting Simulation Using Giotto" << endl;

	// Initialize tasks and stuff, get a schedule for the given mode,
	// and start the director running...
	GiottoDirector giottoD = new GiottoDirector();

	// Weather Model simulates rainfall as a random int per [unit time]
	RandomIntInRange weatherModel = new RandomIntInRange("WeatherModel",20);
	Task generateRainfall = new Task("MakeItRain", weatherModel.Run());
	Driver rainDriver = new Driver("RainDriver"); // ??? what else to set?
	unsigned int rainGenFreq = 2000;
	TaskInvocation genRainInvoke = new TaskInvocation(generateRainfall, rainDriver, rainGenFreq);

	// Rainfall Sensor accumulates rain over [unit time] to send to control
	AccumulatorWithReset rainfallSensor = new AccumulatorWithReset("RainfallSensor");
	Task senseRainfall = new Task("HaveYouEverFeltTheRain", rainfallSensor.Run());
	Driver sensorDriver = new Driver("SensorDriver"); // ???
	unsigned int rainSenseFreq = 2000;
	TaskInvocation senseRainInvoke = new TaskInvocation(senseRainfall, sensorDriver, rainSenseFreq);
	

	// Modes and switching logic for wiper speed based on tracked rate of rainfall
	Mode lowRain = new Mode(); // ???
	Mode medRain = new Mode();
	Mode highRain = new Mode();
	vector<TaskInvocation> lowRainTaskList;
	vector<TaskInvocation> medRainTaskList;
	vector<TaskInvocation> highRainTaskList;
	lowRainTaskList.push_back(genRainInvoke);
	lowRainTaskList.push_back(senseRainInvoke);
	medRainTaskList.push_back(genRainInvoke);
	medRainTaskList.push_back(senseRainInvoke);
	highRainTaskList.push_back(genRainInvoke);
	highRainTaskList.push_back(senseRainInvoke);
	// Wiper actuation based on operating mode
	// (wiper action is to reset sensor accumulator)
	Task actuateWiper new Task("WipeItOff", rainfallSensor.Reset());
	Driver wiperDriver = new Driver("WiperDriver"); // ???
	unsigned int lowWiperFreq = 250;
	TaskInvocation lowWiperInvoke = new TaskInvocation(actuateWiper, wiperDriver, lowWiperFreq);
	lowRainTaskList.push_back(lowWiperInvoke);
	unsigned int medWiperFreq = 500;
	TaskInvocation medWiperInvoke = new TaskInvocation(actuateWiper, wiperDriver, medWiperFreq);
	medRainTaskList.push_back(medWiperInvoke);
	unsigned int highWiperFreq = 1000;
	TaskInvocation highWiperInvoke = new TaskInvocation(actuateWiper, wiperDriver, highWiperFreq);
	highRainTaskList.push_back(highWiperInvoke);
	// ...
	ModeSwitch wiperSpeed = new ModeSwitch(); // ???

	// Pass (start mode) task list and mode switch list to Giotto director to run simulation
	// .......




	// // Ptides simulation
	// PtidesDirector ptidesD = new PtidesDirector();
	// cout << "Starting Simulation Using Ptides" << endl;
	// Fill in - initialize stuff and start the director running...

	return 0;
}