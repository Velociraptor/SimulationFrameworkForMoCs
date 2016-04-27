#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include "scheduler.h"

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

// bool sortByPeriod (const SchedulerTask &lhs, const SchedulerTask &rhs) { return &lhs < &rhs; }
bool sortByPeriod (SchedulerTask lhs, SchedulerTask rhs) { return lhs < rhs; }

vector<SchedulerTask> getSchedule(vector<SchedulerTask> unorderedTasks){
	sort(unorderedTasks.begin(), unorderedTasks.end(), sortByPeriod);
	return unorderedTasks;
}

int main(){
	return 0;
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