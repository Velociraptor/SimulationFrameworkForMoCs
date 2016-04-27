#ifndef ACTORS_H
#define ACTORS_H

#include <string>
#include <vector>

using namespace std;

typedef enum PortType{ BOOL, INT, FLOAT, DOUBLE } PortType;

typedef union value{
	bool myValueBool;
	int myValueInt;
	float myValueFloat;
	double myValueDouble;
} PortValue;

// shared notion of Ports
class Port{
public:
	Port(string, PortType, PortValue);
	Port(string, PortType, bool);
	Port(string, PortType, int);
	Port(string, PortType, float);
	Port(string, PortType, double);
	string Name(){return myName;};
	PortType Type(){return myType;};
	PortValue GetValue(){return myValue;};
	bool GetValueBool(){return myValue.myValueBool;};
	int GetValueInt(){return myValue.myValueInt;};
	float GetValueFloat(){return myValue.myValueFloat;};
	double GetValueDouble(){return myValue.myValueDouble;};
	void SetValueBool(bool b){myValue.myValueBool = b;};
	void SetValueInt(int i){myValue.myValueInt = i;};
	void SetValueFloat(float f){myValue.myValueFloat = f;};
	void SetValueDouble(double d){myValue.myValueDouble = d;};
private:
	string myName;
	PortType myType;
	PortValue myValue;
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
// expected input ports: 2 - numeric
// expected output ports: 1 - bool
class ComparatorGreater : public Actor {
public:
	ComparatorGreater(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {};
	string ActorType();
	void Compute();
};

// generate random int within given range
// expected input ports: 1 - numeric (top of range, from zero)
// expected output ports: 1 - int
class RandomIntInRange : public Actor {
public:
	RandomIntInRange(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {};
	string ActorType();
	void Compute();
};

// accumulator with reset
// expected input ports: 2 - numeric and bool (for reset flag)
// expected output ports: 1 - numeric (current accumulation)
class AccumulatorWithReset : public Actor {
public:
	AccumulatorWithReset(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {};
	string ActorType();
	void Compute();
};

// difference
// expected input ports: 2 - numeric
// expected output ports: 1 - numeric
class Difference : public Actor {
public:
	Difference(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {};
	string ActorType();
	void Compute();
};

#endif