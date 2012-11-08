#include <stdio.h>
#include <stdlib.h>

#include "disassemble.h"

void disassemble(inst_t inst)
{
    /* you could use itype or type instead of rtype */
    /* remember that opcode field is at the same place for all types */
    switch (inst.rtype.opcode)
	{
	    /* opcode == 0x0 (SPECIAL). */
	case 0x0:

	    switch (inst.rtype.funct)
		{
		    /* funct == 0x0 (sll rd, rt, shamt). */
		case 0x0:
		    printf("sll\t$%d,$%d,%u\n",
			   inst.rtype.rd, inst.rtype.rt, (inst.rtype.shamt)); //?fix bug
		    break;

		    /* funct == 0x2 (srl rd, rt, shamt). */
		case 0x2:
		    printf("srl\t$%d,$%d,%u\n",
			   inst.rtype.rd, inst.rtype.rt,  inst.rtype.shamt); //?fix bug
		    break;	
		    
		    /* funct == 0x3 (sra rd, rt, shamt). */
		case 0x3:
		    printf("sra\t$%d,$%d,%u\n",
			   inst.rtype.rd, inst.rtype.rt,  inst.rtype.shamt); //?fix bug
		    break;

		    /* funct == 0x8 (jr rs). */
		case 0x8:
		    printf("jr\t%d\n",
			    inst.rtype.rs); //?fix bug
		    break;

		    /* funct == 0x9 (jalr rd, rs). */
		case 0x9:
		    printf("jalr\t$%d,$%d\n",
			   inst.rtype.rd,  inst.rtype.rs); //?fix bug
		    break;

		    /* funct == 0xc (SYSCALL). */
		case 0xc:
		    printf("syscall\n");
		    break;
		    
		    /* funct == 0x21 (addu rd, rs, rt). */
		case 0x21:
		    printf("addu\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x23 (subu rd, rs, rt). */
		case 0x23:
		    printf("subu\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x24 (and rd, rs, rt). */
		case 0x24:
		    printf("and\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x25 (or rt, rs, rt) */
		case 0x25:
		    printf("or\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x26 (xor rd, rs, rt) */
		case 0x26:
		    printf("xor\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x27 (nor rd, rs, rt) */
		case 0x27:
		    printf("nor\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x2a (slt rd, rs, rt) */
		case 0x2a:
		    printf("slt\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* funct == 0x2b (sltu rd, rs, rt) */
		case 0x2b:
		    printf("sltu\t$%d,$%d,$%d\n",
			   inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
		    break;

		    /* undefined funct. */
		default: // undefined funct
		    fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
		    exit(-1);
		    break;
		}
	    break;
    
	    /*opcode == 0x4 (BEQ). */
	case 0x4:
	    printf("beq\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, ((int16_t) inst.itype.imm) * 4);
	    break;

	    /*opcode == 0x5 (BNE). */
	case 0x5:
	    printf("bne\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, ((int16_t) inst.itype.imm) * 4);
	    break;

	    /*opcode == 0x9 (ADDIU). */
	case 0x9:
	    printf("addiu\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, ((int16_t) inst.itype.imm));
	    break;

	    /*opcode == 0xa (SLTI). */
	case 0xa:
	    printf("slti\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, ((int16_t) inst.itype.imm));
	    break;

	    /*opcode == 0xb (SLTIU). */
	case 0xb:
	    printf("sltiu\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, ((int16_t) inst.itype.imm));
	    break;

	    /*opcode == 0xc (ANDI). */
	case 0xc:
	    printf("andi\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, ((int16_t) inst.itype.imm));
	    break;
	    
	    /*opcode == 0xd (ORI). */
	case 0xd:
	    printf("ori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, ((int16_t) inst.itype.imm));
	    break;

	    /*opcode == 0xe (XORI). */
	case 0xe:
	    printf("xori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, ((int16_t) inst.itype.imm));
	    break;

	    /*opcode == 0xf (LUI). */
	case 0xf:
	    printf("lui\t$%d,0x%x\n", inst.itype.rt, ((int16_t) inst.itype.imm));
	    break;

	     /*opcode == 0x20 (LB). */
	case 0x20:
	    printf("lb\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;

	    /*opcode == 0x21 (LH). */
	case 0x21:
	    printf("lh\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;

	    /*opcode == 0x23 (LW). */
	case 0x23:
	    printf("lw\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;
	    
	    /*opcode == 0x24 (LBU). */
	case 0x24:
	    printf("lbu\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;

	    /*opcode == 0x25 (LHU). */
	case 0x25:
	    printf("lhu\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;

	    /*opcode == 0x28 (SB). */
	case 0x28:
	    printf("sb\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;

	    /*opcode == 0x29 (SH). */
	case 0x29:
	    printf("sh\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;

	     /*opcode == 0x2b (SW). */
	case 0x2b:
	    printf("sw\t$%d,%d($%d)\n", inst.itype.rt, ((int16_t) inst.itype.imm), inst.itype.rs);
	    break;
	    
	    /* opcode == 0x2 (J target). */
	case 0x2:
	    printf("j\t0x%x\n", ((uint32_t) inst.jtype.addr * 4)); //?fix bug
	    break;

	     /* opcode == 0x3 (JAL target). */
	case 0x3:
	    printf("jal\t0x%x\n", ((uint32_t) inst.jtype.addr * 4)); //?fix bug
	    break;

	    /* undefined opcode. */
	default:
	    fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
	    exit(-1);
	    break;
	}
}
