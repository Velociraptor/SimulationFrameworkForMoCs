#ifndef GIOTTO_H
#define GIOTTO_H

#include <vector>
#include <string>
#include "scheduler.h"
using namespace std;

class Port{
public:
	Port(string, string);
	string Name(){return name};
	void SetType(string);
private:
	string name;

	union value{
		bool myValue;
		int myValue;
		float myValue;
		double myValue;
	};
	union initialValue{
		bool myValue;
		int myValue;
		float myValue;
		double myValue;
	};

	enum type {SensPort, ActPort, InPort, OutPort, PrivPort };
};

class Task{
public:
	Task(string);
	vector<Port> TaskFunction(vector<Port>);
	string Name(){return name};
private:
	string name;
	std::vector<Port> in;
	std::vector<Port> out;
	std::vector<Port> priv;
};

class Driver{
public:
	Driver(string);
	bool DriverFunction(vector<Port.value>);
	bool Guard(vector<Port.value>);
	string Name(){return name};
private:
	string name;
	std::vector<Port> src;
	std::vector<Port> dst;
};

class Mode{
public:
	Mode(string);
	void SetStartMode(Mode);
	string Name(){return name};
private:
	string name;
	float period;
	vector<Port> modePorts;
	vector<TaskInvocation> invokes;
	vector<ActuatorUpdate> updates;
	vector<ModeSwitch> switches;
	Mode startMode;
};

class TaskInvocation{
public:
	TaskInvocation(Task, Driver, unsigned int);
	void SetFrequency (unsigned int);
private:
	unsigned int frequency;
	Task myTask;
	Driver myDriver;
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
	Config();
private:
	Mode m;
	float ModeTime;
	float TimeStamp;
	vector<Task> ActiveTasks;
};

class GiottoDirector{
public:
	GiottoDirector(& Scheduler);
	Mode start;
	void Run(vector<Task>);
	vector<Port> GetPortList() {return portList};
	vector<Task> GetTaskList() {return taskList};
	vector<Driver> GetDriverList() {return driverList};
	vector<Mode> GetModeList() {return modeList};
private:
	Scheduler myScheduler;
	vector<Port> portList;
	vector<Task> taskList;
	vector<Driver> driverList;
	vector<Mode> modeList;
};

#endif
