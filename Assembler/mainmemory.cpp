#include "mainmemory.h"

MainMemory::MainMemory()
{
	try
	{
		progMEM = new unsigned char[0x100000];
		dataMEM = new unsigned char[0x100000];
		stackMEM = new unsigned char[0x100000];
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "BAD ALLOCATION\n";
		if (!progMEM) delete[]progMEM;
		if (!dataMEM) delete[]dataMEM;
	}
}
void MainMemory::reinitialize()
{
	if (!progMEM) delete[]progMEM;
	if (!dataMEM) delete[]dataMEM;
	if (!stackMEM) delete[]stackMEM;
	try
	{
		progMEM = new unsigned char[0x100000];
		dataMEM = new unsigned char[0x100000];
		stackMEM = new unsigned char[0x100000];
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "BAD ALLOCATION\n";
		if (!progMEM) delete[]progMEM;
		if (!dataMEM) delete[]dataMEM;
	}
}
MainMemory::~MainMemory()
{
	if (!progMEM) delete[]progMEM;
	if (!dataMEM) delete[]dataMEM;
	if (!stackMEM) delete[]stackMEM;
}

int MainMemory::readFromMemory(unsigned char* memory_pointer, int read_byte)
{
	int result = 0x00000000;
	for (int i = 0; i < read_byte; i++)
	{
		result |= (*(memory_pointer + i) << 8 * i);
		//std::cout << "result: " << std::hex << result << "\n";
	}
	return result;
}
void MainMemory::writeIntoMemory(unsigned char* memory_pointer, int write_byte, int object)
{
	//std::cout << "write: " << std::hex << object << "\n";
	char quarter[4] = { 0 };
	for (int i = 0; i < write_byte; i++)
	{
		quarter[i] = (object >> 8 * i) & 0xff;
		*(memory_pointer + i) = quarter[i];
	}
}

void MainMemory::operator()(uint32_t memory_address, int write_value, SIZE word_size)
{

	unsigned int memory_location, offset;
	unsigned char* memory_pointer;
	memory_location = memory_address >> 20;
	offset = memory_address & 0xfffff;
	if (memory_location == PROGRAM)
		memory_pointer = progMEM;
	else if (memory_location == DATA)
		memory_pointer = dataMEM;
	else if (memory_location == STACK)
		memory_pointer = stackMEM;
	else
	{
		std::cerr << "ERROR: No memory \nMESSAGE FROM: <void MainMemory::operator()(uint32_t memory_address, int write_value, SIZE word_size)> \n FILE: mainmemory.cpp" << std::endl;
		exit(1);
	}

	switch (word_size)
	{
	case BYTE:
		writeIntoMemory(&memory_pointer[offset], 1, write_value);
		break;
	case HALF_WORD:
		writeIntoMemory(&memory_pointer[offset], 2, write_value);
		break;
	case WORD:
		writeIntoMemory(&memory_pointer[offset], 4, write_value);
		break;
	default:
		std::cerr << "invalid size\n";
		exit(1);
		break;
	}

}
int MainMemory::operator()(uint32_t memory_address, SIZE word_size)
{
	unsigned int memory_location, offset;
	unsigned char* memory_pointer;
	memory_location = memory_address >> 20;
	offset = memory_address & 0xfffff;
	if (memory_location == PROGRAM)
		memory_pointer = progMEM;
	else if (memory_location == DATA)
		memory_pointer = dataMEM;
	else if (memory_location == STACK)
		memory_pointer = stackMEM;
	else
	{
		std::cerr << "ERROR: No memory \nMESSAGE FROM: <int MainMemory::operator()(uint32_t memory_address, SIZE word_size)> \nFILE: mainmemory.cpp" << std::endl;
		exit(1);
	}

	switch (word_size)
	{
	case BYTE:
		return readFromMemory(&memory_pointer[offset], 1);
		break;
	case HALF_WORD:
		return readFromMemory(&memory_pointer[offset], 2);
		break;
	case WORD:
		return readFromMemory(&memory_pointer[offset], 4);
		break;
	default:
		std::cerr << "invalid size\n";
		exit(1);
		break;
	}
}
