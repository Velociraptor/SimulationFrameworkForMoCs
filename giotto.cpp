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
	task_function = f;

}

Task::TaskFunction() {
	task_function();
}

Guard::Guard(string nameIn, bool (*f)()) {
	name = nameIn;
	GuardFunction = f;
}

bool Guard::Check() {
	return GuardFunction();
}

Mode::Mode(string nameIn, vector<TaskInvocation*> t, vector<ModeSwitch*> ms) {
	name = nameIn;
	switches = ms;
	invokes = t;
	//Schedule the tasks
	vector<SchedulerTask> unorderedTasks;
	for (int i = 0; i < activeTasks.size(); ++i)
	{
		unorderedTasks.push_back(invokes->mySchedTask);
	}
	vector<SchedulerTask> scheduledTasks = getSchedule(unorderedTasks);
	vector<SchedulerTask*> refScheduledTasks;
	for (int i = 0; i < scheduledTasks.size(); ++i)
	{
		refScheduledTasks.push_back(&scheduledTasks[i]);
	}
	schedTasks = refScheduledTasks;
}


TaskInvocation::TaskInvocation(Task t, Guard g, unsigned int f) {
	myTask = t;
	myGuard = g;
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

// ActuatorUpdate::ActuatorUpdate(Guard g, unsigned int f) {
// 	myGuard = g;
// 	frequency = f;
// }

// void ActuatorUpdate::SetFrequency (unsigned int f) {
// 	frequency = f;
// }

ModeSwitch::ModeSwitch(Guard g, Mode m, unsigned int f) {
	myGuard = g;
	targetMode = m;
	frequency = f;
}

void ModeSwitch::SetFrequency (unsigned int f) {
	frequency = f;
}

void ModeSwitch::SetTargetMode (Mode m) {
	targetMode = m;
}

// Config::Config (Mode startMode) {
// 	myMode.setStartMode(startMode);
// 	ActiveTasks = NULL;
// 	TimetStamp = new std::chrono::milliseconds(0);
// 	ModeTime = new std::chrono::milliseconds(0);
// }

GiottoDirector::GiottoDirector(Mode* m) {
	startMode = m;
}

void GiottoDirector::Run(std::chrono::milliseconds maxRunTime) {
	while(currentTime < maxRunTime){
		invokeNextTask();
		if (checkMode()){
			updateMode();
			modeTime = 0;
		}
		else
			updateModeTime();
		updateActiveTasks();
		advanceTime();
	}
}

void GiottoDirector::invokeNextTask(){

}

bool GiottoDirector::checkMode(){

}

void GiottoDirector::updateMode(){

}

void GiottoDirector::updateModeTime(){

}

void GiottoDirector::updateActiveTasks(){

}

void GiottoDirector::advanceTime(){

}
