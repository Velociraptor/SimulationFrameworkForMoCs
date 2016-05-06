#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include <string>
#include <chrono>

using namespace std;

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