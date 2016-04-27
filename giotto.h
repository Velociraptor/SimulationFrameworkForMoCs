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
	Task(string, vector<Port*> (*f)(vector<Port*>));
	vector<Port*> TaskFunction(vector<Port*>);
private:
	string name;
	std::vector<Port*> in;
	std::vector<Port*> out;
	std::vector<Port*> priv;
};

class Driver{
public:
	Driver(string);
	bool DriverFunction(vector<Port*>);
	bool Guard(vector<Port*>);
private:
	string name;
	std::vector<Port*> src;
	std::vector<Port*> dst;
};

class Mode{
public:
	Mode(string);
	void SetStartMode(Mode);
private:
	string name;
	float period;
	vector<Port*> modePorts;
	vector<TaskInvocation*> invokes;
	vector<ActuatorUpdate*> updates;
	vector<ModeSwitch*> switches;
	Mode startMode;
};

class TaskInvocation{
public:
	TaskInvocation(Task, Driver, unsigned int);
	void SetFrequency (unsigned int);
	SchedulerTask GetSchedulerTask();
private:
	unsigned int frequency;
	Task myTask;
	Driver myDriver;
	SchedulerTask mySchedTask;
};

class ActuatorUpdate{
public:
	ActuatorUpdate(Driver, unsigned int);
	void SetFrequency (unsigned int);
private:
	unsigned int frequency;
	Driver myDriver;
};

class ModeSwitch{
public:
	ModeSwitch(Driver, Mode, unsigned int);
	void SetFrequency (unsigned int);
	void SetTargetMode (Mode);
private:
	unsigned int frequency;
	Mode targetMode;
	Driver myDriver;
};


class Config{
public:
	Config( Mode, vector<Port*>);
private:
	Mode myMode;
	std::chrono::milliseconds ModeTime;
	std::chrono::milliseconds TimeStamp;
	vector<Port*> UsedPorts;
	vector<SchedulerTask*> ActiveTasks;
};

class GiottoDirector{
public:
	GiottoDirector(Config c);
	Mode start;
	void Run(vector<TaskInvocation*>);
	vector<Port*> GetPortList() {return portList;};
	vector<SchedulerTask*> GetTaskList() {return taskList;};
	vector<Driver*> GetDriverList() {return driverList;};
	vector<Mode*> GetModeList() {return modeList;};
private:
	// vector<Task*> GenerateSchedule(vector<Task*>);
	void RunScheduled();
	vector<SchedulerTask*> taskList;
	vector<Driver*> driverList;
	vector<Mode*> modeList;
	vector<Port*> portList;
};

#endif
