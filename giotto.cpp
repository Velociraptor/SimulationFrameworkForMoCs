#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "giotto.h"
#include "scheduler.h"
#include "actors.h"

using namespace std;

//	Giotto components definitions

Task::Task(string nameIn,  Actor* a ) {
	name = nameIn;
	myActor = a;
}

Guard::Guard(string nameIn, bool (*f)(vector<Port*>), vector<Port*> p) {
	name = nameIn;
	GuardFunction = f;
	myPorts = p;
}

bool Guard::Check() {
	return GuardFunction(myPorts);
}

Mode::Mode(string nameIn, vector<TaskInvocation*> t, vector<ModeSwitch*> ms) {
	name = nameIn;
	switches = ms;
	invokes = t;
	//Schedule the tasks
	vector<SchedulerTask> unorderedTasks;
	for (int i = 0; i < invokes.size(); ++i)
	{
		unorderedTasks.push_back(invokes[i]->getSchedulerTask());
	}
	vector<SchedulerTask> scheduledTasks = getSchedule(unorderedTasks);
	vector<SchedulerTask*> refScheduledTasks;
	for (int i = 0; i < scheduledTasks.size(); ++i)
	{
		refScheduledTasks.push_back(&scheduledTasks[i]);
	}
	schedTasks = refScheduledTasks;
}

Task Mode::findTask(string taskName){
	for (int i = 0; i < invokes.size(); ++i)
	{
		if(invokes[i]->getTask().getName().compare(taskName)==0)
			return (invokes[i]->getTask());
	}
	printf("No task of that name!\n");
}

TaskInvocation::TaskInvocation(Task* t, Guard* g, unsigned int f) {
	myTask = t;
	myGuard = g;
	frequency = f;
	int period = (int)1000/frequency;
	std::chrono::milliseconds m(period);
	mySchedTask = new SchedulerTask(myTask->getName(), m);
}

void TaskInvocation::SetFrequency (unsigned int f) {
	frequency = f;
}

// ActuatorUpdate::ActuatorUpdate(Guard g, unsigned int f) {
// 	myGuard = g;
// 	frequency = f;
// }

// void ActuatorUpdate::SetFrequency (unsigned int f) {
// 	frequency = f;
// }

ModeSwitch::ModeSwitch(Guard* g, Mode* m, unsigned int f) {
	myGuard = g;
	targetMode = m;
	frequency = f;
}

void ModeSwitch::SetFrequency (unsigned int f) {
	frequency = f;
}

void ModeSwitch::SetTargetMode (Mode* m) {
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
	currentMode = startMode;
	enabledTasks = currentMode->getScheduledTasks();
	activeTasks = enabledTasks;
	while(currentTime < maxRunTime){
		invokeNextTask();
		if (checkMode()){
			updateMode();
			// modeTime = new std::chrono::milliseconds(0);
		}
		else
			updateModeTime();
		updateActiveTasks();
		advanceTime();
	}
}

void GiottoDirector::invokeNextTask(){
	string curr_task_name = activeTasks[0]->getID();
	Task curr_task = currentMode->findTask(curr_task_name);
	curr_task.getActor()->Compute();
}

bool GiottoDirector::checkMode(){
	vector<ModeSwitch*> possible_switches = currentMode->getSwitches();
	//Check frequency
	for (int i = 0; i < possible_switches.size(); ++i)
	{
		return (possible_switches[i]->getGuard()->Check());
	}
	return false;
}

void GiottoDirector::updateMode(){

}

void GiottoDirector::updateModeTime(){

}

void GiottoDirector::updateActiveTasks(){

}

void GiottoDirector::advanceTime(){

}
