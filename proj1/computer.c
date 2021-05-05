#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "computer.h"
#undef mips			/* gcc already has a def for mips */

unsigned int endianSwap(unsigned int);

void PrintInfo (int changedReg, int changedMem);
unsigned int Fetch (int);
void Decode (unsigned int, DecodedInstr*, RegVals*);
int Execute (DecodedInstr*, RegVals*);
int Mem(DecodedInstr*, int, int *);
void RegWrite(DecodedInstr*, int, int *);
void UpdatePC(DecodedInstr*, int);
void PrintInstruction (DecodedInstr*);

/*Globally accessible Computer variable*/
Computer mips;
RegVals rVals;

// Test with commands like './sim -i sample.dump'

/*
 *  Return an initialized computer with the stack pointer set to the
 *  address of the end of data memory, the remaining registers initialized
 *  to zero, and the instructions read from the given file.
 *  The other arguments govern how the program interacts with the user.
 */
void InitComputer (FILE* filein, int printingRegisters, int printingMemory,
  int debugging, int interactive) {
    int k;
    unsigned int instr;

    /* Initialize registers and memory */

    for (k=0; k<32; k++) {
        mips.registers[k] = 0;
    }
    
    /* stack pointer - Initialize to highest address of data segment */
    mips.registers[29] = 0x00400000 + (MAXNUMINSTRS+MAXNUMDATA)*4;

    for (k=0; k<MAXNUMINSTRS+MAXNUMDATA; k++) {
        mips.memory[k] = 0;
    }

    k = 0;
    while (fread(&instr, 4, 1, filein)) {
	/*swap to big endian, convert to host byte order. Ignore this.*/
        mips.memory[k] = ntohl(endianSwap(instr));
        k++;
        if (k>MAXNUMINSTRS) {
            fprintf (stderr, "Program too big.\n");
            exit (1);
        }
    }

    mips.printingRegisters = printingRegisters;
    mips.printingMemory = printingMemory;
    mips.interactive = interactive;
    mips.debugging = debugging;
}

unsigned int endianSwap(unsigned int i) {
    return (i>>24)|(i>>8&0x0000ff00)|(i<<8&0x00ff0000)|(i<<24);
}

/*
 *  Run the simulation.
 */
void Simulate () {
    char s[40];  /* used for handling interactive input */
    unsigned int instr;
    int changedReg=-1, changedMem=-1, val;
    DecodedInstr d;
    
    /* Initialize the PC to the start of the code section */
    mips.pc = 0x00400000;
    while (1) {
        if (mips.interactive) {
            printf ("> ");
            fgets (s,sizeof(s),stdin);
            if (s[0] == 'q') {
                return;
            }
        }

        /* Fetch instr at mips.pc, returning it in instr */
        instr = Fetch (mips.pc);

        printf ("Executing instruction at %8.8x: %8.8x\n", mips.pc, instr);

        /* 
	 * Decode instr, putting decoded instr in d
	 * Note that we reuse the d struct for each instruction.
	 */
        Decode (instr, &d, &rVals);

        /*Print decoded instruction*/
        PrintInstruction(&d);

        /* 
	 * Perform computation needed to execute d, returning computed value 
	 * in val 
	 */
        val = Execute(&d, &rVals);

	UpdatePC(&d,val);

        /* 
	 * Perform memory load or store. Place the
	 * address of any updated memory in *changedMem, 
	 * otherwise put -1 in *changedMem. 
	 * Return any memory value that is read, otherwise return -1.
         */
        val = Mem(&d, val, &changedMem);

        /* 
	 * Write back to register. If the instruction modified a register--
	 * (including jal, which modifies $ra) --
         * put the index of the modified register in *changedReg,
         * otherwise put -1 in *changedReg.
         */
        RegWrite(&d, val, &changedReg);

        PrintInfo (changedReg, changedMem);
    }
}

/*
 *  Print relevant information about the state of the computer.
 *  changedReg is the index of the register changed by the instruction
 *  being simulated, otherwise -1.
 *  changedMem is the address of the memory location changed by the
 *  simulated instruction, otherwise -1.
 *  Previously initialized flags indicate whether to print all the
 *  registers or just the one that changed, and whether to print
 *  all the nonzero memory or just the memory location that changed.
 */
void PrintInfo ( int changedReg, int changedMem) {
    int k, addr;
    printf ("New pc = %8.8x\n", mips.pc);
    if (!mips.printingRegisters && changedReg == -1) {
        printf ("No register was updated.\n");
    } else if (!mips.printingRegisters) {
        printf ("Updated r%2.2d to %8.8x\n",
        changedReg, mips.registers[changedReg]);
    } else {
        for (k=0; k<32; k++) {
            printf ("r%2.2d: %8.8x  ", k, mips.registers[k]);
            if ((k+1)%4 == 0) {
                printf ("\n");
            }
        }
    }
    if (!mips.printingMemory && changedMem == -1) {
        printf ("No memory location was updated.\n");
    } else if (!mips.printingMemory) {
        printf ("Updated memory at address %8.8x to %8.8x\n",
        changedMem, Fetch (changedMem));
    } else {
        printf ("Nonzero memory\n");
        printf ("ADDR	  CONTENTS\n");
        for (addr = 0x00400000+4*MAXNUMINSTRS;
             addr < 0x00400000+4*(MAXNUMINSTRS+MAXNUMDATA);
             addr = addr+4) {
            if (Fetch (addr) != 0) {
                printf ("%8.8x  %8.8x\n", addr, Fetch (addr));
            }
        }
    }
}

/*
 *  Return the contents of memory at the given address. Simulates
 *  instruction fetch. 
 */
unsigned int Fetch ( int addr) {
    return mips.memory[(addr-0x00400000)/4];
}

/* Decode instr, returning decoded instruction. */
void Decode ( unsigned int instr, DecodedInstr* d, RegVals* rVals) {
    /* Your code goes here */
    int opcode, rs, rt, rd, shamt;
    unsigned int funct, target, addrOrImm;

    // get the opcode, which is the first 6 bits of the instruction
    opcode = instr>>26; // shift right by 26 bits, as we only want the first 6
    d->op = opcode;

    if (opcode == 0) { // all R type instructions have opcode 0
        // type
        d->type = R; // decoded instruction's type is R

        // rs
        rs = instr<<6; // shift 6 bits, as this ignores the opcode which we've already accounted for
        d->regs.r.rs = rs>>27; // we also want to ignore the 27 other bits after rs

        // rt
        rt = instr<<11; // shift 11 bits, as the first 6 represent opcode and the next 5 are rs
        d->regs.r.rt = rt>>27; // similarly as above, ignore the 27 bits after rt

        // rd
        rd = instr<<16; // shift 16 bits, the first 6 are opcode, next 5 are rs, and next 5 are rt
        d->regs.r.rd = rd>>27; //ignore the 27 bits after rd

        // shamt
        shamt = instr<<21; // shift 21 bits, because the first 21 bits account for opcode, rs, rt, and rd
        d->regs.r.shamt = shamt>>27; // ignore the 27 bits after shamt

        // funct
        funct = instr<<26; // shift 26 bits
        d->regs.r.funct = funct>>26; // ignore the 26 bits after funct, because we need 6 bits for funct

        // these will be used in Execute stage
        rVals->R_rs = mips.registers[d->regs.r.rs];
        rVals->R_rt = mips.registers[d->regs.r.rt];
        rVals->R_rd = mips.registers[d->regs.r.rd];
    } else if (opcode == 2 || opcode == 3) { // j instructions have opcodes 2 or 3
        // type
        d->type = J;

        // target
        target = instr<<6; // shift 6 bits, ignoring the opcode since it's already accounted for
        target = target>>4; // shift 4 bits right, as the target is only 26 bits so ignore the rest and we stay within the memory limits (0x00401000 - 0x00403fff)

        d->regs.j.target = target;
    } else { // the others represent i instructions
        // type
        d->type = I;

        // rs
        rs = instr<<6; // shift 6 bits, ignoring the opcode since it's accounted for
        d->regs.i.rs = rs>>27; // ignore the 27 bits after rs

        // rt
        rt = instr<<11; // shift 11 bits, ignoring the opcode and rs
        d->regs.i.rt = rt>>27; // ignore the 27 bits after rt

        // addr or imm
        addrOrImm = instr<<16; // shift 16 bits, ignoring the opcode, rs, and rt
        d->regs.i.addr_or_immed = addrOrImm>>16; // shift 16 bits, ignoring everything after addrOrImm

        // these will be used in Execute stage
        rVals->R_rs = mips.registers[d->regs.i.rs];
        rVals->R_rt = mips.registers[d->regs.i.rt];
    }
}

/*
 *  Print the disassembled version of the given instruction
 *  followed by a newline.
 */
void PrintInstruction ( DecodedInstr* d) { 
    /* Your code goes here */
    if (d->type == R) { // print out from the supported R format instructions, which are addu, subu, sll, srl, and, or, slt, jr
        if (d->regs.r.funct == 33) { // if instruction's function code is 33, then it's an addu instr
            printf("addu\t$%d, $%d, $%d\n", d->regs.r.rd, d->regs.r.rs, d->regs.r.rt); // use '%d' to print the decimal value of d->regs.r.rd, rs, and rt
        } else if (d->regs.r.funct == 36) { // if function code is 36, it's an and instr
            printf("and\t$%d, $%d, $%d\n", d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
        } else if (d->regs.r.funct == 8) { // if function code is , it's a jr instr
            printf("jr\t$%d\n", d->regs.r.rs); // jr only uses one register, rs
        } else if (d->regs.r.funct == 0) { // if function code is 0, it's an sll instr
            printf("sll\t$%d, $%d, %d\n", d->regs.r.rd, d->regs.r.rs, d->regs.r.shamt);
        } else if (d->regs.r.funct == 42) { // if function code is 42, it's an slt instr
            printf("slt\t$%d, $%d, $%d\n", d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
        } else if (d->regs.r.funct == 2) { // if function code is 2, it's an srl instr
            printf("srl\t$%d, $%d, %d\n", d->regs.r.rd, d->regs.r.rs, d->regs.r.shamt);
        } else if (d->regs.r.funct == 35) { // if function code is 35, it's a subu instr
            printf("subu\t$%d, $%d, $%d\n", d->regs.r.rd, d->regs.r.rs, d->regs.r.rt);
        } else { // if unsupported instruction, terminate
            exit(0);
        }
    } else if (d->type == I) { // print out from the supported I format instructions, which are addiu, andi, ori, lui, beq, bne, lw, sw
        if (d->op == 9) { // addiu has op/funct code 9
            printf("addiu\t$%d, $%d, %d\n", d->regs.i.rt, d->regs.i.rs, d->regs.i.addr_or_immed); // addiu rdest, rsrc, imm
        } else if (d->op == 12) { // andi has op/funct code c, which is 12 in hex
            printf("andi\t$%d, $%d, %d\n", d->regs.i.rt, d->regs.i.rs, d->regs.i.addr_or_immed); // andi rdest, rsrc, imm
        } else if (d->op == 13) { // ori has op/funct code d, which is 13 in hex
            printf("ori\t$%d, $%d, %d\n", d->regs.i.rt, d->regs.i.rs, d->regs.i.addr_or_immed); // ori rdest, rsrc, imm
        } else if (d->op == 15) { // lui has op/funct code f, which is 15 in hex
            printf("lui\t$%d, 0x%x\n", d->regs.i.rt, d->regs.i.addr_or_immed); // lui rdest, imm
        } else if (d->op == 4) { // beq has op/funct code 4
            printf("beq\t$%d, $%d, 0x%x\n", d->regs.i.rt, d->regs.i.rs, (d->regs.i.addr_or_immed + 4 + mips.pc)); // beq rdest, rsrc, addr, PC = PC + 4 + addr (from MIPS reference sheet)
        } else if (d->op == 5) { // bne has op/funct code 5
            printf("bne\t$%d, $%d, 0x%x\n", d->regs.i.rt, d->regs.i.rs, (d->regs.i.addr_or_immed + 4 + mips.pc)); // bne rdest, rsrc, addr
        } else if (d->op == 35) { // lw has op/funct code 23
            printf("lw\t$%d,", d->regs.i.rt); // lw rdest offset(rsrc)
            printf(" %d", d->regs.i.addr_or_immed);
            printf("($%d)\n", d->regs.r.rs);
        } else if (d->op == 43) { // sw has op/funct code 43
            printf("sw\t$%d,", d->regs.i.rt); // sw rdest offset(rsrc)
            printf(" %d", d->regs.i.addr_or_immed);
            printf("($%d)\n", d->regs.r.rs);
        } else { // if unsupported instruction, terminate
            exit(0);
        }
    } else if (d->type == J) { // print out from the supported J format instructions, which are j and jal
        if (d->op == 2) { // instruction has opcode 2, which refers to a j instr
            printf("j\t0x%x\n", d->regs.j.target); // use '%x' to print the hexadecimal value
        } else if (d->op == 3) { // instruction has opcode 3, which refers to a jal instr
            printf("jal\t0x%x\n", d->regs.j.target);
        } else { // if unsupported instruction, terminate
            exit(0);
        }
    }
}

/* Perform computation needed to execute d, returning computed value */
int Execute ( DecodedInstr* d, RegVals* rVals) { 
    /* Your code goes here */
    if (d->type == R) { // all R format instructions
        if (d->regs.r.funct == 33) { // if funct code is 33, then this is an addu instr
            return (rVals->R_rs + rVals->R_rt); // R_rd = R_rs + R_rt
        } else if (d->regs.r.funct == 36) { // if funct code is 36, it's an and instr
            return (rVals->R_rs & rVals->R_rt); // R_rd = R_rs & R_rt
        } else if (d->regs.r.funct == 8) { // jr
            return (rVals->R_rs); // PC = R_rs
        } else if (d->regs.r.funct == 42) { // slt
            if (rVals->R_rs < rVals->R_rt) { // R_rd = (R_rs < R_rt) ? 1:0
                return 1;
            } else {
                return 0;
            }
        } else if (d->regs.r.funct == 0) { // sll
            return ((unsigned int)rVals->R_rt << d->regs.r.shamt); // R_rd = R_rt << shamt
        } else if (d->regs.r.funct == 2) { // srl
            return ((unsigned int)rVals->R_rt >> d->regs.r.shamt); // R-Rd = R_rt >> shamt
        } else if (d->regs.r.funct == 35) { // subu
            return (rVals->R_rs - rVals->R_rt); // R_rd = R_rs + R_rt
        } 
    } else if (d->type == I) { // all I format instructions
        if (d->op == 9) { // addiu
            return (rVals->R_rs + d->regs.i.addr_or_immed); // R_rt = R_rs + imm
        } else if (d->op == 12) { // andi
            return (rVals->R_rs & d->regs.i.addr_or_immed); // R_rt = R_rs & imm
        } else if (d->op == 13) { // ori
            return (rVals->R_rs | d->regs.i.addr_or_immed); // R_rt = R_rs | imm
        } else if (d->op == 15) { // lui
            return (d->regs.i.addr_or_immed << 16); // R_rt = {imm, 16'b0}, shift to ignore the 16 lower bits as we only want the 16 upper bits
        } else if (d->op == 4) { // beq
            if (rVals->R_rs == rVals->R_rt) {
                return (d->regs.i.addr_or_immed); // return the address we're branching to
            }
        } else if (d->op == 5) { // bne
            if (rVals->R_rs != rVals->R_rt) {
                return (d->regs.i.addr_or_immed); // return the address we're branching to
            }
        } else if (d->op == 35) { // lw
            return (rVals->R_rs + d->regs.i.addr_or_immed); // R_rt = M[R_rs + imm]
        } else if (d->op == 43) { // sw
            return (rVals->R_rs + d->regs.i.addr_or_immed); // M[R_rs + imm] = R_rt
        }
    } else if (d->type == J) { // all J format instructions
        if (d->op == 2) { // j
            return mips.pc += 4; // update PC
        } else if (d->op == 3) { // jal
            return mips.pc += 4; // update PC
        }
    }
  return 0;
}

/* 
 * Update the program counter based on the current instruction. For
 * instructions other than branches and jumps, for example, the PC
 * increments by 4 (which we have provided).
 */
void UpdatePC ( DecodedInstr* d, int val) {
    mips.pc+=4;
    /* Your code goes here */

    // according to MIPS reference sheet, PC is only updated with beq, bne, j, jr, and jal
    if (d->type == R) { // all R format instructions that change PC, which is jr
        if (d->regs.r.funct == 8) { // jr
            mips.pc = val; // PC = R_rs, val = R_rs as this is what we returned in Execute
        }
    } else if (d->type == I) { // all I format instructions that change PC, which is beq and bne
        if (d->op == 4) { //beq
            mips.pc += val; // PC = PC + 4 + addr, val = PC + 4 + addr
        } else if (d->op == 5) { // bne
            mips.pc += val; // PC = PC + 4 + addr
        }
    } else if (d->type == J) { // all J format instructions that change PC, which is j and jal
        if (d->op == 2) { // j
            mips.pc = d->regs.j.target; // PC = jump addr
        } else if (d->op == 3) { // jal
            mips.pc = d->regs.j.target; // PC = jump addr
        }
    }
}

/*
 * Perform memory load or store. Place the address of any updated memory 
 * in *changedMem, otherwise put -1 in *changedMem. Return any memory value 
 * that is read, otherwise return -1. 
 *
 * Remember that we're mapping MIPS addresses to indices in the mips.memory 
 * array. mips.memory[0] corresponds with address 0x00400000, mips.memory[1] 
 * with address 0x00400004, and so forth.
 *
 */
int Mem( DecodedInstr* d, int val, int *changedMem) {
    /* Your code goes here */
    // print an error message if the instruction reaches an invalid memory address (before 0x00400000 and after 0x00403fff)
    if ((val+mips.memory[0]) < 0x00401000 && (val+mips.memory[0]) > 0x00403fff) {
        printf("Memory Address Exception at 0x%x: address 0x%x ", val, val+mips.memory[0]); // print the current PC and the offending address
        exit(0); // then exit the program
    }

    // lw
    if (d->op == 35) {
        *changedMem = -1; // doesn't change memory, so *changedMem = -1
        return val; // return the read memory value
    }

    // sw
    if (d->op == 43) {
        // according to MIPS cheat sheet, M[R_rs+imm] = R_rt
        val += 0x00401000; // Start from the first value in the memory range (0x00401000), this is to help make sure we hit the right addresses
        *changedMem = val; // Store the new val into the memory
            
        return -1; // nothing was read from the memory, so return -1
    }

    else { // for every other case, since they don't affect the memory *changedMem = -1 and we return the read memory value
        *changedMem = -1;
        return val;
    }
  return 0;
}

/* 
 * Write back to register. If the instruction modified a register--
 * (including jal, which modifies $ra) --
 * put the index of the modified register in *changedReg,
 * otherwise put -1 in *changedReg.
 */
void RegWrite( DecodedInstr* d, int val, int *changedReg) {
    /* Your code goes here */
    if (d->type == R) { // all R format instructions that modify a register, which is all of them except jr
        if (d->regs.r.funct == 33) { // addu
            mips.registers[d->regs.r.rd] = val; // the register is changed to have val
            *changedReg = d->regs.r.rd; // write this back to the register for rd
        } else if (d->regs.r.funct == 36) { // and
            mips.registers[d->regs.r.rd] = val;
            *changedReg = d->regs.r.rd;
        } else if (d->regs.r.funct == 8) { // jr
            *changedReg = -1; // doesn't change register, so *changedReg = -1
        } else if (d->regs.r.funct == 0) { // sll
            mips.registers[d->regs.r.rd] = val;
            *changedReg = d->regs.r.rd;
        } else if (d->regs.r.funct == 42) { // slt
            mips.registers[d->regs.r.rd] = val;
            *changedReg = d->regs.r.rd;
        } else if (d->regs.r.funct == 2) { // srl
            mips.registers[d->regs.r.rd] = val;
            *changedReg = d->regs.r.rd;
        } else if (d->regs.r.funct == 35) { // subu
            mips.registers[d->regs.r.rd] = val;
            *changedReg = d->regs.r.rd;
        }
    } else if (d->type == I) { // all I format instructions that modify a register, which is all of them except bne, beq and sw
        if (d->op == 9) { // addiu
            mips.registers[d->regs.i.rt] = val; // this register is changed to have val
            *changedReg = d->regs.i.rt; // write this back to the register for rt
        } else if (d->op == 12) { // andi
            mips.registers[d->regs.i.rt] = val;
            *changedReg = d->regs.i.rt;
        } else if (d->op == 13) { // ori
            mips.registers[d->regs.i.rt] = val;
            *changedReg = d->regs.i.rt;
        } else if (d->op == 15) { // lui
            mips.registers[d->regs.i.rt] = val;
            *changedReg = d->regs.i.rt;
        } else if (d->op == 4) { // beq
            *changedReg = -1; // doesn't change register, so *changedReg = -1
        } else if (d->op == 5) { // bne
            *changedReg = -1;
        } else if (d->op == 23) { // lw
            mips.registers[d->regs.i.rt] = val;
            *changedReg = d->regs.i.rt;
        } else if (d->op == 43) { // sw
            *changedReg = -1;
        }
    } else if (d->type == J) { // all J format instructions that modify a register, which is jal
        if (d->op == 3) { // jal
            mips.registers[31] = val; // R[31] = PC + 8, from the MIPS cheat sheet
            *changedReg = 31; // register 31 is changed, so write that back to the register
        } else if (d->op == 2) { // j
            *changedReg = -1; // doesn't change register, so *changedReg = -1
        }
    }
}
