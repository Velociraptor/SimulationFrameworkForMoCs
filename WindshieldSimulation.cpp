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
	// input port: range of rain creation
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
	RandomIntInRange weatherModel = RandomIntInRange(string("WeatherModel"), vipRainGen, vopRainGen);
	Task generateRainfall = Task(string("MakeItRain"), weatherModel.Compute);
	Guard rainGuard = Guard(string("RainGuard"), trivialTrueGuard);
	unsigned int rainGenFreq = 2000;
	TaskInvocation *genRainInvoke = new TaskInvocation(generateRainfall, rainGuard, rainGenFreq);

	// // Rainfall Sensor accumulates rain over time to dictate rate of fall for controls
	// AccumulatorWithReset rainfallSensor = new AccumulatorWithReset("RainfallSensor");
	// Task senseRainfall = new Task("HaveYouEverFeltTheRain", rainfallSensor.Compute());
	// Guard sensorGuard = new Guard("SensorGuard", trivialTrueGuard);
	// unsigned int rainSenseFreq = 2000;
	// TaskInvocation senseRainInvoke = new TaskInvocation(senseRainfall, sensorGuard, rainSenseFreq);
	
	// // Rainfall Rate tracks rainfall over [unit time] to send to mode control
	// Difference rainfallRateCheck = new Difference("RainfallRateCheck");
	// Task checkRainfallRate = new Task("RainingCatsOrDogs", rainfallRateCheck.Compute());
	// Guard rainRateGuard = new Guard("rainRateGuard", trivialTrueGuard);
	// unsigned int rainRateCheckFreq = 2000;
	// TaskInvocation rainRateInvoke = new TaskInvocation(checkRainfallRate, rainRateGuard, rainRateCheckFreq);

	// // Mode task invocations and switching logic for wiper speed based on tracked rate of rainfall
	// vector<TaskInvocation> lowRainTaskList;
	// vector<TaskInvocation> medRainTaskList;
	// vector<TaskInvocation> highRainTaskList;
	// lowRainTaskList.push_back(genRainInvoke);
	// lowRainTaskList.push_back(senseRainInvoke);
	// lowRainTaskList.push_back(rainRateInvoke);
	// medRainTaskList.push_back(genRainInvoke);
	// medRainTaskList.push_back(senseRainInvoke);
	// medRainTaskList.push_back(rainRateInvoke);
	// highRainTaskList.push_back(genRainInvoke);
	// highRainTaskList.push_back(senseRainInvoke);
	// highRainTaskList.push_back(rainRateInvoke);

	// // Wiper actuation based on operating mode
	// // (wiper action is to reset sensor accumulator)
	// Task actuateWiper new Task("WipeItOff", rainfallSensor.GetInputs()[1]->SetValueBool(true));
	// Guard wiperGuard = new Guard("WiperGuard", trivialTrueGuard); // ???
	// unsigned int lowWiperFreq = 250;
	// TaskInvocation lowWiperInvoke = new TaskInvocation(actuateWiper, wiperGuard, lowWiperFreq);
	// lowRainTaskList.push_back(lowWiperInvoke);
	// unsigned int medWiperFreq = 500;
	// TaskInvocation medWiperInvoke = new TaskInvocation(actuateWiper, wiperGuard, medWiperFreq);
	// medRainTaskList.push_back(medWiperInvoke);
	// unsigned int highWiperFreq = 1000;
	// TaskInvocation highWiperInvoke = new TaskInvocation(actuateWiper, wiperGuard, highWiperFreq);
	// highRainTaskList.push_back(highWiperInvoke);
	// // ...

	// // Define guard functions
	// bool rainRateLow () {
	// 	// rainfall under 30 from last check
	// 	int currentRainRate = rainfallRateCheck.GetOutputs()[0]->GetValueInt();
	// 	if (currentRainRate < 34) {
	// 		return true;
	// 	} else {
	// 		return false;
	// 	}
	// }
	// bool rainRateMed () {
	// 	// rainfall under 30 from last check
	// 	int currentRainRate = rainfallRateCheck.GetOutputs()[0]->GetValueInt();
	// 	if (currentRainRate > 33 && currentRainRate < 67) {
	// 		return true;
	// 	} else {
	// 		return false;
	// 	}
	// }
	// bool rainRateHigh () {
	// 	// rainfall under 30 from last check
	// 	int currentRainRate = rainfallRateCheck.GetOutputs()[0]->GetValueInt();
	// 	if (currentRainRate > 66) {
	// 		return true;
	// 	} else {
	// 		return false;
	// 	}
	// }

	// // Mode switches *to* each possible mode, 
	// // with appropriate Guards 
	// // and a frequency as often as we want to be able to switch modes...
	// unsigned int modeSwitchCheckFreq = 100;
	// Guard lowRainGuard = new Guard("lowRainGuard", rainRateLow); // set guard: rainfall rate check in low range
	// ModeSwitch wiperSpeedLowSwitch = new ModeSwitch(lowRainGuard, lowRainMode, modeSwitchCheckFreq);
	// Guard medRainGuard = new Guard("medRainGuard", rainRateMed); // set guard: rainfall rate check in med range
	// ModeSwitch wiperSpeedMedSwitch = new ModeSwitch(medRainGuard, medRainMode, modeSwitchCheckFreq);
	// Guard highRainGuard = new Guard("highRainGuard", rainRateHigh); // set guard: rainfall rate check in high range
	// ModeSwitch wiperSpeedHighSwitch = new ModeSwitch(highRainGuard, highRainMode, modeSwitchCheckFreq);

	// // Set mode switches for each mode to other two wiperSpeedXxx
	// vector<ModeSwitch> lowRainModeSwitches;
	// lowRainModeSwitches.push_back(wiperSpeedMedSwitch);
	// lowRainModeSwitches.push_back(wiperSpeedHighSwitch);
	// vector<ModeSwitch> medRainModeSwitches;
	// medRainModeSwitches.push_back(wiperSpeedLowSwitch);
	// medRainModeSwitches.push_back(wiperSpeedHighSwitch);
	// vector<ModeSwitch> highRainModeSwitches;
	// highRainModeSwitches.push_back(wiperSpeedLowSwitch);
	// highRainModeSwitches.push_back(wiperSpeedMedSwitch);

	// // Mode setup
	// Mode lowRainMode = new Mode("lowRainMode", lowRainTaskList, lowRainModeSwitches);
	// Mode medRainMode = new Mode("medRainMode", medRainTaskList, medRainModeSwitches);
	// Mode highRainMode = new Mode("highRainMode", highRainTaskList, highRainModeSwitches);

	// // Initialize director with start mode and kick off simulation
	// GiottoDirector giottoD = new GiottoDirector(lowRainMode);
	// giottoD.Run();

	// // // Ptides simulation
	// // PtidesDirector ptidesD = new PtidesDirector();
	// // cout << "Starting Simulation Using Ptides" << endl;
	// // Fill in - initialize stuff and start the director running...

	return 0;
}