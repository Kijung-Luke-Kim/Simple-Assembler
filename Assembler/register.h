#pragma once
#include "useful.h"

class Reg
{
	uint32_t register_array[32] = { 0 };
	uint32_t register_array_before[32] = { 0 };
public:
	Reg() { register_array[29] = 0x80000000; }
	void reinitialize();
	~Reg() {}

	uint32_t& operator[](int index) { return register_array[index]; }
	void copy() { for (int i = 0; i < 32; i++) register_array_before[i] = register_array[i]; }
	void compare();
	void show();
};

class ProgramCounter
{
	uint32_t counter;
public:
	ProgramCounter() { counter = 0x00000000; }
	void reinitialize() { counter = 0x00400000; }

	auto inc() { counter += 4; }
	auto dec() { counter = -4; }

	auto get() { return counter; }

	void jump(int counter);
	void finish();
};

