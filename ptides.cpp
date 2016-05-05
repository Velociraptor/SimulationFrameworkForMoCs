#include <iostream>
#include <vector>
#include <string>
#include "ptides.h"
#include "scheduler.h"
#include "actors.h"

using namespace std;

PTask::PTask(string name, Actor* a, chrono::milliseconds t){
	myName = name;
	myActor = a;
	myDelay = t;
}

Platform::Platform(string name, vector<Task*> tasks, unsigned int clktime){
	myName = name;
	myTasks = tasks;
	myClockTime = clktime;
}

Network::Network(string name, Platform* p1, Platform* p2, chrono::milliseconds d){
	myName = name;
	srcPlatform = p1;
	destPlatform = p2;
	myDelay = d;
}

PtidesDirector::PtidesDirector(vector<Platform*> p, vector<Network*> n, unsigned int trig, chrono::milliseconds ts){
	allPlatforms = p;
	allNetworks = n;
	startTrigger = trig;
	TimeStamp = ts;
}

void PtidesDirector::Run(unsigned int trig){

}