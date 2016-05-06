#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include <string>
#include <chrono>

using namespace std;

/* 
Scheduler

Uma Balakrishnan and Hannah Sarver
EE 249B Embedded Systems Spring 2016

Simple periodic Scheduler implementation,
agnostic to any particular model of computation
to provide some level of re-use within our framework.
This Scheduler provides a base SchedulerTask with an
identifier, period, and priority, which can be
generated from a Taask class specific to a certain
MoC and passed into the generic Scheduler to be
prepared in a periodic schedule that sorts tasks by
period and priority and generates a set of active tasks
within a given cycle. For now we make the assumption
that if we set a cycle time based on the minimum task
period then all active tasks can be executed within
one cycle - this is overly simple for some applications
but we feel is a reasonable starting point for this
project.
*/

class SchedulerTask{
public:
	SchedulerTask(string id, std::chrono::milliseconds period, unsigned int priority){
		myID = id;
		myPeriod = period;
		myPriority = priority;
		number_of_calls = 0;
	}
	string getID(){return myID;}
	std::chrono::milliseconds getPeriod(){return myPeriod;};
	unsigned int getPriority(){return myPriority;};
	unsigned long int getCallNum(){return number_of_calls;};
	void incrementCallNum(){number_of_calls++;};
	void resetCallNum(){number_of_calls = 0;};
	bool operator <(const SchedulerTask& s)
      {
         return myPeriod.count()<s.myPeriod.count();
      }
private:
	string myID;
	std::chrono::milliseconds myPeriod;
	unsigned int myPriority;
	unsigned long int number_of_calls;
};


bool sortByPeriod (SchedulerTask *lhs, SchedulerTask *rhs);
bool sortByPriority (SchedulerTask *lhs, SchedulerTask *rhs);

vector<SchedulerTask*> getSchedule(vector<SchedulerTask*> unorderedTasks);

class PrepareSchedule{
public:
	PrepareSchedule(vector<SchedulerTask*>, std::chrono::system_clock::time_point);
	void calculateCycleTime(vector<SchedulerTask*>);
	vector<SchedulerTask*> RecalculateActiveTasks(std::chrono::system_clock::time_point);
	void setStartTime(std::chrono::system_clock::time_point);
	void resetAllTaskCalls();
private:

	std::chrono::milliseconds cycleTime;
	vector<SchedulerTask*> enabledTasks;
	std::chrono::system_clock::time_point startTime;
};

#endif