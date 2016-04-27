#include "actors.h"
#include <iostream>

// Port constructors
Port::Port (string name, PortType type, PortValue initialValue) {
	myName = name;
	myType = type;
	myValue = initialValue;
}


Port::Port (string name, PortType type, bool initialValBool) {
	myName = name;
	myType = type;
	PortValue initialValue;
	initialValue.myValueBool = initialValBool;
	myValue = initialValue;
}

Port::Port (string name, PortType type, int initialValInt) {
	myName = name;
	myType = type;
	PortValue initialValue;
	initialValue.myValueInt = initialValInt;
	myValue = initialValue;
}


Port::Port (string name, PortType type, float initialValFloat) {
	myName = name;
	myType = type;
	PortValue initialValue;
	initialValue.myValueFloat = initialValFloat;
	myValue = initialValue;
}


Port::Port (string name, PortType type, double initialValDbl) {
	myName = name;
	myType = type;
	PortValue initialValue;
	initialValue.myValueDouble = initialValDbl;
	myValue = initialValue;
}

// Actor constructor
Actor::Actor (string name, vector<Port> inputPorts, 
	vector<Port> outputPorts) {
	myName = name;
	myInputs = inputPorts;
	myOutputs = outputPorts;
}

// Derived Actor class definitions

// Return actor type
string ComparatorGreater::ActorType () {
	return "ComparatorGreater";
}

// Perform computation to set output port value
// based on current input port values
void ComparatorGreater::Compute () {
	PortType pt = myInputs[0].Type();
	bool output;
	// PortValue input1pv = myInputs[0].GetValue();
	// PortValue input2pv = myInputs[1].GetValue();
	switch (pt) {
		case INT:
			{
				int input1 = myInputs[0].GetValueInt();
				int input2 = myInputs[1].GetValueInt();
				cout << "int 1 " << input1 << " int 2 " << input2 << endl;
				if (input1 > input2) {
					output = true;
				}
				break;
			}
		case FLOAT:
			{
				float input1 = myInputs[0].GetValueFloat();
				float input2 = myInputs[1].GetValueFloat();
				if (input1 > input2) {
					output = true;
				}
				break;
			}
		case DOUBLE:
			{
				double input1 = myInputs[0].GetValueDouble();
				double input2 = myInputs[1].GetValueDouble();
				if (input1 > input2) {
					output = true;
				}
				break;
			}
		default:
			output = false;
			break;
	}
	
	myOutputs[0].SetValueBool(output);
}

int main() {
	Port a = Port(string("a"), INT, 1);
	Port b = Port(string("b"), INT, 5);
	Port o = Port(string("out"), BOOL, false);
	// Port *a = new Port(string("a"), INT, 1);
	// Port *b = new Port(string("b"), INT, 5);
	// Port *o = new Port(string("out"), BOOL, false);
	vector<Port> vip = vector<Port>();
	vector<Port> vop = vector<Port>();
	vip.push_back(a);
	vip.push_back(b);
	vop.push_back(o);
	ComparatorGreater compy = ComparatorGreater(string("compy"), vip, vop);
	compy.Compute();
	cout << "compy says " << compy.GetOutputs()[0].GetValueBool() << " for 1>5" << endl;
	a.SetValueInt(10);
	compy.Compute();
	cout << "compy says " << compy.GetOutputs()[0].GetValueBool() << " for 10>5" << endl;


	return 0;
}