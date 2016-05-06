#ifndef GIOTTO_H
#define GIOTTO_H

#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "scheduler.h"
#include "actors.h"
#include "ptides.h"

using namespace std;

class Task{
public:
	Task(string, Actor*);
	string getName(){return name;};
	Actor* getActor(){return myActor;};
private:
	string name;
	Actor* myActor;
};

// Define trivially true guard
bool trivialTrueGuard (vector<Port*>);

class Guard{
public:
	Guard(string, bool (*f)(vector<Port*>), vector<Port*> );
	bool Check();
private:
	string name;
	bool (*GuardFunction)(vector<Port*>);
	vector<Port*> myPorts;
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
	unsigned int frequency;
	unsigned int priority;
	Task *myTask;
	Guard *myGuard;
	SchedulerTask *mySchedTask;
};



class Mode;

class ModeSwitch{
public:
	ModeSwitch(Guard*, Mode* ,Mode*);
	Mode* getSource(){return srcMode;};
	Mode* getDest(){return destMode;};
	Guard* getGuard(){return myGuard;};
private:
	Mode* srcMode;
	Mode* destMode;
	Guard* myGuard;
};

// class ActuatorUpdate{
// public:
// 	ActuatorUpdate(Guard, unsigned int);
// 	void SetFrequency (unsigned int);
// private:
// 	unsigned int frequency;
// 	Guard myGuard;
// };


class Interrupt{
public:
	Interrupt(string, PtidesDirector*);
	//Interrupt(string, SDFDirector*);
	PtidesDirector* getDirector(){return myPtidesDirector;};
	string getName(){return myName;};
	//SDFDirector* getDirector(){return mySDFDirector;};
private:
	string myName;
	PtidesDirector* myPtidesDirector;
};


class InterruptInvocation{
public:
	InterruptInvocation(Interrupt*, Guard*, unsigned int);
	Guard* getGuard(){return myGuard;};
	Interrupt* getInterrupt(){return myInterrupt;};
	unsigned int getPriority(){return priority;};
private:
	Interrupt* myInterrupt;
	Guard* myGuard;
	unsigned int priority;
};

class Mode{
public:
	Mode(string, vector<TaskInvocation*>);
	vector<SchedulerTask*> getScheduledTasks(){return schedTasks;};
	Task* findTask(string);
	string getName(){return name;};
	vector<TaskInvocation*> getInvokes(){return invokes;};
private:
	string name;
	vector<TaskInvocation*> invokes;
	vector<SchedulerTask*> schedTasks;
	// vector<ActuatorUpdate*> updates;
};

// class Config{
// public:
// 	Config( Mode);
// private:
// 	Mode myMode;
// 	std::chrono::milliseconds ModeTime;
// 	std::chrono::milliseconds TimeStamp;
// 	vector<SchedulerTask*> ActiveTasks;
// };

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
	unsigned int ModeSwitchFrequency;
	unsigned int InterruptFrequency;
	vector<SchedulerTask*> enabledTasks;
	vector<SchedulerTask*> activeTasks;
	vector<ModeSwitch*> allTheSwitches;
	vector<InterruptInvocation*> myInterrupts;
	std::chrono::system_clock::time_point startRun;
	std::chrono::system_clock::time_point lastModeSwitch;
	std::chrono::system_clock::time_point lastInterruptTime;
	std::chrono::milliseconds modeTime;
	std::chrono::milliseconds currentTime;
	std::chrono::milliseconds modeSwitchPeriod;
	std::chrono::milliseconds interruptPeriod;
	void invokeNextTask();
	Mode* checkNextMode();
	void updateModeTime();
	void updateActiveTasks();
	void advanceTime();
	void checkForInterrupts();
};

bool sortInterruptsByPriority (InterruptInvocation* lhs, InterruptInvocation* rhs);

#endif
