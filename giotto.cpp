#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <algorithm>
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
	vector<SchedulerTask*> unorderedTasks;
	for (unsigned int i = 0; i < invokes.size(); ++i)
	{
		unorderedTasks.push_back(invokes[i]->getSchedulerTask());
	}
	vector<SchedulerTask*> scheduledTasks = getSchedule(unorderedTasks);
	schedTasks = scheduledTasks;
}

Task* Mode::findTask(string taskName){
	for (unsigned int i = 0; i < invokes.size(); ++i)
	{
		if(invokes[i]->getTask()->getName().compare(taskName)==0)
			return (invokes[i]->getTask());
	}	
	cout<<"No task of that name!\n";
	return (new Task(taskName,NULL));
}

TaskInvocation::TaskInvocation(Task* t, Guard* g, unsigned int f, unsigned int p) {
	myTask = t;
	myGuard = g;
	frequency = f;
	priority = p;
	int period = (int)1000/frequency;
	std::chrono::milliseconds m(period);
	mySchedTask = new SchedulerTask(myTask->getName(), m, p);
}

void TaskInvocation::SetFrequency (unsigned int f) {
	frequency = f;
}

std::chrono::milliseconds TaskInvocation::getPeriod(){
	int period = (int)1000/frequency;
	std::chrono::milliseconds m(period);
	return m;
}

// ActuatorUpdate::ActuatorUpdate(Guard g, unsigned int f) {
// 	myGuard = g;
// 	frequency = f;
// }

// void ActuatorUpdate::SetFrequency (unsigned int f) {
// 	frequency = f;
// }

ModeSwitch::ModeSwitch(Guard* g, Mode* from, Mode* to) {
	myGuard = g;
	destMode = to;
	srcMode = from;
}


// Config::Config (Mode startMode) {
// 	myMode.setStartMode(startMode);
// 	ActiveTasks = NULL;
// 	TimetStamp = new std::chrono::milliseconds(0);
// 	ModeTime = new std::chrono::milliseconds(0);
// }
Interrupt::Interrupt(string name, PtidesDirector* pd){
	myName = name;
	myPtidesDirector = pd;
}

InterruptInvocation::InterruptInvocation(Interrupt* i, Guard* g, unsigned int p){
	myInterrupt = i;
	myGuard = g;
	priority = p;
}

bool sortInterruptsByPriority (InterruptInvocation* lhs, InterruptInvocation* rhs) {return lhs->getPriority() < rhs->getPriority();}

GiottoDirector::GiottoDirector(Mode* m, vector<ModeSwitch*> switches, unsigned int mfreq, vector<InterruptInvocation*> interrupts, unsigned int ifreq) {
	startMode = m;
	allTheSwitches = switches;
	currentMode = m;
	ModeSwitchFrequency = mfreq;
	int period = (int)1000/ModeSwitchFrequency;
	std::chrono::milliseconds ms(period);
	modeSwitchPeriod = ms;
	myInterrupts = interrupts;
	InterruptFrequency = ifreq;
	int iperiod = (int)1000/InterruptFrequency;
	std::chrono::milliseconds msi(iperiod);
	interruptPeriod = msi;
}

void GiottoDirector::Run(std::chrono::milliseconds maxRunTime) {
	sort(myInterrupts.begin(), myInterrupts.end(), sortInterruptsByPriority);
	startRun = std::chrono::system_clock::now();
	currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startRun);
	lastModeSwitch = startRun;
	currentMode = startMode; 
	lastInterruptTime = startRun;
	Mode* nextMode;
	enabledTasks = currentMode->getScheduledTasks();
	PrepareSchedule mySchedule = PrepareSchedule(enabledTasks, startRun);
	std::chrono::system_clock::time_point cycleStart = std::chrono::system_clock::now();
	mySchedule.calculateCycleTime(enabledTasks);
	activeTasks = mySchedule.RecalculateActiveTasks(cycleStart);

	while (currentTime.count() < maxRunTime.count()) {	
		if (activeTasks.size() == 0)
		{
			cout << "Recalculating active tasks for next cycle" << endl;
			activeTasks = mySchedule.RecalculateActiveTasks(cycleStart);
			cycleStart = std::chrono::system_clock::now();
			if(activeTasks.size() == 0)
				cout<<"No Tasks!!!!!! This is the end."<<endl;
		}
		
		invokeNextTask();
		updateActiveTasks();
		if(modeTime.count() >= modeSwitchPeriod.count()){
			nextMode = checkNextMode();
			lastModeSwitch = std::chrono::system_clock::now();
			if (nextMode->getName().compare(currentMode->getName()) != 0){
				cout << "Changing Mode to " << nextMode->getName() << endl;
				currentMode = nextMode;
				enabledTasks = currentMode->getScheduledTasks();
				mySchedule.calculateCycleTime(enabledTasks);
				mySchedule.setStartTime(std::chrono::system_clock::now());
				cycleStart = std::chrono::system_clock::now();
				activeTasks = mySchedule.RecalculateActiveTasks(cycleStart);
				mySchedule.resetAllTaskCalls();
			}
		}
		else
			updateModeTime();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastInterruptTime)>= interruptPeriod)
			checkForInterrupts();
		advanceTime();
	}
}

void GiottoDirector::invokeNextTask(){
	activeTasks[0]->incrementCallNum();
	string curr_task_name = activeTasks[0]->getID();
	cout << "Invoking task " << curr_task_name << endl;
	Task* curr_task = currentMode->findTask(curr_task_name);
	curr_task->getActor()->Compute();
}

Mode* GiottoDirector::checkNextMode(){
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
	modeTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastModeSwitch);	
}

void GiottoDirector::updateActiveTasks(){
	activeTasks.erase (activeTasks.begin());
}

void GiottoDirector::advanceTime(){
	currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startRun);
}

std::chrono::milliseconds GiottoDirector::getModeSwitchPeriod(){
	return modeSwitchPeriod;
}

void GiottoDirector::checkForInterrupts(){
	for (unsigned int i = 0; i < myInterrupts.size(); ++i)
	{
		if(myInterrupts[i]->getGuard()->Check())
			myInterrupts[i]->getInterrupt()->getDirector()->Run();
		
	}
	lastInterruptTime = chrono::system_clock::now();
	return;
}
