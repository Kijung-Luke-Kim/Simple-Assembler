#pragma once
#include "register.h"
#include "mainmemory.h"
#include "ALU.h"
const int L{ 4 };

int r_format(unsigned int IR, int opcode, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int i_format(unsigned int IR, int opcode, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int j_format(unsigned int IR, int opcode, Reg& REG, ProgramCounter& PC, MainMemory& MEM);

//I-format
int bltz(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int beq(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int bne(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int addi(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int slti(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int andi(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int ori(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int xori(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int lui(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int lb(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int lw(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int lbu(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int sb(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int sw(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM);

static std::array<int (*) (int, int, int, Reg&, ProgramCounter&, MainMemory&), 48> instructions_i
{
	nullptr,	bltz,		nullptr,	nullptr,	beq,		bne,		nullptr,	nullptr,
	addi,		nullptr,	slti,		nullptr,	andi,		ori,		xori,		lui,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	lb,			nullptr,	nullptr,	lw,			lbu,		nullptr,	nullptr,	nullptr,
	sb,			nullptr,	nullptr,	sw,			nullptr,	nullptr,	nullptr,	nullptr
};

//J-format
int j(int L, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int jal(int L, Reg& REG, ProgramCounter& PC, MainMemory& MEM);

static std::array<int (*) (int, Reg&, ProgramCounter&, MainMemory&), 48> instructions_j
{
	nullptr,	nullptr,	j,			jal,		nullptr,	nullptr,	nullptr,	nullptr,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr
};

//R-format instructions
int sll(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int srl(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int sra(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int jr(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int syscall(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int mfhi(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int mflo(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int mul(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int add(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int sub(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int r_and(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int r_or(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int r_xor(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int nor(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);
int slt(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM);

static std::array<int (*) (int, int, int, int, Reg&, ProgramCounter&, MainMemory&), 48>instructions_r
{
	sll,		nullptr,	srl,		sra,		nullptr,	nullptr,	nullptr,	nullptr,
	jr,			nullptr,	nullptr,	nullptr,	syscall,	nullptr,	nullptr,	nullptr,
	mfhi,		nullptr,	mflo,		nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	mul,		nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,	nullptr,
	add,		nullptr,	sub,		nullptr,	r_and,		r_or,		r_xor,		nor,
	nullptr,	nullptr,	slt,		nullptr,	nullptr,	nullptr,	nullptr,	nullptr
};

