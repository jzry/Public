// Simulates a single MIPS data cycle.

#include "spimcore.h"

void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    *Zero = ((A - B) == 0) ? 1 : 0;

	switch(ALUControl)
    {
		case 0x0:
			*ALUresult = A + B; // ADD
			break;

		case 0x1:
			*ALUresult = A - B; // SUB
			break;

        case 0x2:
            *ALUresult = ((int) A < (int) B) ? 1 : 0; // SLT SIGNED
            break;

		case 0x3:
			*ALUresult = (A < B) ? 1 : 0; // SLT UNSIGNED
			break;

		case 0x4:
			*ALUresult = A & B; // AND
			break;

		case 0x5:
			*ALUresult = A | B; // OR
			break;

		case 0x6:
			*ALUresult = B << 16; // SLL 16 BITS
			break;

		case 0x7:
			*ALUresult = ~A; // NOT
	}
}

int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // Word is not aligned or word is out of bounds. Send halt condition.
    if ( PC % 4 == 1 )
    {
        return 1;
    }

    *instruction = Mem[PC >> 2];
    return 0;
}

void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	*op     = (instruction & 0xfc000000) >> 26; // Instruction 31-26
	*r1     = (instruction & 0x03e00000) >> 21; // Instruction 25-21
	*r2     = (instruction & 0x001f0000) >> 16; // Instruction 20-16
	*r3     = (instruction & 0x0000f800) >> 11; // Instruction 15-11
	*funct  =  instruction & 0x0000003f; // Instruction 5-0
	*offset =  instruction & 0x0000ffff; // Instruction 15-0
	*jsec   =  instruction & 0x03ffffff; // Instruction 25-0
}

int instruction_decode(unsigned op,struct_controls *controls)
{
	switch(op)
    {
		case 0x0: // R-type instruction
		 	*controls = (struct_controls) {1,0,0,0,0,7,0,0,1};
			break;

		case 0x8: // ADDI
			*controls = (struct_controls) {0,0,0,0,0,0,0,1,1};
			break;

		case 0x23: // LW
			*controls = (struct_controls) {0,0,0,1,1,0,0,1,1};
			break;

		case 0x2b: // SW
			*controls = (struct_controls) {0,0,0,0,0,0,1,1,0};
			break;

		case 0xf: // LUI
			*controls = (struct_controls) {0,0,0,0,0,6,0,1,1};
			break;

		case 0x4: // BEQ
			*controls = (struct_controls) {2,0,1,0,2,1,0,2,0};
			break;

		case 0xa: // SLTI
			*controls = (struct_controls) {0,0,0,0,0,2,0,1,1};
			break;

		case 0xb: // SLTIU
			*controls = (struct_controls) {0,0,0,0,0,3,0,1,1};
			break;

		case 0x2: // JUMP
			*controls = (struct_controls) {2,1,2,0,2,0,0,2,0};
			break;

		default: // HALT
			return 1;
	}

	return 0;
}

void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}

void sign_extend(unsigned offset,unsigned *extended_value)
{
    // Check if offset is negative.
    if ((offset >> 15) == 1)
    {
        // MSB negative, fill with ones.
        *extended_value = offset | 0xffff0000;
    }
    else
    {
        // MSB positive, do not fill.
        *extended_value = offset & 0x0000ffff;
    }
}

int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    unsigned char ALUctrl = ALUOp;

	switch (ALUOp)
    {
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
			break;
		case 0x7:
			switch(funct)
            {
				case 0x20:
					ALUctrl = 0x0;
                    break;
				case 0x24:
					ALUctrl = 0x4;
                    break;
				case 0x25:
					ALUctrl = 0x5;
                    break;
				case 0x2a:
					ALUctrl = 0x2;
                    break;
				case 0x2b:
					ALUctrl = 0x3;
                    break;
				default:
					return 1;
			}
            break;
		default: // HALT
			return 1;
	}

	ALU( data1, (ALUSrc == 1) ? extended_value : data2, ALUctrl, ALUresult, Zero);
	return 0;
}

int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if (MemRead  == 1)
    {
		if( ALUresult % 4 == 0 && ALUresult < 65536 )
			*memdata = Mem[ALUresult >> 2]; // Read memory
		else
			return 1; // Halt
    }

	if (MemWrite == 1)
    {
		if ( ALUresult % 4 == 0 && ALUresult < 65536 )
			Mem[ALUresult >> 2] = data2; // Write memory
		else
			return 1; // Halt
    }

	return 0;

}

void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if (RegWrite == 1)
    {
        // I-type instruction. RegDst is 0.
        if (MemtoReg == 1)
        {
            Reg[r2] = memdata;
        }

        // Determine register source.
        if (MemtoReg == 0)
        {
            if (RegDst == 1)
            {
                // R-type instruction -> r3.
                Reg[r3] = ALUresult;
            }
            else
            {
                // I-type instruction -> r2.
                Reg[r2] = ALUresult;
            }
        }
    }
}

void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC += 4;

    // Increment program counter by offset.
    if (Zero == 1 && Branch == 1)
    {
        *PC += (extended_value << 2);
    }

    // For jump instruction.
    if (Jump == 1)
    {
        *PC = (*PC & 0xf000000) | (jsec << 2);
    }
}
