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
class IntComparatorG : public Actor {
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

#endif