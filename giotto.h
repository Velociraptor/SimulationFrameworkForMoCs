#ifndef GIOTTO_H
#define GIOTTO_H

#include <vector>
#include <string>
using namespace std;

class Port{
public:
	string portName(){return name};
	void setType(string);
private:
	string name;

	union value{
		bool myValue;
		int myValue;
		float myValue;
		double myValue;
	};
	union initalValue{
		bool myValue;
		int myValue;
		float myValue;
		double myValue;
	};

	enum type {SensPort, ActPort, InPort, OutPort, PrivPort };
};

class Task{
private:
	string name;
	std::vector<Port> in;
	std::vector<Port> out;
	std::vector<Port> priv;
public:
	vector<Port> TaskFunction(vector<Port>);
	string portName(){return name};
};

class Driver{
private:
	string name;
	std::vector<Port> src;
	std::vector<Port> dst;
public:
	bool DriverFunction(vector<Port.value>);
	bool guard(vector<Port.value>);
	string portName(){return name};

};

class Mode{
private:
	string name;
	float period;
	vector<Port> modePorts;
	vector<TaskInvocation> invokes;
	vector<ActuatorUpdate> updates;
	vector<ModeSwitch> switches;
	Mode startMode;																													
public:
	void setStartMode(Mode);

};

class TaskInvocation{
private:
	unsigned int frequency;
	Task myTask;
	Driver myDriver;
public:
	void SetFrequency (unsigned int);
};

class ActuatorUpdate{
private:
	unsigned int frequency;
	Driver myDriver;
public:
	void SetFrequency (unsigned int);
};

class ModeSwitch{
private:
	unsigned int frequency;
	Mode targetMode;
	Driver myDriver;
public:
	void SetFrequency (unsigned int);
	void SetTargetMode (Mode);
};


class Config{
private:
	Mode m;
	float ModeTime;
	float TimeStamp;
	vector<Task> ActiveTasks;
};

class GiottoDirector{
private:
	std::vector<Port> portList;
	std::vector<Task> taskList;
	std::vector<Driver> driverList;
	std::vector<Mode> modeList;
public:
	Mode start;
	void run(taskList activeTasks);
	vector<Port> getPortList() {return portList};
	vector<Task> getTaskList() {return taskList};
	vector<Driver> getDriverList() {return driverList};
	vector<Mode> getModeList() {return modeList};
};