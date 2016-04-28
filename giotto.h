#ifndef GIOTTO_H
#define GIOTTO_H

#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "scheduler.h"
#include "actors.h"

using namespace std;

class Task{
public:
	Task(string, void (*f)(void));
	void TaskFunction(void);
private:
	string name;
	void *TaskFunction;
};

class Guard
public:
	Guard(string, bool (*f)());
	bool Check();
private:
	string name;
	void *GuardFunction;
};

class Mode{
public:
	Mode(string, vector<TaskInvocation*>);
	getScheduledTasks(){return schedTasks;};
private:
	string name;
	std::chrono::milliseconds period;
	vector<TaskInvocation*> invokes;
	vector<SchedulerTask*> schedTasks;
	// vector<ActuatorUpdate*> updates;
	vector<ModeSwitch*> switches;
};

class TaskInvocation{
public:
	TaskInvocation(Task, Guard, unsigned int);
	void SetFrequency (unsigned int);
	SchedulerTask GetSchedulerTask();
private:
	unsigned int frequency;
	Task myTask;
	Guard myGuard;
	SchedulerTask mySchedTask;
};

// class ActuatorUpdate{
// public:
// 	ActuatorUpdate(Guard, unsigned int);
// 	void SetFrequency (unsigned int);
// private:
// 	unsigned int frequency;
// 	Guard myGuard;
// };

class ModeSwitch{
public:
	ModeSwitch(Guard, Mode, unsigned int);
	void SetFrequency (unsigned int);
	void SetTargetMode (Mode);
private:
	unsigned int frequency;
	Mode targetMode;
	Guard myGuard;
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
	GiottoDirector(Mode*);
	void Run();
	vector<SchedulerTask*> GetEnabledTaskList() {return enabledTasks;};

	void invokeNextTask();
	void checkMode();
	void updateMode();
	void updateModeTime();
	void updateActiveTasks();
	void advanceTime();
private:

	void RunScheduled();
	vector<SchedulerTask*> enabledTasks;
	vector<SchedulerTask*> activeTasks;
	std::chrono::milliseconds modeTime;
	std::chrono::milliseconds currentTime;

};

#endif
