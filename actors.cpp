#include "actors.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
using namespace std;

// Port constructor
Port::Port (string name, PortContents portInit) {
	cout << "Creating Port " << name << endl;
	myName = name;
	myContents = portInit;
}

// Port value changes - called by actors

void Port::SetValueBool(bool b){
	cout << "    Port " << myName << " new bool value: " << b << endl;
	myContents.portValue.valBool = bool(b);
}

void Port::SetValueInt(int i){
	cout << "    Port " << myName << " new int value: " << i << endl;
	myContents.portValue.valInt = int(i);
	ofstream fileOut;
	fileOut.open("output.txt", std::ios_base::app);
	fileOut << myName << "," << i << endl;
	fileOut.close();
}

void Port::SetValueFloat(float f){
	cout << "    Port " << myName << " new float value: " << f << endl;
	myContents.portValue.valFloat = float(f);
}

void Port::SetValueDouble(double d){
	cout << "    Port " << myName << " new double value: " << d << endl;
	myContents.portValue.valDouble = double(d);
}

// Base Actor constructor
Actor::Actor (string name, vector<Port*> inputPorts, 
	vector<Port*> outputPorts) {
	cout << "Creating Actor " << name << endl;
	myName = name;
	myInputs = inputPorts;
	myOutputs = outputPorts;
}

// Derived Actor class definitions

/* 
ComparatorGreater - Sets boolean output true if first input is greater
	than second (expected same numeric type), else false
Constructor: call base class constructor and warn on unexpected port vector sizes
Expected input ports: 2 - numeric
Expected output ports: 1 - bool
*/
ComparatorGreater::ComparatorGreater(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 2) {
		cout << "Warning: ComparatorGreater Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: ComparatorGreater Actor received unexpected number of output ports." << endl;
	}
}

// Return actor type
string ComparatorGreater::ActorType () {
	return "ComparatorGreater";
}

/* Perform computation to set output port value
based on current input port values */
void ComparatorGreater::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	bool output = false;
	PortType pt = myInputs[0]->Type();
	switch (pt) {
		case INT:
			{
				int input1i = myInputs[0]->GetValueInt();
				int input2i = myInputs[1]->GetValueInt();
				if (input1i > input2i) {
					output = true;
				}
				break;
			}
		case FLOAT:
			{
				float input1f = myInputs[0]->GetValueFloat();
				float input2f = myInputs[1]->GetValueFloat();
				if (input1f > input2f) {
					output = true;
				}
				break;
			}
		case DOUBLE:
			{
				double input1d = myInputs[0]->GetValueDouble();
				double input2d = myInputs[1]->GetValueDouble();
				if (input1d > input2d) {
					output = true;
				}
				break;
			}
		default:
			output = false;
			break;
	}
	
	myOutputs[0]->SetValueBool(output);
}

/*
RandomIntInRange - generate a rand integer within range of zero to given max
Constructor: call base class constructor and warn on unexpected port vector sizes
Expected input ports: 1 - numeric (top of range, from zero)
Expected output ports: 1 - int
*/
RandomIntInRange::RandomIntInRange(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 1) {
		cout << "Warning: RandomIntInRange Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: RandomIntInRange Actor received unexpected number of output ports." << endl;
	}
}

// Return actor type
string RandomIntInRange::ActorType () {
	return "RandomIntInRange";
}

/* Perform computation to set output port value
based on current input port values */
void RandomIntInRange::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	int rangeBottom = 0;
	int rangeTop = myInputs[0]->GetValueInt();
	int output = rangeBottom + (rand() % (int)(rangeTop - rangeBottom + 1));
	myOutputs[0]->SetValueInt(output);
}

/*
AccumulatorWithReset - Add input to 
Constructor: call base class constructor and warn on unexpected port vector sizes
Expected input ports: 2 - numeric and bool (for reset flag)
Expected output ports: 1 - numeric (current accumulation)
*/
AccumulatorWithReset::AccumulatorWithReset(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 2) {
		cout << "Warning: AccumulatorWithReset Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: AccumulatorWithReset Actor received unexpected number of output ports." << endl;
	}
	internalSum = 0;
}

// Return actor type
string AccumulatorWithReset::ActorType () {
	return "AccumulatorWithReset";
}

/* Perform computation to set output port value
based on current input port values */
void AccumulatorWithReset::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	// check reset and act as appropriate
	bool reset = myInputs[1]->GetValueBool();
	if (reset) {
		Reset();
	} else {
		// no reset; add value and set input to zero
		int inputAdd = myInputs[0]->GetValueInt();
		internalSum += inputAdd;
		myOutputs[0]->SetValueInt(internalSum);
		cout << "   Accumulator Actor " << Name() << " clearing input" << endl;
		myInputs[0]->SetValueInt(0);
	}
}

// Set output to zero (and set reset port back to false) 
void AccumulatorWithReset::Reset () {
	cout << "   Accumulator Actor " << Name() << " resetting" << endl;
	internalSum = 0;
	myOutputs[0]->SetValueInt(internalSum);
	myInputs[1]->SetValueBool(false);
}

/*
Difference - Subtract second input from first, 
	currently both assumed integers
Constructor: call base class constructor and warn on unexpected port vector sizes
expected input ports: 2 - numeric
expected output ports: 1 - numeric
*/
Difference::Difference(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 2) {
		cout << "Warning: Difference Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: Difference Actor received unexpected number of output ports." << endl;
	}
}

// Return actor type
string Difference::ActorType () {
	return "Difference";
}

/* Perform computation to set output port value
based on current input port values */
void Difference::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	int diff = myInputs[0]->GetValueInt() - myInputs[1]->GetValueInt();
	myOutputs[0]->SetValueInt(diff);
}

/*
Trigger - compute function sets output to true, to trigger a subsequent actor
Constructor: call base class constructor and warn on unexpected port vector sizes
expected input ports: none
expected output ports: 1 - bool (true when triggered)
*/
Trigger::Trigger(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 0) {
		cout << "Warning: Trigger Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: Trigger Actor received unexpected number of output ports." << endl;
	}
}

// Return actor type
string Trigger::ActorType () {
	return "Trigger";
}

// Set output port value to true
void Trigger::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	myOutputs[0]->SetValueBool(true);
}

/*
ThresholdTrigger - set output to true if input exceeds specified threshold
Constructor: call base class constructor and warn on unexpected port vector sizes
expected input ports: 2 - numeric (threshold and current value)
expected output ports: 1 - bool (true when triggered)
*/
ThresholdTrigger::ThresholdTrigger(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 2) {
		cout << "Warning: ThresholdTrigger Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: ThresholdTrigger Actor received unexpected number of output ports." << endl;
	}
}

// Return actor type
string ThresholdTrigger::ActorType () {
	return "ThresholdTrigger";
}

// Set output port value to true if threshold crossed
void ThresholdTrigger::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	if (myInputs[1]->GetValueInt() > myInputs[0]->GetValueInt()) {
		myOutputs[0]->SetValueBool(true);
	} else {
		myOutputs[0]->SetValueBool(false);
	}
}

/*
Register - store previous input internally and set as output on next Compute
Parent constructor plus initially set internalStorage to zero
expected input ports: 1 - numeric
expected output ports: 1 - numeric (gives previous stored value)
*/
Register::Register (string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) :  Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 1) {
		cout << "Warning: Register Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: Register Actor received unexpected number of output ports." << endl;
	}
	internalStorage = 0;
}

// Return actor type
string Register::ActorType () {
	return "Register";
}

// Shift internal storage to output, and input to internal storage
void Register::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	myOutputs[0]->SetValueInt(internalStorage);
	internalStorage = myInputs[0]->GetValueInt();
}

/*
RepeatingRamp - generate successive values in a range, reset on max
Constructor: call base class constructor and warn on unexpected port vector sizes
expected input ports: 2 - numeric (top of range, increment)
expected output ports: 1 - numeric
*/
RepeatingRamp::RepeatingRamp(string s, vector<Port*> inputPorts, 
		vector<Port*> outputPorts) : Actor(s, inputPorts, outputPorts) {
	if (inputPorts.size() != 2) {
		cout << "Warning: RepeatingRamp Actor received unexpected number of input ports." << endl;
	}
	if (outputPorts.size() != 1) {
		cout << "Warning: RepeatingRamp Actor received unexpected number of output ports." << endl;
	}
	nextOutput = myInputs[1]->GetValueInt();
}

// Return actor type
string RepeatingRamp::ActorType () {
	return "RepeatingRamp";
}

// Set output port to subsequent value, or zero if at max
void RepeatingRamp::Compute () {
	cout << "  Actor " << Name() << " computing" << endl;
	int increment = myInputs[1]->GetValueInt();
	int maximal = myInputs[0]->GetValueInt();
	// int newOutput = myOutputs[0]->GetValueInt() + increment;
	nextOutput = nextOutput + increment;
	if (nextOutput > maximal) {
		nextOutput = 0;
	}
	myOutputs[0]->SetValueInt(nextOutput);
}

// // Unit Testing
// int main() {
// 	PortValue pva;
// 	pva.valInt = 1;
// 	PortContents pca = {INT, pva};
// 	PortValue pvo;
// 	pvo.valBool = false;
// 	PortContents pco = {BOOL, pvo};
// 	Port *a = new Port(string("a"), pca);
// 	Port *b = new Port(string("b"), pca);
// 	Port *o = new Port(string("out"), pco);
// 	vector<Port*> vip;
// 	vector<Port*> vop;
// 	vip.push_back(a);
// 	vip.push_back(b);
// 	vop.push_back(o);
// 	b->SetValueInt(5);
// 	ComparatorGreater compy = ComparatorGreater(string("compy"), vip, vop);
// 	// cout << "compy inputs: " << compy.GetInputs()[0]->GetValueInt() << " " << compy.GetInputs()[1]->GetValueInt() << endl;
// 	compy.Compute();
// 	cout << "compy says " << compy.GetOutputs()[0]->GetValueBool() << " for 1>5" << endl;
// 	// cout << "compy inputs: " << compy.GetInputs()[0]->GetValueInt() << " " << compy.GetInputs()[1]->GetValueInt() << endl;
// 	a->SetValueInt(10);
// 	compy.Compute();
// 	cout << "compy says " << compy.GetOutputs()[0]->GetValueBool() << " for 10>5" << endl;
// 	// cout << "compy inputs: " << compy.GetInputs()[0]->GetValueInt() << " " << compy.GetInputs()[1]->GetValueInt() << endl;
// 	srand((unsigned)time(NULL));
// 	PortValue pvaa;
// 	pvaa.valInt = 100;
// 	PortContents pcaa = {INT, pvaa};
// 	PortValue pvbb;
// 	pvbb.valInt = 100;
// 	PortContents pcbb = {INT, pvbb};
// 	Port *aa = new Port(string("aa"), pcaa);
// 	Port *oo = new Port(string("out"), pcbb);
// 	vector<Port*> viip = vector<Port*>();
// 	vector<Port*> voop = vector<Port*>();
// 	viip.push_back(aa);
// 	voop.push_back(oo);
// 	// srand(time(NULL));
// 	RandomIntInRange randy = RandomIntInRange(string("randy"), viip, voop);
// 	for (int i=0;i<10;i++) {
// 		randy.Compute();
// 		cout << "randy says " << randy.GetOutputs()[0]->GetValueInt() << " out of 100" << endl;
// 	}

// 	Port *in = new Port(string("in"), INT, 2);
// 	Port *reset = new Port(string("reset"), BOOL, false);
// 	Port *out = new Port(string("outt"), INT, 0);
// 	vector<Port*> vinp = vector<Port*>();
// 	vector<Port*> voutp = vector<Port*>();
// 	vinp.push_back(in);
// 	vinp.push_back(reset);
// 	voutp.push_back(out);
// 	AccumulatorWithReset acc = AccumulatorWithReset(string("accy"), vinp, voutp);
// 	for (int i=1;i<12;i++) {
// 		acc.Compute();
// 		in->SetValueInt(i);
// 		if (i%5 == 0) {
// 			reset->SetValueBool(true);
// 		}
// 		cout << "accy says " << acc.GetOutputs()[0]->GetValueInt() << " now + " << i << " reset " << reset->GetValueBool() << endl;
// 	}

// 	Port *aaa = new Port(string("aaa"), INT, 100);
// 	Port *bbb = new Port(string("bbb"), INT, 5);
// 	Port *ooo = new Port(string("ooo"), INT, 0);
// 	vector<Port*> viiip = vector<Port*>();
// 	vector<Port*> vooop = vector<Port*>();
// 	viiip.push_back(aaa);
// 	viiip.push_back(bbb);
// 	vooop.push_back(ooo);
// 	Difference diffy = Difference(string("diffy"), viiip, vooop);
// 	diffy.Compute();
// 	cout << "diffy says " << diffy.GetOutputs()[0]->GetValueInt() << " for 100-5" << endl;
// 	aaa->SetValueInt(1000);
// 	diffy.Compute();
// 	cout << "diffy says " << diffy.GetOutputs()[0]->GetValueInt() << " for 1000-5" << endl;
	
// 	return 0;
// }