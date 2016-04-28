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

bool trivialTrueGuard (vector<Port*>) { return true; }

Guard::Guard(string nameIn, bool (*f)(vector<Port*>), vector<Port*> p) {
	name = nameIn;
	GuardFunction = f;
	myPorts = p;
}

bool Guard::Check() {
	return GuardFunction(myPorts);
}

Mode::Mode(string nameIn, vector<TaskInvocation*> t) {
	name = nameIn;
	invokes = t;
	//Schedule the tasks
	vector<SchedulerTask> unorderedTasks;
	for (unsigned int i = 0; i < invokes.size(); ++i)
	{
		unorderedTasks.push_back(invokes[i]->getSchedulerTask());
	}
	vector<SchedulerTask> scheduledTasks = getSchedule(unorderedTasks);
	vector<SchedulerTask*> refScheduledTasks;
	for (unsigned int i = 0; i < scheduledTasks.size(); ++i)
	{
		refScheduledTasks.push_back(&scheduledTasks[i]);
	}
	schedTasks = refScheduledTasks;
}

Task Mode::findTask(string taskName){
	for (unsigned int i = 0; i < invokes.size(); ++i)
	{
		if(invokes[i]->getTask().getName().compare(taskName)==0)
			return (invokes[i]->getTask());
	}
	printf("No task of that name!\n");
	return Task(taskName,NULL);
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

ModeSwitch::ModeSwitch(Guard* g, Mode* from, Mode* to, unsigned int f) {
	myGuard = g;
	destMode = to;
	srcMode = from;
	frequency = f;
}

void ModeSwitch::SetFrequency (unsigned int f) {
	frequency = f;
}


// Config::Config (Mode startMode) {
// 	myMode.setStartMode(startMode);
// 	ActiveTasks = NULL;
// 	TimetStamp = new std::chrono::milliseconds(0);
// 	ModeTime = new std::chrono::milliseconds(0);
// }

GiottoDirector::GiottoDirector(Mode* m, vector<ModeSwitch*> switches) {
	startMode = m;
	allTheSwitches = switches;
}

void GiottoDirector::Run(std::chrono::milliseconds maxRunTime) {
	currentMode = startMode;
	Mode* nextMode;
	enabledTasks = currentMode->getScheduledTasks();
	activeTasks = enabledTasks;
	while(currentTime < maxRunTime){
		invokeNextTask();
		nextMode = checkNextMode();
		if (nextMode->getName().compare(currentMode->getName()) != 0){
			currentMode = nextMode;
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

Mode* GiottoDirector::checkNextMode(){
	//Check frequency
	for (unsigned int i = 0; i < allTheSwitches.size(); ++i)
	{
		if(allTheSwitches[i]->getSource()->getName().compare(currentMode->getName()) == 0){
			if(allTheSwitches[i]->getGuard()->Check()){
				return allTheSwitches[i]->getDest();
			}
		}
	}
	return currentMode;
}

void GiottoDirector::updateModeTime(){
	
}

void GiottoDirector::updateActiveTasks(){
	activeTasks.erase (activeTasks.begin());
}

void GiottoDirector::advanceTime(){

}
int main(){return 0;}
