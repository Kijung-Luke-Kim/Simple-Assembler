#include "instructions.h"

int r_format(unsigned int IR, int opcode, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	int rs{ 0 }, rt{ 0 }, rd{ 0 }, sh{ 0 }, fn{ 0 };

	fn = IR & 63;
	rs = (IR >> 21) & 31;
	rt = (IR >> 16) & 31;
	rd = (IR >> 11) & 31;
	sh = (IR >> 6) & 31;

	try
	{
		int (*ptr_inst)(int, int, int, int, Reg&, ProgramCounter&, MainMemory&) = instructions_r[fn];
		if (ptr_inst == nullptr) throw fn;
		ptr_inst(rs, rt, rd, sh, REG, PC, MEM);
	}
	catch (...)
	{
		std::cerr << "FUNCT ERROR OCCURED: " << fn << "\n";
		return 1;
	}

	return 0;
}
int i_format(unsigned int IR, int opcode, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	int rs{ 0 }, rt{ 0 };
	int16_t imm{ 0 };

	rs = (IR >> 21) & 31;
	rt = (IR >> 16) & 31;
	imm = IR & 65535; // 65535 = 2e16-1

	try
	{
		int (*ptr_inst)(int, int, int, Reg&, ProgramCounter&, MainMemory&) = instructions_i[opcode];
		if (ptr_inst == nullptr) throw opcode;
		ptr_inst(rs, rt, static_cast<int>(imm), REG, PC, MEM);
	}
	catch (...)
	{
		std::cerr << "OPCODE ERROR OCCURED: " << opcode << "\n";
		return 1;
	}

	return 0;
}
int j_format(unsigned int IR, int opcode, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	int imm{ 0 };
	imm = IR & 67108863; // 67108863 = 2e26-1  

	try
	{
		int (*ptr_inst)(int, Reg&, ProgramCounter&, MainMemory&) = instructions_j[opcode];
		if (ptr_inst == nullptr) throw opcode;
		ptr_inst(imm, REG, PC, MEM);
	}
	catch (...)
	{
		std::cerr << "OPCODE ERROR OCCURED: " << opcode << "\n";
		return 1;
	}

	return 0;
}

int bltz(int rs, int rd, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "bltz " << regnum(rs) << L * imm << "\n";
	if (REG[rs] < REG[0])
		PC.jump(L * imm);
	else
		PC.inc();
	return 0;
}

int j(int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "j 0x" << std::setfill('0') << std::setw(8) << std::hex << L * imm << std::dec << "\n";
	PC.jump(L * imm);
	return 0;
}

int jal(int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "jal 0x" << std::setfill('0') << std::setw(8) << std::hex << L * imm << std::dec << "\n";
	REG[31] = PC.get() + 4;
	PC.jump(L * imm);
	return 0;
}

int beq(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "beq " << regnum(rs) << regnum(rt) << L * imm << "\n";
	if (REG[rs] == REG[rt])
		PC.jump(L * imm);
	else
		PC.inc();
	return 0;
}

int bne(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "bne " << regnum(rs) << regnum(rt) << L * imm << "\n";
	if (!(REG[rs] == REG[rt]))
		PC.jump(PC.get()+L * imm);
	else
		PC.inc();
	return 0;
}

int addi(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "addi " << regnum(rt) << regnum(rs) << imm << "\n";
	REG[rt] = ALU(REG[rs], imm, ADD);
	PC.inc();
	return 0;
}

int slti(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "slti " << regnum(rt) << regnum(rs) << imm << "\n";
	PC.inc();
	return 0;
}

int andi(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "andi " << regnum(rt) << regnum(rs) << imm << "\n";
	REG[rt] = ALU(REG[rs], imm, AND);
	PC.inc();
	return 0;
}

int ori(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "ori " << regnum(rt) << regnum(rs) << imm << "\n";
	REG[rt] = ALU(REG[rs], imm, OR);
	PC.inc();
	return 0;
}

int xori(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "xori " << regnum(rt) << regnum(rs) << imm << "\n";
	REG[rt] = ALU(REG[rs], imm, XOR);
	PC.inc();
	return 0;
}

int lui(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "lui " << std::dec << regnum(rt) << imm << "\n";
	REG[rt] = (imm << 16);
	PC.inc();
	return 0;
}

int lb(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "lb " << regnum(rt) << imm << "( " << regnum(rs) << ")\n";
	PC.inc();
	return 0;
}

int lw(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "lw " << regnum(rt) << imm << "( " << regnum(rs) << ")\n";
	REG[rt] = MEM(REG[rs] + imm, WORD);
	PC.inc();
	return 0;
}

int lbu(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "lbu " << regnum(rt) << imm << "( " << regnum(rs) << ")\n";
	PC.inc();
	return 0;
}

int sb(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "sb " << regnum(rt) << imm << "( " << regnum(rs) << ")\n";
	PC.inc();
	return 0;
}

int sw(int rs, int rt, int imm, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "sw " << regnum(rt) << imm << "( " << regnum(rs) << ")\n";
	MEM(REG[rs] + imm, REG[rt], WORD);
	PC.inc();
	return 0;
}

int sll(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "sll " << regnum(rd) << regnum(rt) << sh << " \n";
	REG[rd] = ALU(REG[rt], sh, SLL);
	PC.inc();
	return 0;
}

int srl(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "srl " << regnum(rd) << regnum(rt) << sh << " \n";
	REG[rd] = ALU(REG[rt], sh, SRL);
	PC.inc();
	return 0;
}

int sra(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "sra " << regnum(rd) << regnum(rt) << sh << " \n";
	REG[rd] = ALU(REG[rt], sh, SRA);
	PC.inc();
	return 0;
}

int jr(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "jr " << regnum(rs) << "\n";
	PC.jump(REG[31]);
	return 0;
}

int syscall(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "syscall " << "\n";
	PC.finish();
	return 0;
}

int mfhi(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "mfhi " << regnum(rd) << "\n";
	PC.inc();
	return 0;
}

int mflo(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "mflo " << regnum(rd) << "\n";
	PC.inc();
	return 0;
}

int mul(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "mul " << regnum(rs) << regnum(rt) << "\n";
	PC.inc();
	return 0;
}

int add(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "add " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	REG[rd] = ALU(REG[rs], REG[rt], ADD);
	PC.inc();
	return 0;
}

int sub(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "sub " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	REG[rd] = ALU(REG[rs], REG[rt], SUB);
	PC.inc();
	return 0;
}

int r_and(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "and " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	REG[rd] = ALU(REG[rs], REG[rt], AND);
	PC.inc();
	return 0;
}

int r_or(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "or " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	REG[rd] = ALU(REG[rs], REG[rt], OR);
	PC.inc();
	return 0;
}

int r_xor(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "xor " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	REG[rd] = ALU(REG[rs], REG[rt], XOR);
	PC.inc();
	return 0;
}

int nor(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "nor " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	PC.inc();
	return 0;
}

int slt(int rs, int rt, int rd, int sh, Reg& REG, ProgramCounter& PC, MainMemory& MEM)
{
	std::cout << "slt " << regnum(rd) << regnum(rs) << regnum(rt) << "\n";
	PC.inc();
	return 0;
}
