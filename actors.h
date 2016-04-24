#ifndef ACTORS_H
#define ACTORS_H

#include <string>
#include <vector>

using namespace std;

// generic Actor base class
class Actor {
public:
	Actor(string);
	string Name();
	virtual string ActorType();
	virtual void Compute();
private:
	string myName;
};

// strict greater than comparator for integer inputs
class IntComparatorG : Actor {
public:
	IntComparatorG(string s) : Actor(s) {};
	string ActorType();
	void Compute();
	void SetIntInput1(int);
	void SetIntInput2(int);
	bool GetOutput();
private:
	int input1;
	int input2;
	bool output;
};

// generate random int within given range
class RandomIntInRange : Actor {
public:
	RandomIntInRange(string, int);
	string ActorType();
	void Compute();
	void SetRange(int);
	int GetOutput();
private:
	int range;
	int output;
};

// accumulator with reset
class AccumulatorWithReset : Actor {
public:
	AccumulatorWithReset(string) : Actor(s) {};
};

#endif