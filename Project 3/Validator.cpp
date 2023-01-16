#include "Validator.h"
#include <stdexcept>

Validator::Validator(int min, int max) {

	if (min > max) {
		throw std::invalid_argument("Min is not smaller than max");
	}
	this->minValue = min;
	this->maxValue = max;
}

//Checks if a value is within a min and max.
//@param value - int value to test.
//@returns - a bool value. False if the value is not valid, True if the value is valid.
bool Validator::isValid(int value) {
	bool valid = false;

	if (value >= minValue && value <= maxValue) {
		valid = true;
	}

	return valid;
}

//Returns the minimum value.
//@returns - The minimum value.
int Validator::getMinValue() {
	return minValue;
}

//Returns the maximum value.
//@returns - The maximum value.
int Validator::getMaxValue() {
	return maxValue;
}