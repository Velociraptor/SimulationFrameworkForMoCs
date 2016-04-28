#ifndef GIOTTO_H
#define GIOTTO_H

#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "scheduler.h"

using namespace std;

class Task{
public:
	Task(string, void (*f)(void));
	void TaskFunction(void);
	string getName(){return name;};
private:
	string name;
	void *task_function;
};

class Guard{
public:
	Guard(string, bool (*f)());
	bool Check();
private:
	string name;
	bool GuardFunction;
};

class TaskInvocation{
public:
	TaskInvocation(Task, Guard, unsigned int);
	void SetFrequency (unsigned int);
	SchedulerTask getSchedulerTask(){return mySchedTask;};
	Task getTask(){return myTask;}
private:
	unsigned int frequency;
	Task myTask;
	Guard myGuard;
	SchedulerTask mySchedTask;
};



class Mode;

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

// class ActuatorUpdate{
// public:
// 	ActuatorUpdate(Guard, unsigned int);
// 	void SetFrequency (unsigned int);
// private:
// 	unsigned int frequency;
// 	Guard myGuard;
// };

class Mode{
public:
	Mode(string, vector<TaskInvocation*>, vector<ModeSwitch*>);
	vector<SchedulerTask*> getScheduledTasks(){return schedTasks;};
	Task findTask(string);
private:
	string name;
	std::chrono::milliseconds period;
	vector<TaskInvocation*> invokes;
	vector<SchedulerTask*> schedTasks;
	// vector<ActuatorUpdate*> updates;
	vector<ModeSwitch*> switches;
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
	void Run(std::chrono::milliseconds);
	vector<SchedulerTask*> GetEnabledTaskList() {return enabledTasks;};
	vector<SchedulerTask*> GetActiveTaskList() {return activeTasks;};

private:

	Mode* startMode;
	Mode* currentMode;
	vector<SchedulerTask*> enabledTasks;
	vector<SchedulerTask*> activeTasks;
	std::chrono::milliseconds modeTime;
	std::chrono::milliseconds currentTime;
	void invokeNextTask();
	bool checkMode();
	void updateMode();
	void updateModeTime();
	void updateActiveTasks();
	void advanceTime();

};

#endif
