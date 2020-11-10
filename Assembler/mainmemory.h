#pragma once
#include "useful.h"
enum MEMORY_TYPE { PROGRAM = 0x004, DATA = 0x100, STACK = 0x7ff };
enum SIZE { BYTE, HALF_WORD, WORD };

class MainMemory {
	unsigned char *progMEM = nullptr, *dataMEM = nullptr, *stackMEM = nullptr;

public:
	//allocates main memory space
	MainMemory();
	void reinitialize();
	~MainMemory();

	//copy constructor blocked
	MainMemory(const MainMemory& temp_obj) = delete;
	MainMemory& operator=(const MainMemory& temp_obj) = delete;

	//Put 3 args to write, 2 args to read
	void writeIntoMemory(unsigned char* memory_pointer, int write_byte, int object);
	int readFromMemory(unsigned char* memory_pointer, int return_byte);
	void operator()(uint32_t memory_address, int write_value, SIZE word_size);//write
	int operator()(uint32_t memory_address, SIZE word_size);//read
};