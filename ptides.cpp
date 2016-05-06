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

Platform::Platform(string name, vector<PTask*> tasks, unsigned int clktime){
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

PtidesDirector::PtidesDirector(Platform* p, vector<Network*> n, chrono::milliseconds ts){
	firstPlatform = p;
	currentPlatform = p;
	allNetworks = n;
	TimeStamp = ts;
}

void PtidesDirector::Run(){
	running = true;
	while(running){
		for (int i = 0; i < currentPlatform->getTasks().size(); ++i)
			{
				currentPlatform->getTasks()[i]->getActor()->Compute();
				TimeStamp+=currentPlatform->getTasks()[i]->getDelay();
			}
		//findNextPlatform();
		}
}