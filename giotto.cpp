#include <iostream>
#include <vector>
#include <string>
#include <giotto.h>

using namespace std;

//	Giotto components definitions

Port::Port(string nameIn, string typeIn) {
	name = nameIn;
	SetType(typeIn);
	initialValue = 0; // which type?
}

void Port::SetType(string typeIn) {
	type = typeIn;
}

Task::Task(string nameIn) {
	name = nameIn;
}

vector<Port> Task::TaskFunction(vector<Port> pVector) {
	// ...
}

Driver::Driver(string nameIn) {
	name = nameIn;
}

bool Driver::DriverFunction(vector<Port.value> pValVector) {
	// ...
}

bool Driver::Guard(vector<Port.value> pValVector) {
	// ...
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
}

void TaskInvocation::SetFrequency (unsigned int f) {
	frequency = f;
}

SchedulerTask TaskInvocation::GetSchedulerTask() {
	// return in scheduler task format (id and period)
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

Config::Config () {
	// ...
}

GiottoDirector::GiottoDirector() {
	// ...
}

void GiottoDirector::Run(vector<Task> activeTasks) {
	// ...
	vector<Task> scheduledTasks = GenerateSchedule(activeTasks);
	taskList = scheduledTasks;
	RunScheduled();
}

void GiottoDirector::RunScheduled() {
	// ... actual run loop, assumes taskList already in scheduled order
}