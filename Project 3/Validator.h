#pragma once
class Validator
{
public:
	Validator(int min, int max);

	bool isValid(int value);
	int getMinValue();
	int getMaxValue();

private:
	int minValue;
	int maxValue;
};