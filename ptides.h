#ifndef PTIDES_H
#define PTIDES_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "scheduler.h"
#include "actors.h"

using namespace std;

class Task{
public:
	Task(string, Actor*, chrono::milliseconds);
	string getName(){return myName;};
	Actor* getActor(){return myActor;};
	chrono::milliseconds getDelay(){return myDelay;};

private:
	string myName;
	Actor *myActor;
	chrono::milliseconds myDelay;
};

class Platform{
public:
	Platform(string, vector<Task*>, unsigned int); //What is the type of platformClockTime????
	string getName(){return myName;};
	vector<Task*> getTasks(){return myTasks;};
	unsigned int getClockTime(){return myClockTime;};
private:
	string myName;
	vector<Task*> myTasks;
	unsigned int myClockTime;
};

class Network{
public:
	Network(string, Platform*, Platform*, chrono::milliseconds);
	string getName(){return myName;};
	Platform* getSrcPlatform(){return srcPlatform;};
	Platform* getDestplatform(){return destPlatform;};
	chrono::milliseconds getDelay(){return myDelay;};
private:
	string myName;
	Platform* srcPlatform;
	Platform* destPlatform;
	chrono::milliseconds myDelay;
};

class PtidesDirector{
public:
	PtidesDirector(vector<Platform*>, vector<Network*>, unsigned int, chrono::milliseconds);
	void Run(unsigned int);
private:
	vector<Platform*> allPlatforms;
	vector<Network*> allNetworks;
	unsigned int startTrigger;
	chrono::milliseconds TimeStamp; //What is the type for TimeStamp????
};

#endif