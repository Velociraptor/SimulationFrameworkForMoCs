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

// Constructor for Task
Task::Task(string nameIn,  Actor* a ) {
	name = nameIn;
	myActor = a;
}
// Trivial guard for functions that should always happen
bool trivialTrueGuard (vector<Port*>) { return true; }

// Constructor for Guard
Guard::Guard(string nameIn, bool (*f)(vector<Port*>), vector<Port*> p) {
	name = nameIn;
	GuardFunction = f;
	myPorts = p;
}

// Function to run the guard function
// Returns true if check passes and false if it does not
bool Guard::Check() {
	return GuardFunction(myPorts);
}

//Constructor for ModeSwitch
ModeSwitch::ModeSwitch(Guard* g, Mode* from, Mode* to){
	myGuard = g;
	srcMode = from;
	destMode = to;
}


// Contstructor for Mode
Mode::Mode(string nameIn, vector<TaskInvocation*> t) {
	name = nameIn;
	invokes = t;
	// Get each task in the form of a SchedulerTask so that it can be scheduled while running the simulation
	vector<SchedulerTask*> unorderedTasks;
	for (unsigned int i = 0; i < invokes.size(); ++i)
	{
		unorderedTasks.push_back(invokes[i]->getSchedulerTask());
	}
	// Order the tasks first by period and then by priority
	vector<SchedulerTask*> scheduledTasks = getSchedule(unorderedTasks);
	schedTasks = scheduledTasks;
}

// Function to find the Giotto Task that corresponds to the given SchedulerTask
// Returns the corresponding Task if it exists
Task* Mode::findTask(string taskName){
	for (unsigned int i = 0; i < invokes.size(); ++i)
	{
		if(invokes[i]->getTask()->getName().compare(taskName)==0)
			return (invokes[i]->getTask());
	}	
	cout<<"No task of that name!\n";  //Task not found
	return (new Task(taskName,NULL)); //Return an empty task with that name
}

//Constructor for TaskInvocation
TaskInvocation::TaskInvocation(Task* t, Guard* g, unsigned int f, unsigned int p) {
	myTask = t;
	myGuard = g;
	frequency = f;
	priority = p;
	int period = (int)1000/frequency;  // Get period from Task Invocation frequency
	std::chrono::milliseconds m(period);
	mySchedTask = new SchedulerTask(myTask->getName(), m, p); // Create a Scheduler Task for each invocation
}

void TaskInvocation::SetFrequency (unsigned int f) {
	frequency = f;
}
//Function to return the period of the Task Invocation from the frequency
std::chrono::milliseconds TaskInvocation::getPeriod(){
	int period = (int)1000/frequency;
	std::chrono::milliseconds m(period);
	return m;
}

// Constructor for Interrupts
Interrupt::Interrupt(string name, PtidesDirector* pd){
	myName = name;
	myPtidesDirector = pd;
}

//Constructor for InterruptInvocation
InterruptInvocation::InterruptInvocation(Interrupt* i, Guard* g, unsigned int p){
	myInterrupt = i;
	myGuard = g;
	priority = p;
}

// Helper function to sort a vector of interrupts according to their priority
bool sortInterruptsByPriority (InterruptInvocation* lhs, InterruptInvocation* rhs) {return lhs->getPriority() < rhs->getPriority();}

// Constructor for GiottoDirector
GiottoDirector::GiottoDirector(Mode* m, vector<ModeSwitch*> switches, unsigned int mfreq, vector<InterruptInvocation*> interrupts, unsigned int ifreq) {
	startMode = m;
	allTheSwitches = switches;
	currentMode = m;
	myInterrupts = interrupts;
	//Set mode switch period from the mode switch frequency
	ModeSwitchFrequency = mfreq;
	int period = (int)1000/ModeSwitchFrequency;
	std::chrono::milliseconds ms(period);
	modeSwitchPeriod = ms;
	//Set the interrupt check period from the interrupt frequency 
	InterruptFrequency = ifreq;
	int iperiod = (int)1000/InterruptFrequency;
	std::chrono::milliseconds msi(iperiod);
	interruptPeriod = msi;
}

// Run function! 
// Input parameter: maxRunTime - the maximum time for which the simulation needs to run
void GiottoDirector::Run(std::chrono::milliseconds maxRunTime) {
	sort(myInterrupts.begin(), myInterrupts.end(), sortInterruptsByPriority); // sort interrupt invocations according their priority
	startRun = std::chrono::system_clock::now();
	currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startRun); //update the current time
	lastModeSwitch = startRun;
	currentMode = startMode; 
	lastInterruptTime = startRun;
	Mode* nextMode;
	enabledTasks = currentMode->getScheduledTasks(); // Get all the taks for the current mode
	PrepareSchedule mySchedule = PrepareSchedule(enabledTasks, startRun); //Create Prepare Scheduler object
	std::chrono::system_clock::time_point cycleStart = std::chrono::system_clock::now(); // update cycle start time
	mySchedule.calculateCycleTime(enabledTasks); //calculate the cycle time for scheduling tasks
	activeTasks = mySchedule.RecalculateActiveTasks(cycleStart); //find the tasks to run in the currrent cycle

	while (currentTime.count() < maxRunTime.count()) { 	
		if (activeTasks.size() == 0) 
		{
			cout << "Recalculating active tasks for next cycle" << endl;
			activeTasks = mySchedule.RecalculateActiveTasks(cycleStart); //find the tasks to run in the next cycle
			cycleStart = std::chrono::system_clock::now(); //update cycle time
			if(activeTasks.size() == 0)
				cout<<"No Tasks!!!!!! This is the end."<<endl;
		}
		
		invokeNextTask(); // Run the next task
		updateActiveTasks(); //Remove executed task from list of active tasks
		if(modeTime.count() >= modeSwitchPeriod.count()){  // Is it time to check for mode switches?
			nextMode = checkNextMode(); 
			lastModeSwitch = std::chrono::system_clock::now();
			if (nextMode->getName().compare(currentMode->getName()) != 0){   //Is the next mode the same as the current mode?
				cout << "Changing Mode to " << nextMode->getName() << endl;
				currentMode = nextMode;		//Switch modes
				enabledTasks = currentMode->getScheduledTasks(); //Get set of tasks for current mad
				mySchedule.calculateCycleTime(enabledTasks); //Calculate cycle time for this mode
				mySchedule.setStartTime(std::chrono::system_clock::now()); //Update the start time for the new mode
				cycleStart = std::chrono::system_clock::now(); // update cycle start time
				activeTasks = mySchedule.RecalculateActiveTasks(cycleStart);
				mySchedule.resetAllTaskCalls(); // Reset all the task calls numbers to 0
			}
		}
		else
			updateModeTime();
		if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastInterruptTime)>= interruptPeriod) //Is it time to check for interrupts?
			checkForInterrupts();
		advanceTime();
	}
}

//Function to execute the next task
void GiottoDirector::invokeNextTask(){
	activeTasks[0]->incrementCallNum(); //Increment the number of calls for this task
	string curr_task_name = activeTasks[0]->getID();
	cout << "Invoking task " << curr_task_name << endl;
	Task* curr_task = currentMode->findTask(curr_task_name); //Find the Giotto task that corresponds to the Scheduler Task
	curr_task->getActor()->Compute(); // Call the corresponding Actors compute function
}

// Function to check which mode must run next
// Returns the next mode if it exists, otherwise returns the same mode, implying that the mode does not change
Mode* GiottoDirector::checkNextMode(){
	for (unsigned int i = 0; i < allTheSwitches.size(); ++i)
	{
		if(allTheSwitches[i]->getSource()->getName().compare(currentMode->getName()) == 0){
			if(allTheSwitches[i]->getGuard()->Check()){ //If the Guard for the Mode Switch is satisfied
				return allTheSwitches[i]->getDest();
			}
		}
	}
	return currentMode;
}

// Function to update the time since since the last mode switch
void GiottoDirector::updateModeTime(){
	modeTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastModeSwitch);	
}

// Function to remove the last executed task from the list of active tasks
void GiottoDirector::updateActiveTasks(){
	activeTasks.erase (activeTasks.begin());
}

// Function to advance time since the start of the simulation
void GiottoDirector::advanceTime(){
	currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startRun);
}
// Function to get return the Giotto Directors mode switch period
std::chrono::milliseconds GiottoDirector::getModeSwitchPeriod(){
	return modeSwitchPeriod;
}
// Function to check all interrupts to see if one of them needs to be invoked
void GiottoDirector::checkForInterrupts(){
	for (unsigned int i = 0; i < myInterrupts.size(); ++i)
	{
		if(myInterrupts[i]->getGuard()->Check()) //Is the guard function true?
			myInterrupts[i]->getInterrupt()->getDirector()->Run(); //Invoke the concerned interrupt
		
	}
	lastInterruptTime = chrono::system_clock::now(); 
	return;
}
