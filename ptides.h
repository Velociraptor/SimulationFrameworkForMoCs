#ifndef PTIDES_H
#define PTIDES_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "scheduler.h"
#include "actors.h"

using namespace std;

/* 
PTIDES Model of Computation

Uma Balakrishnan and Hannah Sarver
EE 249B Embedded Systems Spring 2016

Very basic PTIDES class implementation based on the model
of computation presented in:
 https://chess.eecs.berkeley.edu/pubs/529/RTASPtides-1.pdf

Primary components include PTasks with Actors and logical time
delays which are included in Platforms that dictate linear
execution for the Director. When the PTasks in the current
Platform have been exhausted, the Director will check the
Networks provided for a transition away from that Platform
and then execute the PTasks in the destination Platform.
When all Platforms' PTasks have been completed, execution returns
to the external triggering environment, such as a periodic Director
(ie Giotto).
*/

class PTask{
public:
	PTask(string, Actor*, chrono::milliseconds);
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
	Platform(string, vector<PTask*>, unsigned int); //What is the type of platformClockTime????
	string getName(){return myName;};
	vector<PTask*> getTasks(){return myTasks;};
	unsigned int getClockTime(){return myClockTime;};
private:
	string myName;
	vector<PTask*> myTasks;
	unsigned int myClockTime;
};

class Network{
public:
	Network(string, Platform*, Platform*, chrono::milliseconds);
	string getName(){return myName;};
	Platform* getSrcPlatform(){return srcPlatform;};
	Platform* getDestPlatform(){return destPlatform;};
	chrono::milliseconds getDelay(){return myDelay;};
private:
	string myName;
	Platform* srcPlatform;
	Platform* destPlatform;
	chrono::milliseconds myDelay;
};

class PtidesDirector{
public:
	PtidesDirector(Platform*, vector<Network*>, chrono::milliseconds);
	void FindNextPlatform();
	void Run();
private:
	bool running;
	Platform* firstPlatform;
	Platform* currentPlatform;
	vector<Network*> allNetworks;
	chrono::milliseconds TimeStamp;
};

#endif