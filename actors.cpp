#include "actors.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

// Port constructors
Port::Port (string name, PortContents portInit) {
	cout << "Creating Port " << name << endl;
	myName = name;
	myContents = portInit;
}

void Port::SetValueBool(bool b){
	cout << "Port " << myName << " new bool value: " << b << endl;
	myContents.portValue.valBool = bool(b);
}

void Port::SetValueInt(int i){
	cout << "Port " << myName << " new int value: " << i << endl;
	myContents.portValue.valInt = int(i);
}

void Port::SetValueFloat(float f){
	cout << "Port " << myName << " new float value: " << f << endl;
	myContents.portValue.valFloat = float(f);
}

void Port::SetValueDouble(double d){
	cout << "Port " << myName << " new double value: " << d << endl;
	myContents.portValue.valDouble = double(d);
}

// Actor constructor
Actor::Actor (string name, vector<Port*> inputPorts, 
	vector<Port*> outputPorts) {
	cout << "Creating Actor " << name << endl;
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
	cout << "Actor " << Name() << " computing" << endl;
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

// Return actor type
string RandomIntInRange::ActorType () {
	return "RandomIntInRange";
}

// Perform computation to set output port value
// based on current input port values
void RandomIntInRange::Compute () {
	cout << "Actor " << Name() << " computing" << endl;
	int rangeBottom = 0;
	int rangeTop = myInputs[0]->GetValueInt();
	int output = rangeBottom + (rand() % (int)(rangeTop - rangeBottom + 1));
	myOutputs[0]->SetValueInt(output);
}

// Return actor type
string AccumulatorWithReset::ActorType () {
	return "AccumulatorWithReset";
}

// Perform computation to set output port value
// based on current input port values
void AccumulatorWithReset::Compute () {
	cout << "Actor " << Name() << " computing" << endl;
	// check reset and set output to zero if appropriate
	// (and set reset port back to false) 
	bool reset = myInputs[1]->GetValueBool();
	if (reset) {
		myOutputs[0]->SetValueInt(0);
		myInputs[1]->SetValueBool(false);
	} else {
		// no reset, add value and set input to zero
		int inputAdd = myInputs[0]->GetValueInt();
		int prevOut = myOutputs[0]->GetValueInt();
		myOutputs[0]->SetValueInt(prevOut + inputAdd);
		myInputs[0]->SetValueInt(0);
	}
}

// // Set reset input port high to trigger reset on next Compute()
// void AccumulatorWithReset::Reset () {
// 	myInputs[1]->SetValueBool(true);
// }

// Return actor type
string Difference::ActorType () {
	return "Difference";
}

// Perform computation to set output port value
// based on current input port values
void Difference::Compute () {
	cout << "Actor " << Name() << " computing" << endl;
	int diff = myInputs[0]->GetValueInt() - myInputs[1]->GetValueInt();
	myOutputs[0]->SetValueInt(diff);
}

// Return actor type
string Trigger::ActorType () {
	return "Trigger";
}

// Set output port value to true
void Trigger::Compute () {
	cout << "Actor " << Name() << " computing" << endl;
	myOutputs[0]->SetValueBool(true);
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
// 	Port *aa = new Port(string("aa"), INT, 100);
// 	Port *oo = new Port(string("out"), INT, 0);
// 	vector<Port*> viip = vector<Port*>();
// 	vector<Port*> voop = vector<Port*>();
// 	viip.push_back(aa);
// 	voop.push_back(oo);
// 	srand(time(NULL));
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