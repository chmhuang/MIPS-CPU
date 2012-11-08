#include <stdio.h>
#include <stdlib.h>

#include "processor.h"
#include "disassemble.h"
#include <stdint.h> // added to check

void execute_one_inst(processor_t* p, int prompt, int print_regs)
{
    uint32_t tmp;
    inst_t inst;

    int32_t signext8(int32_t n) {
	n = n << 24;
	n = n >> 24;
	return n;
    }
    int32_t signext(int32_t n) {
	n = n << 16;
	n = n >> 16;
	return n;
    }
    /* fetch an instruction */
    inst.bits = load_mem(p->pc, SIZE_WORD);

    /* interactive-mode prompt */
    if(prompt)
	{
	    if (prompt == 1) {
		printf("simulator paused, enter to continue...");
		while(getchar() != '\n')
		    ;
	    }
	    printf("%08x: ",p->pc);
	    disassemble(inst);
	}

    switch (inst.rtype.opcode) /* could also use e.g. inst.itype.opcode */
	{
	case 0x0: // opcode == 0x0 (SPECIAL)

	    switch (inst.rtype.funct)
		{
		case 0x0: // funct == 0x0 (SLL)
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rt] << inst.rtype.shamt; //? do i need to extend the shamt?
		    p->pc += 4;
		    break;
	
		case 0x2: // funct == 0x2 (SRL)
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rt] >> inst.rtype.shamt; //? do i need to extend the shamt?
		    p->pc += 4;
		    break;
    
		case 0x3: // funct == 0x3 (SRA)
		    p->R[inst.rtype.rd] = ((int32_t) p->R[inst.rtype.rt]) >> inst.rtype.shamt; //? do i need to extend the shamt?
		    p->pc += 4;
		    break;
	
		case 0x8: // funct == 0x8 (JR)
		    p->pc = p->R[inst.rtype.rs];
		    break;
	  
		case 0x9: // funct == 0x9 (JALR)
		    tmp = p->pc + 4;
		    p->pc = p->R[inst.rtype.rs];
		    p->R[inst.rtype.rd] = tmp;
		    break;	
	
		case 0xc: // funct == 0xc (SYSCALL) DEFINED BY SKELETON
		    handle_syscall(p);
		    p->pc += 4;
		    break;
	  
		case 0x21: // funct == 0x21 (ADDU)
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rs] + p->R[inst.rtype.rt];
		    p->pc += 4;
		    break;
	  
		case 0x23: // funct == 0x23 (SUBU)
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rs] - p->R[inst.rtype.rt];
		    p->pc += 4;
		    break;

		case 0x24: // funct == 0x24 (AND)
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rs] & p->R[inst.rtype.rt];
		    p->pc += 4;
		    break;
	  
		case 0x25: // funct == 0x25 (OR) DEFINED BY SKELETON
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rs] | p->R[inst.rtype.rt];
		    p->pc += 4;
		    break;

		case 0x26: // funct == 0x26 (XOR)
		    p->R[inst.rtype.rd] = p->R[inst.rtype.rs] ^ p->R[inst.rtype.rt];
		    p->pc += 4;
		    break;
	
		case 0x27: // funct == 0x27 (NOR)
		    p->R[inst.rtype.rd] = ~(p->R[inst.rtype.rs] | p->R[inst.rtype.rt]);
		    p->pc += 4;
		    break;
	  
		case 0x2a: // funct == 0x2a (SLT) ?how do i do this?
		    if (((int32_t) p->R[inst.rtype.rs]) < ((int32_t) p->R[inst.rtype.rt])) {
			p->R[inst.rtype.rd] = 1;
			p->pc += 4;
		    } else {
			p->R[inst.rtype.rd] = 0;
			p->pc += 4;
		    }
		    break;
	  
		case 0x2b: // funct == 0x2b (SLTU)
		    if (p->R[inst.rtype.rs] < p->R[inst.rtype.rt]) {
			p->R[inst.rtype.rd] = 1;
			p->pc += 4;
		    } else {
			p->R[inst.rtype.rd] = 0;
			p->pc += 4;
		    }
		    break;
	  
		default: // undefined funct
		    fprintf(stderr, "%s: pc=%08x, illegal instruction=%08x\n", __FUNCTION__, p->pc, inst.bits);
		    exit(-1);
		    break;
		}
	    break;

	case 0xc: // opcode == 0xc (ANDI) 
	    p->R[inst.itype.rt] = p->R[inst.itype.rs] & inst.itype.imm;
	    p->pc += 4;
	    break;
	
	case 0xd: // opcode == 0xd (ORI) DEFINED BY SKELETON
	    p->R[inst.itype.rt] = p->R[inst.itype.rs] | inst.itype.imm;
	    p->pc += 4;
	    break;
	
	case 0xe: // opcode == 0xe (XORI)
	    p->R[inst.itype.rt] = p->R[inst.itype.rs] ^ inst.itype.imm;
	    p->pc += 4;
	    break;

	case 0xf: // opcode == 0xf (LUI)
	    p->R[inst.itype.rt] = (inst.itype.imm << 16);
	    p->pc += 4;
	    break;

	    /*opcode == 0x20 (LB). */
	case 0x20:
	    p->R[inst.itype.rt] = signext8(load_mem(p->R[inst.itype.rs] +
						   signext(inst.itype.imm), SIZE_BYTE));
	     p->pc += 4;
	    break;

	    /*opcode == 0x21 (Lh). */
	case 0x21:
	    p->R[inst.itype.rt] = signext(load_mem(p->R[inst.itype.rs] +
						   signext(inst.itype.imm), SIZE_HALF_WORD));
	     p->pc += 4;
	    break;

	    /*opcode == 0x23 (LW). */
	case 0x23:
	    p->R[inst.itype.rt] = load_mem(p->R[inst.itype.rs] +
						   signext(inst.itype.imm), SIZE_WORD);
	     p->pc += 4;
	    break;

	    /*opcode == 0x24 (Lbu). */
	case 0x24:
	    p->R[inst.itype.rt] = load_mem(p->R[inst.itype.rs] +
					   signext(inst.itype.imm), SIZE_BYTE);
	     p->pc += 4;
	    break;

	    /*opcode == 0x25 (Lhu). */
	case 0x25:
	    p->R[inst.itype.rt] = load_mem(p->R[inst.itype.rs] +
					   signext(inst.itype.imm), SIZE_HALF_WORD);
	     p->pc += 4;
	    break;


	    /*opcode == 0x28 (sb). */
	case 0x28:
	    store_mem(p->R[inst.itype.rs] + signext(inst.itype.imm),
		      SIZE_BYTE, p->R[inst.itype.rt]);
	     p->pc += 4;
	    break;

	    /*opcode == 0x29 (sh). */
	case 0x29:
	    store_mem(p->R[inst.itype.rs] +
		      signext(inst.itype.imm), SIZE_HALF_WORD,
		      p->R[inst.itype.rt]); 
	    p->pc += 4;
	    break;

	    /*opcode == 0x2b (sw). */
	case 0x2b:
	    store_mem(p->R[inst.itype.rs] +
		      signext(inst.itype.imm), SIZE_WORD,
		      p->R[inst.itype.rt]);
	     p->pc += 4;
	    break;
	    
	case 0x2: // opcode == 0x2 (J) DEFINED BY SKELETON
	    p->pc = ((p->pc+4) & 0xf0000000) | (inst.jtype.addr << 2);
	    break;

	case 0x3: // opcode == 0x3 (JAL) ?
	    p->R[31] = p->pc + 4;
	    p->pc = ((p->pc+4) & 0xf0000000) | (inst.jtype.addr << 2);
	    break;
	
	case 0x4: // opcode == 0x4 (BEQ) ?
	    if (p->R[inst.itype.rs] == p->R[inst.itype.rt]) {
		p->pc = p->pc + 4 + ((signext(inst.itype.imm)) * 4);//??
	    }
	    else {	
		p->pc += 4;
	    }
	    break;
	
	case 0x5: // opcode == 0x5 (BNE) ?
	    if (p->R[inst.itype.rs] != p->R[inst.itype.rt]) {
		p->pc = p->pc + 4 + ((signext(inst.itype.imm)) * 4);//??
	    } else {
		p->pc += 4;
	    }
	    
	    break;
	    
	case 0x9: // opcode == 0x9 (ADDIU) ?
	    p->R[inst.itype.rt] = p->R[inst.itype.rs] + signext(inst.itype.imm);
	    p->pc += 4;
	    break;
	    
	case 0xa: // opcode == 0xa (SLTI) ?
	    if (((int32_t) p->R[inst.rtype.rs]) < signext(p->R[inst.rtype.rt])) {
		p->R[inst.rtype.rd] = 1;
		p->pc += 4;
	    } else {
		p->R[inst.rtype.rd] = 0;
		p->pc += 4;
	    }
	    break;
	    
	
	case 0xb: // opcode == 0xb (SLTIU) ?
	    if (p->R[inst.rtype.rs] < signext(p->R[inst.rtype.rt])) {
		p->R[inst.rtype.rd] = 1;
		p->pc += 4;
	    } else {
		p->R[inst.rtype.rd] = 0;
		p->pc += 4;
	    }
	    break;

	
	default: // undefined opcode
	    fprintf(stderr, "%s: pc=%08x, illegal instruction: %08x\n", __FUNCTION__, p->pc, inst.bits);
	    exit(-1);
	    break;
	}

    // enforce $0 being hard-wired to 0
    p->R[0] = 0;

    if (print_regs)
	print_registers(p);
}

void init_processor(processor_t* p)
{
    int i;

    /* initialize pc to 0x1000 */
    p->pc = 0x1000;

    /* zero out all registers */
    for (i=0; i<32; i++)
	{
	    p->R[i] = 0;
	}
}

void print_registers(processor_t* p)
{
    int i,j;
    for (i=0; i<8; i++)
	{
	    for (j=0; j<4; j++)
		printf("r%2d=%08x ",i*4+j,p->R[i*4+j]);
	    puts("");
	}
}

void handle_syscall(processor_t* p)
{
    reg_t i;

    switch (p->R[2]) // syscall number is given by $v0 ($2)
	{
	case 1: // print an integer
	    printf("%d", p->R[4]);
	    break;

	case 4: // print a string
	    for(i = p->R[4]; i < MEM_SIZE && load_mem(i, SIZE_BYTE); i++)
		printf("%c", load_mem(i, SIZE_BYTE));
	    break;

	case 10: // exit
	    printf("exiting the simulator\n");
	    exit(0);
	    break;

	case 11: // print a character
	    printf("%c", p->R[4]);
	    break;

	default: // undefined syscall
	    fprintf(stderr, "%s: illegal syscall number %d\n", __FUNCTION__, p->R[2]);
	    exit(-1);
	    break;
	}
}
