#include "instructions.h"
constexpr auto M_SIZE = 10000;
enum COMMANDS { LOAD = 'l', JUMP = 'j', GO_PROGRAM = 'g', STEP_OR_SET = 's', VIEW_MEMORY = 'm', VIEW_REGISTER = 'r', EXIT = 'x' };

int main()
{
	FILE* pFile = NULL;
	errno_t err;
	int count;
	unsigned char buffer[80] = { 0 };
	unsigned char data;
	unsigned int IR, DT;
	unsigned int opCode;
	int i, j;

	Reg REG;
	ProgramCounter PC;
	MainMemory MEM;

	std::string filename;
	std::string cmd;

	int Num_of_instruction;
	int Num_of_data;

	std::cout << ("Simple MIPS Simulator [Version 1.0]\n(2019) 컴퓨터구조 4조. Edited by 김기정, 박종현, 윤현우, 전효빈.\n\n");

	int cnt;
	uint32_t program_memory_load_counter, data_memory_load_counter;
	unsigned int start, end;
	unsigned int location, value, regno;
	unsigned int target;
	while (true)
	{
		std::cout << ">>> ";
		std::getline(std::cin, cmd);

		switch (cmd[0])
		{
		case LOAD:
			REG.reinitialize();
			PC.reinitialize();
			MEM.reinitialize();

			filename = cmd.substr(2);
			std::cout << filename << std::endl;
			err = fopen_s(&pFile, filename.c_str(), "rb");
			if (err || pFile == 0) {
				std::cerr << "Cannot open file\n";
			}
			cnt = 0;
			while (1)
			{
				count = fread(&data, 1, 1, pFile);
				if (count != 1)
					break;
				buffer[cnt++] = data;
			}

			Num_of_instruction = buffer[0] * 16 * 16 * 16 * 16 * 16 * 16 + buffer[1] * 16 * 16 * 16 * 16 + buffer[2] * 16 * 16 + buffer[3];
			Num_of_data = buffer[4] * 16 * 16 * 16 * 16 * 16 * 16 + buffer[5] * 16 * 16 * 16 * 16 + buffer[6] * 16 * 16 + buffer[7];

			std::cout << "Number of Instructions :" << Num_of_instruction << "\n";
			std::cout << "Number of Data : " << Num_of_data << "\n\n";

			//load instructions on program memory
			program_memory_load_counter = 0x00400000;
			for (i = 2; i < Num_of_instruction + 2; ++i)
			{
				IR = 0;

				for (j = i * 4; j < (i * 4) + 4; ++j)
					IR += buffer[j] * (int)pow((int)16, (int)(3 - (j % 4)) * 2);

				MEM(program_memory_load_counter, IR, WORD);
				program_memory_load_counter += 4;
			}

			//load data on data memory
			data_memory_load_counter = 0x10000000;
			for (i = 2 + Num_of_instruction; i < Num_of_data + 2 + Num_of_instruction; ++i)
			{
				DT = 0;

				for (j = i * 4; j < (i * 4) + 4; ++j)
					DT += buffer[j] * (int)pow((int)16, (int)(3 - (j % 4)) * 2);

				MEM(data_memory_load_counter, DT, WORD);
				data_memory_load_counter += 4;
			}
			break;

		case JUMP:
			target = std::stoi(cmd.substr(2), nullptr, 16) ? std::stoi(cmd.substr(2), nullptr, 16) : std::stoi(cmd.substr(4), nullptr, 16);
			if (target % 4 != 0)
				std::cerr << "ERROR: Invalid jump address. Program continues to the end.\nMESSAGE FROM: switch case:JUMP\nFILE: Assembler.cpp" << std::endl;
			while (true)
			{
				IR = MEM(PC.get(), WORD);
				opCode = IR >> 26;

				if (opCode == 0)	// R format
				{
					r_format(IR, opCode, REG, PC, MEM);
				}
				else if (opCode == 2 || opCode == 3) //J format
				{
					j_format(IR, opCode, REG, PC, MEM);
				}
				else //I format
				{
					i_format(IR, opCode, REG, PC, MEM);
				}

				//breakpoint
				if (PC.get() == target)
					break;
				if (PC.get() < 0x400000)
					break;
			}
			break;
		case GO_PROGRAM:
			if (!PC.get())
			{
				std::cerr << "File is not loaded.\n";
				break;
			}
			while (true)
			{
				IR = MEM(PC.get(), WORD);

				opCode = IR >> 26;

				if (opCode == 0)	// R format
				{
					r_format(IR, opCode, REG, PC, MEM);
				}
				else if (opCode == 2 || opCode == 3) //J format
				{
					j_format(IR, opCode, REG, PC, MEM);
				}
				else //I format
				{
					i_format(IR, opCode, REG, PC, MEM);
				}

				//breakpoint
				if (PC.get() < 0x400000)
					break;
			}
			break;
		case STEP_OR_SET:
			if (!PC.get())
			{
				std::cerr << "File is not loaded.\n";
				break;
			}
			if (cmd[1] == 'r')//set register
			{
				try
				{
					regno = atoi(cmd.substr(3, cmd.rfind(' ') - 3).c_str());
					if (regno == 0) throw regno;
					value = toHexString(cmd.substr(3 + cmd.substr(2, cmd.rfind(' ') - 2).length(), cmd.find('\0') - 1));
					int orig = REG[regno];
					REG[regno] = value;
					std::cout << std::hex << "REGISTER SET [" << orig << "] -> [" << REG[regno] << "]\n";
				}
				catch (int regno)
				{
					std::cerr << "$zero cannot be changed.";
				}
				catch (...)
				{
					std::cerr << "Invalid register address or value.\n";
				}
			}
			else if (cmd[1] == 'm')//set memory
			{
				try
				{
					location = toHexString(cmd.substr(3, cmd.rfind(' ') - 3));
					value = toHexString(cmd.substr(3 + cmd.substr(2, cmd.rfind(' ') - 2).length(), cmd.find('\0') - 1));
					int orig = MEM(location, BYTE);
					MEM(location, value, BYTE);
					std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << "MEMORY SET [" << orig << "] -> [" << MEM(location, BYTE) << "]\n";
				}
				catch (...)
				{
					std::cerr << "Invalid memory address or value.\n";
				}
			}
			else if (cmd[1] == '\0')//step
			{
				IR = MEM(PC.get(), WORD);
				opCode = IR >> 26;
				REG.copy();
				if (opCode == 0)	// R format
				{
					r_format(IR, opCode, REG, PC, MEM);
				}
				else if (opCode == 2 || opCode == 3) //J format
				{
					j_format(IR, opCode, REG, PC, MEM);
				}
				else //I format
				{
					i_format(IR, opCode, REG, PC, MEM);
				}
				REG.compare();
				REG.copy();
			}
			else
			{
				std::cerr << "Invalid instruction.\n";
			}
			break;
		case VIEW_MEMORY:
			try
			{
				start = toHexString(cmd.substr(2, cmd.rfind(' ')-2));
				end = toHexString(cmd.substr(2 + cmd.substr(2, cmd.rfind(' ') - 1).length(), cmd.find('\0') - 1));
				std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << start << ": ";
				for (unsigned int i = start; i < end; i++) {
					std::cout << std::setfill('0') << std::setw(2) << std::hex << MEM(i, BYTE)<<" ";
					if (i % 16 == 15)
					{
						std::cout << "\n";
						if(i+1<end) std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << i + 1 << ": ";
					}
					else if (i % 4 == 3) std::cout << "    ";
				}
				std::wcout << "\n";
			}
			catch (...)
			{
				std::cerr << "Invalid memory range.\n";
			}
			break;
		case VIEW_REGISTER:
			REG.show();
			std::cout << "PC: 0x" << std::setfill('0') << std::setw(8) << std::hex << PC.get() << "\n";
			break;
		case EXIT:
			std::cout << "PROGRAM FINISHED BY EXIT CMD.\n";
			return 0;
			break;
		default:
			std::cout << "CMD Not Supported\n";
			break;
		}
	}

	std::cout << "PROGRAM FINISHED BY THE END OF THE PROGRAM.\n";
	return 0;
}
