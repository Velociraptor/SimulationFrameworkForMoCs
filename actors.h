#ifndef ACTORS_H
#define ACTORS_H

#include <string>
#include <vector>

using namespace std;

/* 
Actor Model

Uma Balakrishnan and Hannah Sarver
EE 249B Embedded Systems Spring 2016

Implementations for Port and Actor classes to be
used in any simulation with any model of computation
built upon this platform. 

Ports can currently be
generated with any of four types, and their values
retrieved by specified type.

Derived Actors must specify a Run method that may
use their input port values to determine new values
to apply to their output ports. Actors will be used
within simulations for any model of computation.
*/

typedef enum PortType{ BOOL, INT, FLOAT, DOUBLE } PortType;

typedef union PortValue{
	bool valBool;
	int valInt;
	float valFloat;
	double valDouble;
} PortValue;

typedef struct PortContents{
	PortType portType;
	PortValue portValue;
} PortContents;

// shared notion of Ports
class Port{
public:
	Port(string, PortContents);
	string Name(){return myName;};
	PortType Type(){return myContents.portType;};
	PortContents GetContents(){return myContents;};
	bool GetValueBool(){return bool(myContents.portValue.valBool);};
	int GetValueInt(){return int(myContents.portValue.valInt);};
	float GetValueFloat(){return float(myContents.portValue.valFloat);};
	double GetValueDouble(){return double(myContents.portValue.valDouble);};
	void SetValueBool(bool b);
	void SetValueInt(int i);
	void SetValueFloat(float f);
	void SetValueDouble(double d);
private:
	string myName;
	PortContents myContents;
};

// generic Actor base class
class Actor {
public:
	Actor(string, vector<Port*>, vector<Port*>);
	string Name(){return myName;};
	vector<Port*> GetInputs(){return myInputs;};
	vector<Port*> GetOutputs(){return myOutputs;};
	virtual string ActorType() = 0;
	virtual void Compute() {};
protected:
	vector<Port*> myInputs;
	vector<Port*> myOutputs;
private:
	string myName;
};

// strict greater than comparator
class ComparatorGreater : public Actor {
public:
	ComparatorGreater(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
};

// generate random int within given range
class RandomIntInRange : public Actor {
public:
	RandomIntInRange(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
};

// accumulator with reset
class AccumulatorWithReset : public Actor {
public:
	AccumulatorWithReset(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
private:
	int internalSum;
	void Reset();
};

// difference
class Difference : public Actor {
public:
	Difference(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
};

// trigger
class Trigger : public Actor {
public:
	Trigger(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
};

// threshold trigger
class ThresholdTrigger : public Actor {
public:
	ThresholdTrigger(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
};

// register
class Register : public Actor {
public:
	Register(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
private:
	int internalStorage;
};

// repeating ramp
class RepeatingRamp : public Actor {
public:
	RepeatingRamp(string, vector<Port*>, vector<Port*>);
	string ActorType();
	void Compute();
private:
	int nextOutput;
};

#endif