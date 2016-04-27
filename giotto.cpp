#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "giotto.h"
#include "scheduler.h"
#include "actor.h"

using namespace std;

//	Giotto components definitions

Task::Task(string nameIn,  void (*f)() ) {
	name = nameIn;
	TaskFunction = f;

}

vector<Port*> Task::TaskFunction(vector<Port*> pVector) {
	src = pVector;
	return dst;
}

Driver::Driver(string nameIn) {
	name = nameIn;
}

bool Driver::DriverFunction(vector<Port*> pValVector) {
	return TRUE;
}

bool Driver::Guard(vector<Port*> pValVector) {
	return TRUE;
}

Mode::Mode(string nameIn) {
	name = nameIn;
}

void Mode::SetStartMode(Mode m) {
	startMode = m;
}

TaskInvocation::TaskInvocation(Task t, Driver d, unsigned int f) {
	myTask = t;
	myDriver = d;
	frequency = f;
	mySchedTask = GetSchedulerTask();
}

void TaskInvocation::SetFrequency (unsigned int f) {
	frequency = f;
}

SchedulerTask TaskInvocation::GetSchedulerTask() {
	// return in scheduler task format (id and period)
	int period = (int)1000/frequency;
	std::chrono::milliseconds m(period);
	SchedulerTask myShedulerTask = new SchedulerTask( myTask.name, m);
	return myShedulerTask;
}

ActuatorUpdate::ActuatorUpdate(Driver d, unsigned int f) {
	myDriver = d;
	frequency = f;
}

void ActuatorUpdate::SetFrequency (unsigned int f) {
	frequency = f;
}

ModeSwitch::ModeSwitch(Driver d, Mode m, unsigned int f) {
	myDriver = d;
	targetMode = m;
	frequency = f;
}

void ModeSwitch::SetFrequency (unsigned int f) {
	frequency = f;
}

void ModeSwitch::SetTargetMode (Mode m) {
	targetMode = m;
}

Config::Config (Mode startMode, vector<Port*> p) {
	myMode.setStartMode(startMode);
	UsedPorts = p;
	ActiveTasks = NULL;
	TimetStamp = new std::chrono::milliseconds(0);
	ModeTime = new std::chrono::milliseconds(0);
}

GiottoDirector::GiottoDirector(Config c) {
	start = c.myMode;
	portList = c.UsedPorts;
}

void GiottoDirector::Run(vector<TaskInvocation*> activeTasks) {
	// ...
	vector<SchedulerTask> unorderedTasks;
	for (int i = 0; i < activeTasks.size(); ++i)
	{
		unorderedTasks.push_back(activeTasks->mySchedTask);
	}
	vector<SchedulerTask> scheduledTasks = getSchedule(unorderedTasks);
	vector<SchedulerTask*> refScheduledTasks;
	for (int i = 0; i < scheduledTasks.size(); ++i)
	{
		refScheduledTasks.push_back(&scheduledTasks[i]);
	}
	taskList = refScheduledTasks;
	RunScheduled();
}

void GiottoDirector::RunScheduled() {
	// ... actual run loop, assumes taskList already in scheduled order

}