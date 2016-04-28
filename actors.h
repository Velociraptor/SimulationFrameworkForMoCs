#ifndef ACTORS_H
#define ACTORS_H

#include <string>
#include <vector>

using namespace std;

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
	void SetValueBool(bool b){myContents.portValue.valBool = bool(b);};
	void SetValueInt(int i){myContents.portValue.valInt = int(i);};
	void SetValueFloat(float f){myContents.portValue.valFloat = float(f);};
	void SetValueDouble(double d){myContents.portValue.valDouble = double(d);};
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