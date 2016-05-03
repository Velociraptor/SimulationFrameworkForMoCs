#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include "scheduler.h"

using namespace std;

PrepareSchedule::PrepareSchedule(vector<SchedulerTask*> t, std::chrono::system_clock::time_point start){
	enabledTasks = t;
	startTime = start;
}

void PrepareSchedule::calculateCycleTime(vector<SchedulerTask*> sorted){
	cycleTime = sorted[0]->getPeriod();
	enabledTasks = sorted;
}

vector<SchedulerTask*> PrepareSchedule::RecalculateActiveTasks(std::chrono::system_clock::time_point time_of_last_cycle){
	vector<SchedulerTask*> activeTasks;
	std::chrono::milliseconds wait_for_cycle = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_of_last_cycle);
	while(wait_for_cycle < cycleTime){
		wait_for_cycle = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_of_last_cycle);
		;
	}
	std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < enabledTasks.size(); ++i)
	{
		// if((enabledTasks[i]->getPeriod().count())*(enabledTasks[i]->getCallNum()) < std::chrono::duration_cast<std::chrono::milliseconds>(current_time - startTime).count())
		if((enabledTasks[i]->getPeriod().count())*(enabledTasks[i]->getCallNum()) < std::chrono::duration_cast<std::chrono::milliseconds>(current_time - startTime + cycleTime).count()) // ??? Wasn't this what we wanted ?? 
		{
			activeTasks.push_back(enabledTasks[i]);
		}
	}
	return activeTasks;
}

void PrepareSchedule::setStartTime(std::chrono::system_clock::time_point newStart){
	startTime = newStart;
}

vector<SchedulerTask*> getSchedule(vector<SchedulerTask*> unorderedTasks){
	sort(unorderedTasks.begin(), unorderedTasks.end(), sortByPeriod);
	unsigned int i = 0;
	while (i < unorderedTasks.size())
	{
		int j=1;
		while(i+j<unorderedTasks.size() && unorderedTasks[i]->getPeriod() == unorderedTasks[i+j]->getPeriod()){
			j++;
		}
		if(j>1){
			sort(unorderedTasks.begin()+i, unorderedTasks.begin()+i+j-1, sortByPriority);
		}
		i+=j;
	}
	return unorderedTasks;
}

// Testing


// int main ()
// {
//   	vector<SchedulerTask> myList;
//   	string mystrings[] = {"one", "two", "three","four","five","six","seven","eight","nine","ten"};
//   	for (int i = 9; i >= 0; i--)
//   	{
//   		std::chrono::milliseconds m(i+1);
 
//   		SchedulerTask nextTask(mystrings[i], m);
//   		myList.push_back(nextTask);
//   	}
//   	cout<<"Before"<<endl;
//   	for (int i = 0; i < myList.size(); ++i)
//   	{
//   		cout<<myList[i].getID()<<endl;
//   		std::cout<<myList[i].getPeriod().count()<<endl;
//   	}
//   	vector<SchedulerTask> answer = getSchedule(myList);
//   	cout<<"\nAfter"<<endl;
//   	for (int i = 0; i < answer.size(); ++i)
//   	{
//   		cout<<answer[i].getID()<<endl;
//   		std::cout<<answer[i].getPeriod().count()<<endl;
//   	}
//   return 0;
// }