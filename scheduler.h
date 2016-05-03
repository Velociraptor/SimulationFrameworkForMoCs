#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include <string>
#include <chrono>

using namespace std;

class SchedulerTask{
public:
	SchedulerTask(string id, std::chrono::microseconds period, unsigned int priority){
		myID = id;
		myPeriod = period;
		myPriority = priority;
	}
	string getID(){return myID;}
	std::chrono::microseconds getPeriod(){return myPeriod;};
	unsigned int getPriority(){return myPriority;};
	bool operator <(const SchedulerTask& s)
      {
         return myPeriod.count()<s.myPeriod.count();
      }
private:
	string myID;
	std::chrono::microseconds myPeriod;
	unsigned int myPriority;
};


static bool sortByPeriod (SchedulerTask *lhs, SchedulerTask *rhs) { return lhs->getPeriod() < rhs->getPeriod(); }
static bool sortByPriority (SchedulerTask *lhs, SchedulerTask *rhs) { return lhs->getPriority() < rhs->getPriority(); }

vector<SchedulerTask*> getSchedule(vector<SchedulerTask*> unorderedTasks);

class PrepareSchedule{
public:
	PrepareSchedule(vector<SchedulerTask*>);
	void calculateCycleTime(vector<SchedulerTask*>);
	vector<SchedulerTask*> RecalculateActiveTasks(std::chrono::system_clock::time_point);
private:

	std::chrono::microseconds cycleTime;
	vector<SchedulerTask*> enabledTasks;
};

#endif