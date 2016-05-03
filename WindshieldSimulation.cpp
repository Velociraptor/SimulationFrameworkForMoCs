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

	// // Weather Model simulates rainfall as a random int per [unit time]
	// // input port: range of rain creation
	// srand((unsigned)time(NULL));
	// PortValue pvRainGenRange;
	// pvRainGenRange.valInt = 100;
	// PortContents pcRainGenRange = {INT, pvRainGenRange};
	// Port *rainGenRange = new Port(string("rainGenerationRange"), pcRainGenRange);
	// vector<Port*> vipRainGen;
	// vipRainGen.push_back(rainGenRange);
	// // output port: randomly generated int (in range)
	// PortValue pvRainInit;
	// pvRainInit.valInt = 0;
	// PortContents pcRainfall = {INT, pvRainInit};
	// Port *rainfall = new Port(string("rainfall"), pcRainfall);
	// vector<Port*> vopRainGen;
	// vopRainGen.push_back(rainfall);
	// // Initialize weather model actor
	// RandomIntInRange *weatherModel = new RandomIntInRange(string("WeatherModel"), vipRainGen, vopRainGen);

	// Alternate Weather Model simulates rainfall as a repeating ramp from 0 to 100
	// input ports: range of rain creation, increment	PortValue pvRainGenRange;
	PortValue pvRainGenRange;
	pvRainGenRange.valInt = 100;
	PortContents pcRainGenRange = {INT, pvRainGenRange};
	Port *rainGenRange = new Port(string("rainGenerationRange"), pcRainGenRange);
	PortValue pvRainGenIncr;
	pvRainGenIncr.valInt = 1;
	PortContents pcRainGenIncr = {INT, pvRainGenIncr};
	Port *rainGenIncr = new Port(string("rainGenerationIncrement"), pcRainGenIncr);
	vector<Port*> vipRainGen;
	vipRainGen.push_back(rainGenRange);
	vipRainGen.push_back(rainGenIncr);
	// output port: generated rainfall int (ramp in range)
	PortValue pvRainInit;
	pvRainInit.valInt = 0;
	PortContents pcRainfall = {INT, pvRainInit};
	Port *rainfall = new Port(string("rainfall"), pcRainfall);
	vector<Port*> vopRainGen;
	vopRainGen.push_back(rainfall);
	// Initialize weather model actor
	RepeatingRamp *weatherModel = new RepeatingRamp(string("WeatherModel"), vipRainGen, vopRainGen);

	// Task invocation for rain generation based on weather model
	Task *generateRainfall = new Task(string("MakeItRain"), weatherModel);
	// Empty port vector for empty guards
	vector<Port*> trivialPV;
	Guard *rainGuard = new Guard(string("RainGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainGenFreq = 200;
	unsigned int rainGenPriority = 1;
	TaskInvocation *genRainInvoke = new TaskInvocation(generateRainfall, rainGuard, rainGenFreq, rainGenPriority);

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
	Task *senseRainfall = new Task(string("SenseTheRain"), rainfallSensor);
	Guard *sensorGuard = new Guard(string("SensorGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainSenseFreq = 200;
	unsigned int rainSensePriority = 2;
	TaskInvocation *senseRainInvoke = new TaskInvocation(senseRainfall, sensorGuard, rainSenseFreq, rainSensePriority);

	// Rainfall Register stores last rainfall sensor accumulator output for rate check
	// input port: sensed rain accumulation from rainfall sensor
	vector<Port*> vipRainRegister;
	vipRainRegister.push_back(rainfallSensed);
	// output port: previous rainfall datapoint stored
	PortValue pvRainRegInit;
	pvRainRegInit.valInt = 0;
	PortContents pcRainRegister = {INT, pvRainRegInit};
	Port *rainfallPrevStored = new Port(string("rainfallPrevStored"), pcRainRegister);
	vector<Port*> vopRainRegister;
	vopRainRegister.push_back(rainfallPrevStored);
	// Initialize rain register actor
	Register *prevRainRegister = new Register(string("PreviousRainRegister"), vipRainRegister, vopRainRegister);
	Task *storeLatestRainfall = new Task(string("StoreLatestRain"), prevRainRegister);
	Guard *rainRegGuard = new Guard(string("rainStoreGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainStoreFreq = 200;
	unsigned int rainStorePriority = 3;
	TaskInvocation *storeRainInvoke = new TaskInvocation(storeLatestRainfall, rainRegGuard, rainStoreFreq, rainStorePriority);
	
	// Rainfall Rate tracks rainfall over time to send to mode control
	// input ports: sensed rain accumulation from rainfall sensor, and previous value from register
	vector<Port*> vipRainRate;
	vipRainRate.push_back(rainfallSensed);
	vipRainRate.push_back(rainfallPrevStored);
	// output port: determined rain rate over latest time unit
	PortValue pvRainRateInit;
	pvRainRateInit.valInt = 0;
	PortContents pcRainfallRate = {INT, pvRainRateInit};
	Port *rainfallRate = new Port(string("rainfallRate"), pcRainfallRate);
	vector<Port*> vopRainRate;
	vopRainRate.push_back(rainfallRate);
	// Initialize rain rate actor
	Difference *rainfallRateCheck = new Difference(string("RainfallRateCheck"),vipRainRate, vopRainRate);
	Task *checkRainfallRate = new Task(string("CheckRainRate"), rainfallRateCheck);
	Guard *rainRateGuard = new Guard(string("rainRateGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainRateCheckFreq = 200;
	unsigned int rainRatePriority = 4;
	TaskInvocation *rainRateInvoke = new TaskInvocation(checkRainfallRate, rainRateGuard, rainRateCheckFreq, rainRatePriority);

	// Mode task invocations for generating and tracking rainfall
	vector<TaskInvocation*> lowRainTaskList;
	vector<TaskInvocation*> medRainTaskList;
	vector<TaskInvocation*> highRainTaskList;
	lowRainTaskList.push_back(genRainInvoke);
	lowRainTaskList.push_back(senseRainInvoke);
	lowRainTaskList.push_back(storeRainInvoke);
	lowRainTaskList.push_back(rainRateInvoke);
	medRainTaskList.push_back(genRainInvoke);
	medRainTaskList.push_back(senseRainInvoke);
	medRainTaskList.push_back(storeRainInvoke);
	medRainTaskList.push_back(rainRateInvoke);
	highRainTaskList.push_back(genRainInvoke);
	highRainTaskList.push_back(senseRainInvoke);
	highRainTaskList.push_back(storeRainInvoke);
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
	Guard *wiperGuard = new Guard(string("WiperGuard"), trivialTrueGuard, trivialPV);
	unsigned int lowWiperFreq = 25;
	unsigned int lowWiperPriority = 10;
	Task *actuateWiperLow = new Task(string("WipeItOffLow"), wiper);
	TaskInvocation *lowWiperInvoke = new TaskInvocation(actuateWiperLow, wiperGuard, lowWiperFreq, lowWiperPriority);
	lowRainTaskList.push_back(lowWiperInvoke);
	unsigned int medWiperFreq = 50;
	unsigned int medWiperPriority = 10;
	Task *actuateWiperMed = new Task(string("WipeItOffMed"), wiper);
	TaskInvocation *medWiperInvoke = new TaskInvocation(actuateWiperMed, wiperGuard, medWiperFreq, medWiperPriority);
	medRainTaskList.push_back(medWiperInvoke);
	unsigned int highWiperFreq = 100;
	unsigned int highWiperPriority = 10;
	Task *actuateWiperHigh = new Task(string("WipeItOffHigh"), wiper);
	TaskInvocation *highWiperInvoke = new TaskInvocation(actuateWiperHigh, wiperGuard, highWiperFreq, highWiperPriority);
	highRainTaskList.push_back(highWiperInvoke);

	cout << "Mode setup" << endl;

	// Mode setup
	Mode *lowRainMode = new Mode(string("lowRainMode"), lowRainTaskList);
	Mode *medRainMode = new Mode(string("medRainMode"), medRainTaskList);
	Mode *highRainMode = new Mode(string("highRainMode"), highRainTaskList);

	// Mode switches to each possible destination mode from each possible start,
	// with appropriate Guards calling functions above with correct port for rain rate
	// and a frequency as often as we want to be able to switch modes (set in director)
	vector<Port*> pvGuard;
	pvGuard.push_back(rainfallRate);
	Guard *lowRainGuard = new Guard(string("lowRainGuard"), rainRateLow, pvGuard);
	Guard *medRainGuard = new Guard(string("medRainGuard"), rainRateMed, pvGuard);
	Guard *highRainGuard = new Guard(string("highRainGuard"), rainRateHigh, pvGuard);
	ModeSwitch *wiperSpeedLowToMedSwitch = new ModeSwitch(medRainGuard, lowRainMode, medRainMode);
	ModeSwitch *wiperSpeedLowToHighSwitch = new ModeSwitch(highRainGuard, lowRainMode, highRainMode);
	ModeSwitch *wiperSpeedMedToLowSwitch = new ModeSwitch(lowRainGuard, medRainMode, lowRainMode);
	ModeSwitch *wiperSpeedMedToHighSwitch = new ModeSwitch(highRainGuard, medRainMode, highRainMode);
	ModeSwitch *wiperSpeedHighToLowSwitch = new ModeSwitch(lowRainGuard, highRainMode, lowRainMode);
	ModeSwitch *wiperSpeedHighToMedSwitch = new ModeSwitch(medRainGuard, highRainMode, medRainMode);

	// Set up vector of mode switches for director
	vector<ModeSwitch*> rainModeSwitches;
	rainModeSwitches.push_back(wiperSpeedLowToMedSwitch);
	rainModeSwitches.push_back(wiperSpeedLowToHighSwitch);
	rainModeSwitches.push_back(wiperSpeedMedToLowSwitch);
	rainModeSwitches.push_back(wiperSpeedMedToHighSwitch);
	rainModeSwitches.push_back(wiperSpeedHighToLowSwitch);
	rainModeSwitches.push_back(wiperSpeedHighToMedSwitch);

	// Initialize director with start mode and mode switches and kick off simulation
	unsigned int modeSwitchCheckFreq = 10;
	GiottoDirector giottoD = GiottoDirector(lowRainMode, rainModeSwitches, modeSwitchCheckFreq);
	std::chrono::milliseconds m(10000);
	giottoD.Run(m);

	// // // Ptides simulation
	// // PtidesDirector ptidesD = new PtidesDirector();
	// // cout << "Starting Simulation Using Ptides" << endl;
	// // Fill in - initialize stuff and start the director running...

	return 0;
}