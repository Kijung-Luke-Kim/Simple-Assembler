#include "ALU.h"

int addSubtract(int X, int Y, int C)
{
	int ret;
	if (C < 0 || C>1) {
		std::cout << "error in add/subtract operation\n";
		exit(1);
	}
	if (C == 0) {	// add
		ret = X + Y;
	}
	else {	// subtract
		ret = X - Y;
	}
	return ret;
}

int logicOperation(int X, int Y, int C)
{
	if (C < 0 || C>3) {
		std::cout << "error in logic operation\n";
		exit(1);
	}
	if (C == 0) {	// AND
		return X & Y;
	}
	else if (C == 1) {	// OR
		return X | Y;
	}
	else if (C == 2) {	// XOR
		return (X ^ Y);
	}
	else {	//  NOR 
		return ~(X | Y);
	}
}

int shiftOperation(int V, int Y, int C)
{
	int ret, mask;

	if (C < 0 || C>3) {
		std::cout << "error in shift operation\n";
		exit(1);
	}
	if (C == 0) { // No Shift
		ret = V;
	}
	else if (C == 1) { // Logical Left Shift
		ret = V << Y;
	}
	else if (C == 2) { // Logical Right Shift
		mask = ~(UINT32_MAX << Y) << (32 - Y);
		ret = ~mask & ((V >> Y) | mask);
	}
	else { // Arith Right Shift
		ret = V >> Y;
	}
	return ret;
}

int checkSetLess(int X, int Y)
{
	int ret;
	if (X < Y) {
		ret = 1;
	}
	else {
		ret = 0;
	}
	return ret;
}

int ALU(int X, int Y, ALU_commands C)
{
	int c32, c10;
	int ret;

	c32 = (C >> 2) & 3;
	c10 = C & 3;

	if (c32 == 0) { // shift
		ret = shiftOperation(X, Y, c10);
	}
	else if (c32 == 1) { // set less
		ret = checkSetLess(X, Y);
	}
	else if (c32 == 2) {  // addsubtract
		c10 = c10 & 1;
		ret = addSubtract(X, Y, c10);
	}
	else {  // logic
		ret = logicOperation(X, Y, c10);
	}
	return ret;
}
