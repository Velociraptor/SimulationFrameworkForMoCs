#include <iostream>
#include "giotto.h"
#include "ptides.h"
#include "actors.h"
#include "scheduler.h"

int main() {
	cout << "Windshield Simulation Program for EE249B Project" << endl;

	// Giotto simulation
	cout << "Starting Simulation Using Giotto" << endl;

	// Weather Model simulates rainfall as a random int per [unit time]
	RandomIntInRange weatherModel = new RandomIntInRange("WeatherModel",20);
	Task generateRainfall = new Task("MakeItRain", weatherModel.Compute());
	Driver rainDriver = new Driver("RainDriver"); // ??? what else to set?
	unsigned int rainGenFreq = 2000;
	TaskInvocation genRainInvoke = new TaskInvocation(generateRainfall, rainDriver, rainGenFreq);

	// Rainfall Sensor accumulates rain over time to dictate rate of fall for controls
	AccumulatorWithReset rainfallSensor = new AccumulatorWithReset("RainfallSensor");
	Task senseRainfall = new Task("HaveYouEverFeltTheRain", rainfallSensor.Compute());
	Driver sensorDriver = new Driver("SensorDriver"); // ???
	unsigned int rainSenseFreq = 2000;
	TaskInvocation senseRainInvoke = new TaskInvocation(senseRainfall, sensorDriver, rainSenseFreq);
	
	// Rainfall Rate tracks rainfall over [unit time] to send to mode control
	Difference rainfallRateCheck = new Difference("RainfallRateCheck");
	Task checkRainfallRate = new Task("RainingCatsOrDogs", rainfallRateCheck.Compute());
	Driver rainRateDriver = new Driver("rainRateDriver"); // ???
	unsigned int rainRateCheckFreq = 2000;
	TaskInvocation rainRateInvoke = new TaskInvocation(checkRainfallRate, rainRateDriver, rainRateCheckFreq);

	// Modes and switching logic for wiper speed based on tracked rate of rainfall
	Mode lowRainMode = new Mode(); // ???
	Mode medRainMode = new Mode();
	Mode highRainMode = new Mode();
	vector<TaskInvocation> lowRainTaskList;
	vector<TaskInvocation> medRainTaskList;
	vector<TaskInvocation> highRainTaskList;
	lowRainTaskList.push_back(genRainInvoke);
	lowRainTaskList.push_back(senseRainInvoke);
	lowRainTaskList.push_back(rainRateInvoke);
	medRainTaskList.push_back(genRainInvoke);
	medRainTaskList.push_back(senseRainInvoke);
	medRainTaskList.push_back(rainRateInvoke);
	highRainTaskList.push_back(genRainInvoke);
	highRainTaskList.push_back(senseRainInvoke);
	highRainTaskList.push_back(rainRateInvoke);
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

	// ????
	// Mode switches *to* each possible mode, 
	// with Drivers that have the appropriate guards 
	// and a frequency as often as we want to be able to switch modes...
	unsigned int modeSwitchCheckFreq = 100;
	Driver lowRainDriver = new Driver("lowRainDriver"); // set guard: not already in low, and rainfall rate check in low range
	ModeSwitch wiperSpeedLowSwitch = new ModeSwitch(lowRainDriver, lowRainMode, modeSwitchCheckFreq);
	Driver medRainDriver = new Driver("medRainDriver"); // set guard: not already in med, and rainfall rate check in med range
	ModeSwitch wiperSpeedMedSwitch = new ModeSwitch(medRainDriver, medRainMode, modeSwitchCheckFreq);
	Driver highRainDriver = new Driver("highRainDriver"); // set guard: not already in high, and rainfall rate check in high range
	ModeSwitch wiperSpeedHighSwitch = new ModeSwitch(highRainDriver, highRainMode, modeSwitchCheckFreq);

	// More mode setup
	// Set invocations for each mode to xxxWiperInvoke
	// Set mode switches for each mode to other two wiperSpeedXxx
	lowRainMode.setInvokes(lowRainTaskList);
	medRainMode.setInvokes(medRainTaskList);
	highRainMode.setInvokes(highRainTaskList);
	vector<ModeSwitch> lowRainModeSwitches;
	lowRainModeSwitches.push_back(wiperSpeedMedSwitch);
	lowRainModeSwitches.push_back(wiperSpeedHighSwitch);
	lowRainMode.setModeSwitches(lowRainModeSwitches);
	vector<ModeSwitch> medRainModeSwitches;
	medRainModeSwitches.push_back(wiperSpeedLowSwitch);
	medRainModeSwitches.push_back(wiperSpeedHighSwitch);
	medRainMode.setModeSwitches(medRainModeSwitches);
	vector<ModeSwitch> highRainModeSwitches;
	highRainModeSwitches.push_back(wiperSpeedLowSwitch);
	highRainModeSwitches.push_back(wiperSpeedMedSwitch);
	highRainMode.setModeSwitches(highRainModeSwitches);

	// Pass (start mode) task list to Giotto director to run simulation
	// .......
	// set mode list & set start mode ???
	// Initialize tasks and stuff, get a schedule for the given mode,
	// and start the director running...
	// vector<Mode*> modeVector;
	// modeVector.push_back(lowRainMode);
	// modeVector.push_back(medRainMode);
	// modeVector.push_back(highRainMode);
	GiottoDirector giottoD = new GiottoDirector(lowRainMode);
	// port and driver lists ???
	giottoD.Run();




	// // Ptides simulation
	// PtidesDirector ptidesD = new PtidesDirector();
	// cout << "Starting Simulation Using Ptides" << endl;
	// Fill in - initialize stuff and start the director running...

	return 0;
}