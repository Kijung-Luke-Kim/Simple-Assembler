#include "register.h"

void Reg::compare()
{
	for (int i = 0; i < 32; i++)
	{
		if (register_array_before[i] != register_array[i])
			std::cout <<"\t"<< std::hex << regnum(i) << ": 0x" << std::setfill('0') << std::setw(8) << register_array[i] << "\n";
	}
}

void Reg::show()
{
	std::cout << "Register Status\n";
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 4;i++) 
			std::cout << std::hex << regnum(j*4+i) << ": 0x" << std::setfill('0') << std::setw(8) << register_array[j * 4 + i] << "\t";
		std::cout << "\n";
	}
}

void Reg::reinitialize()
{
	for (int i = 0; i < 32; i++)
		register_array[i] = 0;
	register_array[29] = 0x80000000;
}

void ProgramCounter::jump(int address)
{
	try
	{
		if (address % 4 != 0)
			throw address;
		counter = address;
	}
	catch (int e)
	{
		std::cerr << "Invalid memory address: " << e << "\n";
		exit(1);
	}
}

void ProgramCounter::finish()
{
	std::cout << "*** Program Finished ***\n";
	this->jump(0x000000);
}
