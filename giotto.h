#ifndef GIOTTO_H
#define GIOTTO_H

#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <ctime>
#include "scheduler.h"
#include "actors.h"
#include "ptides.h"

using namespace std;

/* 
Giotto Model of Computation

Uma Balakrishnan and Hannah Sarver
EE 249B Embedded Systems Spring 2016

Basic Giotto class design based on the 
Giotto model of computation as presented in:
 https://www.hh.se/download/18.70cf2e49129168da0158000145742/henzinger-giotto-time-triggered.pdf

Primary components include Tasks which are
included in periodic TaskInvocations, which are in
turn incorporated into Modes of operation
that the Director runs through continuously,
running tasks according to their periods
(as scheduled by our external MoC-agnostic periodic Scheduler).
The Director also handles Mode switching, based on checking Guards.
Additionally, we have introduced a concept of
Interrupts that can be checked within Giotto's periodic-based
Director and kick off an internal event-based MoC execution
(for instance PTIDES).
*/


// Actors are encapsulated in Tasks
class Task{
public:
	Task(string, Actor*);
	string getName(){return name;};
	Actor* getActor(){return myActor;};
private:
	string name; //Task name
	Actor* myActor; //Actor associated with Task
};

// Function that serves as a guard that always evaluates to true
bool trivialTrueGuard (vector<Port*>);

class Guard{
public:
	Guard(string, bool (*f)(vector<Port*>), vector<Port*> );
	bool Check(); //This funciton encapsulates the private guard function
private:
	string name; // Guard name
	bool (*GuardFunction)(vector<Port*>); // The guard function that takes a vector of ports as an argument and evaluates to true or false
	vector<Port*> myPorts; //The vector of ports used by the guard function
};

class TaskInvocation{
public:
	TaskInvocation(Task*, Guard*, unsigned int, unsigned int); 
	void SetFrequency (unsigned int);
	SchedulerTask* getSchedulerTask(){return mySchedTask;};
	Task* getTask(){return myTask;}
	std::chrono::milliseconds getPeriod();
	unsigned int getPriority(){return priority;};
private:
	unsigned int frequency; //Frequency at which the Task needs to be invoked
	unsigned int priority; // Priority of the task
	Task *myTask; 
	Guard *myGuard; //The guard which decides whether the task must be invoked
	SchedulerTask *mySchedTask;
};



class Mode;

class ModeSwitch{
public:
	ModeSwitch(Guard*, Mode*, Mode*);
	Mode* getSource(){return srcMode;};
	Mode* getDest(){return destMode;};
	Guard* getGuard(){return myGuard;};
private:
	Mode* srcMode; // Source Mode
	Mode* destMode; // Destination Mode
	Guard* myGuard; //Guard which decides whether the mode must be switched
};

class Interrupt{
public:
	Interrupt(string, PtidesDirector*);
	//Interrupt(string, SDFDirector*);  //Could have a number of constructors depending on the type of event based interrupt
	PtidesDirector* getDirector(){return myPtidesDirector;};
	//SDFDirector* getDirector(){return mySDFDirector;};
	string getName(){return myName;};
private:
	string myName; //Name of the interrupt

	//Provide member directors for any number of event-based MoCs
	PtidesDirector* myPtidesDirector; 
	//SDF Director* mySDF Director;
};


class InterruptInvocation{
public:
	InterruptInvocation(Interrupt*, Guard*, unsigned int);
	Guard* getGuard(){return myGuard;};
	Interrupt* getInterrupt(){return myInterrupt;};
	unsigned int getPriority(){return priority;};
private:
	Interrupt* myInterrupt; 
	Guard* myGuard; //Guard which decides if the interrrupt should be invoked
	unsigned int priority; //Priority of the interrupt
};

class Mode{
public:
	Mode(string, vector<TaskInvocation*>);
	vector<SchedulerTask*> getScheduledTasks(){return schedTasks;};
	Task* findTask(string);
	string getName(){return name;};
	vector<TaskInvocation*> getInvokes(){return invokes;};
private:
	string name; //Mode name
	vector<TaskInvocation*> invokes; //Set of task invocations in this mode
	vector<SchedulerTask*> schedTasks; //Set of scheduler tasks corresponding to the invokes in the mode
};

// Giotto Director class contains the information about the simulation needed by the MoC
class GiottoDirector{
public:
	GiottoDirector(Mode*, vector<ModeSwitch*>, unsigned int, vector<InterruptInvocation*>, unsigned int);
	void Run(std::chrono::milliseconds);
	vector<SchedulerTask*> GetEnabledTaskList() {return enabledTasks;};
	vector<SchedulerTask*> GetActiveTaskList() {return activeTasks;};
	vector<ModeSwitch*> GetModeSwitches(){return allTheSwitches;};
	std::chrono::milliseconds getModeSwitchPeriod();

private:
	Mode* startMode; 
	Mode* currentMode;
	unsigned int ModeSwitchFrequency; // How often we need to check if the mode must be updated
	unsigned int InterruptFrequency; // How often we need to check if an interrupt must be invoked
	vector<SchedulerTask*> enabledTasks; // List of currently enabled tasks
	vector<SchedulerTask*> activeTasks; // List of currently active tasks
	vector<ModeSwitch*> allTheSwitches; // List of all the possible mode switches
	vector<InterruptInvocation*> myInterrupts; // List of all possible interrupt that may need to be called
	std::chrono::system_clock::time_point startRun; // Time at which the simulation started
	std::chrono::system_clock::time_point lastModeSwitch; // Time at which the last checked if the mode needs to be switched
	std::chrono::system_clock::time_point lastInterruptTime; // Time at which the last interrupt check happened
	std::chrono::milliseconds modeTime; // Time since last mode switch check
	std::chrono::milliseconds currentTime; // Time since the start of the simulation
	std::chrono::milliseconds modeSwitchPeriod; // How often we need to check if the mode must be updated
	std::chrono::milliseconds interruptPeriod; // How often we need to check if an interrupt must be invoke
	void invokeNextTask();
	Mode* checkNextMode();
	void updateModeTime();
	void updateActiveTasks();
	void advanceTime();
	void checkForInterrupts();
};

bool sortInterruptsByPriority (InterruptInvocation* lhs, InterruptInvocation* rhs); 

#endif
