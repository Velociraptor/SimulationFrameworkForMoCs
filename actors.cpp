#include "actors.h"

// Actor constructor
Actor::Actor (string name) {
	myName = name;
	input1 = 0;
	input2 = 0;
	output = false;
}

// Return name with which Actor was initialized
string Actor::Name () {
	return myName;
}

// Return actor type: IntComparator
string IntComparatorG::ActorType () {
	return "IntComparatorG";
}

// Perform computation to set output value
// based on current inputs
void IntComparatorG::Compute () {
	if (input1 > input2) {
		output = true;
	} else {
		output = false;
	}
}

void IntComparatorG::SetIntInput1 (int i) {
	input1 = i;
}

void IntComparatorG::SetIntInput2 (int i) {
	input2 = i;
}

bool IntComparatorG::GetOutput () {
	return output;
}