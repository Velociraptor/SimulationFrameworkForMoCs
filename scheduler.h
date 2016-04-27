#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <vector>
#include <string>

using namespace std;
class SchedulerTask;
bool sortByPeriod (SchedulerTask lhs, SchedulerTask rhs);

vector<SchedulerTask> getSchedule(vector<SchedulerTask> unorderedTasks);

#endif