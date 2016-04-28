#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include <string>
#include <chrono>

using namespace std;

class SchedulerTask{
public:
	SchedulerTask(string id, std::chrono::milliseconds period){
		myID = id;
		myPeriod = period;
	}
	const string getID(){return myID;}
	const std::chrono::milliseconds getPeriod(){return myPeriod;};

	bool operator <(const SchedulerTask& s)
      {
         return myPeriod.count()<s.myPeriod.count();
      }
private:
	string myID;
	std::chrono::milliseconds myPeriod;
};

static bool sortByPeriod (SchedulerTask *lhs, SchedulerTask *rhs) { return lhs->getPeriod() < rhs->getPeriod(); }

vector<SchedulerTask*> getSchedule(vector<SchedulerTask*> unorderedTasks);

#endif