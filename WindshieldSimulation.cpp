#include <iostream>
#include "giotto.h"
#include "ptides.h"
#include "actors.h"
#include "scheduler.h"

// Define guard functions for mode switching
bool rainRateLow (vector<Port*> p) {
	// rainfall low from latest check
	int currentRainRate = p[0]->GetValueInt();
	if (currentRainRate < 34) {
		return true;
	} else {
		return false;
	}
}
bool rainRateMed (vector<Port*> p) {
	// rainfall medium from latest check
	int currentRainRate = p[0]->GetValueInt();
	if (currentRainRate > 33 && currentRainRate < 67) {
		return true;
	} else {
		return false;
	}
}
bool rainRateHigh (vector<Port*> p) {
	// rainfall high from latest check
	int currentRainRate = p[0]->GetValueInt();
	if (currentRainRate > 66) {
		return true;
	} else {
		return false;
	}
}

int main() {
	cout << "Windshield Simulation Program for EE249B Project" << endl;

	// Giotto simulation
	cout << "Starting Simulation Using Giotto" << endl;

	// Weather Model simulates rainfall as a random int per [unit time]
	// input port: range of rain creation
	srand((unsigned)time(NULL));
	PortValue pvRainGenRange;
	pvRainGenRange.valInt = 100;
	PortContents pcRainGenRange = {INT, pvRainGenRange};
	Port *rainGenRange = new Port(string("rainGenerationRange"), pcRainGenRange);
	vector<Port*> vipRainGen;
	vipRainGen.push_back(rainGenRange);
	// output port: randomly generated int (in range)
	PortValue pvRainInit;
	pvRainInit.valInt = 0;
	PortContents pcRainfall = {INT, pvRainInit};
	Port *rainfall = new Port(string("rainfall"), pcRainfall);
	vector<Port*> vopRainGen;
	vopRainGen.push_back(rainfall);
	// Initialize weather model actor
	RandomIntInRange *weatherModel = new RandomIntInRange(string("WeatherModel"), vipRainGen, vopRainGen);
	Task *generateRainfall = new Task(string("MakeItRain"), weatherModel);
	// Empty port vector for empty guards
	vector<Port*> trivialPV;
	Guard *rainGuard = new Guard(string("RainGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainGenFreq = 2000;
	TaskInvocation *genRainInvoke = new TaskInvocation(generateRainfall, rainGuard, rainGenFreq);

	// Rainfall Sensor accumulates rain over time to dictate rate of fall for controls
	// input ports: generated rain from weather model, reset port (start false)
	PortValue pvRainResetInit;
	pvRainResetInit.valBool = false;
	PortContents pcRainReset = {BOOL, pvRainResetInit};
	Port *rainfallReset = new Port(string("rainfallReset"), pcRainReset);
	vector<Port*> vipRainSense;
	vipRainSense.push_back(rainfall);
	vipRainSense.push_back(rainfallReset);
	// output port: current rain accumulation on windshield (sensed)
	PortValue pvRainSenseInit;
	pvRainSenseInit.valInt = 0;
	PortContents pcRainfallSensed = {INT, pvRainSenseInit};
	Port *rainfallSensed = new Port(string("rainfallSensed"), pcRainfallSensed);
	vector<Port*> vopRainSense;
	vopRainSense.push_back(rainfallSensed);
	// Initialize rain sensor actor
	AccumulatorWithReset *rainfallSensor = new AccumulatorWithReset(string("RainfallSensor"), vipRainSense, vopRainSense);
	Task *senseRainfall = new Task(string("HaveYouEverFeltTheRain"), rainfallSensor);
	Guard *sensorGuard = new Guard(string("SensorGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainSenseFreq = 2000;
	TaskInvocation *senseRainInvoke = new TaskInvocation(senseRainfall, sensorGuard, rainSenseFreq);
	
	// Rainfall Rate tracks rainfall over time to send to mode control
	// input port: sensed rain accumulation from rainfall sensor
	vector<Port*> vipRainRate;
	vipRainRate.push_back(rainfallSensed);
	// output port: determined rain rate over latest time unit
	PortValue pvRainRateInit;
	pvRainRateInit.valInt = 0;
	PortContents pcRainfallRate = {INT, pvRainRateInit};
	Port *rainfallRate = new Port(string("rainfallRate"), pcRainfallRate);
	vector<Port*> vopRainRate;
	vopRainRate.push_back(rainfallRate);
	// Initialize rain rate actor
	Difference *rainfallRateCheck = new Difference(string("RainfallRateCheck"),vipRainRate, vopRainRate);
	Task *checkRainfallRate = new Task(string("RainingCatsOrDogs"), rainfallRateCheck);
	Guard *rainRateGuard = new Guard(string("rainRateGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainRateCheckFreq = 2000;
	TaskInvocation *rainRateInvoke = new TaskInvocation(checkRainfallRate, rainRateGuard, rainRateCheckFreq);

	// Mode task invocations for generating and tracking rainfall
	vector<TaskInvocation*> lowRainTaskList;
	vector<TaskInvocation*> medRainTaskList;
	vector<TaskInvocation*> highRainTaskList;
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
	// (wiper action is to reset sensor accumulator by triggering reset port)
	// no input port, just triggers whenever Compute() called
	vector<Port*> vipWiper;
	// output port: reset input to sensor accumulator
	vector<Port*> vopWiper;
	vopWiper.push_back(rainfallReset);
	// Initialize wiper actor
	Trigger *wiper = new Trigger(string("wiper"), vipWiper, vopWiper);
	Task *actuateWiper = new Task(string("WipeItOff"), wiper);
	Guard *wiperGuard = new Guard(string("WiperGuard"), trivialTrueGuard, trivialPV);
	unsigned int lowWiperFreq = 250;
	TaskInvocation *lowWiperInvoke = new TaskInvocation(actuateWiper, wiperGuard, lowWiperFreq);
	lowRainTaskList.push_back(lowWiperInvoke);
	unsigned int medWiperFreq = 500;
	TaskInvocation *medWiperInvoke = new TaskInvocation(actuateWiper, wiperGuard, medWiperFreq);
	medRainTaskList.push_back(medWiperInvoke);
	unsigned int highWiperFreq = 1000;
	TaskInvocation *highWiperInvoke = new TaskInvocation(actuateWiper, wiperGuard, highWiperFreq);
	highRainTaskList.push_back(highWiperInvoke);

	// Mode setup
	Mode *lowRainMode = new Mode(string("lowRainMode"), lowRainTaskList);
	Mode *medRainMode = new Mode(string("medRainMode"), medRainTaskList);
	Mode *highRainMode = new Mode(string("highRainMode"), highRainTaskList);

	// Mode switches to each possible destination mode from each possible start,
	// with appropriate Guards calling functions above with correct port for rain rate
	// and a frequency as often as we want to be able to switch modes
	unsigned int modeSwitchCheckFreq = 100;
	vector<Port*> pvGuard;
	pvGuard.push_back(rainfallRate);
	Guard *lowRainGuard = new Guard(string("lowRainGuard"), rainRateLow, pvGuard);
	Guard *medRainGuard = new Guard(string("medRainGuard"), rainRateMed, pvGuard);
	Guard *highRainGuard = new Guard(string("highRainGuard"), rainRateHigh, pvGuard);
	ModeSwitch *wiperSpeedLowToMedSwitch = new ModeSwitch(medRainGuard, lowRainMode, medRainMode, modeSwitchCheckFreq);
	ModeSwitch *wiperSpeedLowToHighSwitch = new ModeSwitch(highRainGuard, lowRainMode, highRainMode, modeSwitchCheckFreq);
	ModeSwitch *wiperSpeedMedToLowSwitch = new ModeSwitch(lowRainGuard, medRainMode, lowRainMode, modeSwitchCheckFreq);
	ModeSwitch *wiperSpeedMedToHighSwitch = new ModeSwitch(highRainGuard, medRainMode, highRainMode, modeSwitchCheckFreq);
	ModeSwitch *wiperSpeedHighToLowSwitch = new ModeSwitch(lowRainGuard, highRainMode, lowRainMode, modeSwitchCheckFreq);
	ModeSwitch *wiperSpeedHighToMedSwitch = new ModeSwitch(medRainGuard, highRainMode, medRainMode, modeSwitchCheckFreq);

	// Set up vector of mode switches for director
	vector<ModeSwitch*> rainModeSwitches;
	rainModeSwitches.push_back(wiperSpeedLowToMedSwitch);
	rainModeSwitches.push_back(wiperSpeedLowToHighSwitch);
	rainModeSwitches.push_back(wiperSpeedMedToLowSwitch);
	rainModeSwitches.push_back(wiperSpeedMedToHighSwitch);
	rainModeSwitches.push_back(wiperSpeedHighToLowSwitch);
	rainModeSwitches.push_back(wiperSpeedHighToMedSwitch);

	// Initialize director with start mode and mode switches and kick off simulation
	GiottoDirector giottoD = GiottoDirector(lowRainMode, rainModeSwitches);
	std::chrono::milliseconds m(10000);
	giottoD.Run(m);

	// // // Ptides simulation
	// // PtidesDirector ptidesD = new PtidesDirector();
	// // cout << "Starting Simulation Using Ptides" << endl;
	// // Fill in - initialize stuff and start the director running...

	return 0;
}