#pragma once
#include "useful.h"
enum ALU_commands { NONE, SLL, SRL, SRA, SLT, ADD = 8, SUB, AND = 12, OR, XOR, NOR };

int addSubtract(int X, int Y, int C);
int logicOperation(int X, int Y, int C);
int shiftOperation(int V, int Y, int C);

int checkSetLess(int X, int Y);

int ALU(int X, int Y, ALU_commands C);

/*EXAMPLE{
	int x = 1361, y = 3, z;
	std::cout << "x: %8x, y: %8x\n", x, y;

	for (int c = 0; c < 16; c++) {
		int s = ALU(x, y, c, z);
		std::cout << "\nc: %2d, s: %8x", c, s;

		if (c >= 8 && c <= 11)
			std::cout << ", z: %8x", z;

		if (c % 4 == 3) {
			if (c == 15)
				continue;
			std::cout << "\n";
		}
	}
	return;
}*/