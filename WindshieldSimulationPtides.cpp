#include <iostream>
#include "giotto.h"
#include "ptides.h"
#include "actors.h"
#include "scheduler.h"

int main() {
	cout << "Windshield Simulation Program for EE249B Project" << endl;

	// Ptides simulation
	cout << "Starting Simulation Using PTIDES" << endl;

	// Weather Model simulates rainfall as a repeating ramp from 0 to 100
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

	// Giotto wrapper task invocation for rain generation based on weather model
	Task *generateRainfall = new Task(string("MakeItRain"), weatherModel);
	// Empty port vector for empty guards
	vector<Port*> trivialPV;
	Guard *rainGuard = new Guard(string("RainGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainGenFreq = 20;
	unsigned int rainGenPriority = 1;
	TaskInvocation *genRainInvoke = new TaskInvocation(generateRainfall, rainGuard, rainGenFreq, rainGenPriority);

	// Giotto wrapper task invocation list
	vector<TaskInvocation*> giottoWrapperTaskList;
	giottoWrapperTaskList.push_back(genRainInvoke);

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
	chrono::milliseconds rainfallSensorDelay(5);
	PTask *senseRainfall = new PTask(string("SenseTheRain"), rainfallSensor, rainfallSensorDelay);

	// First PTIDES Platform and related PTasks - Sensing
	vector<PTask*> senseTasks;
	senseTasks.push_back(senseRainfall);
	unsigned int senseClk = 0;
	Platform *sensePlatform = new Platform(string("sensePlatform"), senseTasks, senseClk);

	// Wiper actuation based on trigger threshold
	// (wiper action is to reset sensor accumulator by triggering reset port)
	// input ports: threshold value and sensed rainfall accumulation - trigger if above threshold
	PortValue pvThresholdInit;
	pvThresholdInit.valInt = 100; // wipe when at least 100 rain accumulated
	PortContents pcWiperThreshold = {INT, pvThresholdInit};
	Port *wiperThreshold = new Port(string("wiperThreshold"), pcWiperThreshold);
	vector<Port*> vipWiper;
	vipWiper.push_back(wiperThreshold);
	vipWiper.push_back(rainfallSensed);
	// output port: reset input to sensor accumulator
	vector<Port*> vopWiper;
	vopWiper.push_back(rainfallReset);
	// Initialize wiper actor
	ThresholdTrigger *wiper = new ThresholdTrigger(string("wiper"), vipWiper, vopWiper);
	chrono::milliseconds wiperDelay(10);
	PTask *actuateWiper = new PTask(string("WipeItOff"), wiper, wiperDelay);

	// Second PTIDES Platform and related PTasks - actuation
	vector<PTask*> actuateTasks;
	actuateTasks.push_back(actuateWiper);
	unsigned int actuateClk = 0;
	Platform *actuatePlatform = new Platform(string("actuatePlatform"), actuateTasks, actuateClk);

	// Network connection between sensing and actuation platforms
	chrono::milliseconds networkDelay(15);
	Network *network = new Network(string("NetworkConnection"), sensePlatform, actuatePlatform, networkDelay);

	// Initialize PTIDES director
	vector<Network*> networks;
	networks.push_back(network);
	chrono::milliseconds timestart(0);
	PtidesDirector* ptidesD = new PtidesDirector(sensePlatform, networks, timestart);

	// Giotto Mode setup
	Mode *wrapperMode = new Mode(string("giottoWrapperMode"), giottoWrapperTaskList);

	// Trigger a Giotto interrupt to run Ptides director whetnever rain is generated
	Interrupt *rainfallInterrupt = new Interrupt("RainfallOccurrence", ptidesD);
	Guard *rainIntGuard = new Guard(string("RainInterruptGuard"), trivialTrueGuard, trivialPV);
	unsigned int rainIntFreq = 20;
	unsigned int rainIntPriority = 1;
	InterruptInvocation *rainfallIntInvoke = new InterruptInvocation(rainfallInterrupt, rainIntGuard, rainIntPriority);
	vector<InterruptInvocation*> giottoInterrupts;
	giottoInterrupts.push_back(rainfallIntInvoke);

	// Initialize Giotto wrapper director and kick off simulation
	vector<ModeSwitch*> noModeSwitches;
	unsigned int noModeSwitchCheckFreq = 1;
	GiottoDirector giottoWrapperD = GiottoDirector(wrapperMode, noModeSwitches, noModeSwitchCheckFreq, giottoInterrupts, rainIntFreq);
	std::chrono::milliseconds m(10000);
	giottoWrapperD.Run(m);

	return 0;
}