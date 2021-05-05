#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void instrOp(char instr[]) {
	if (strncmp(instr, "add", 3) == 0) {
		printf("Operation: %s", "add");
		printf("\n");
	}

	if (strncmp(instr, "addu", 4) == 0) {
		printf("Operation: %s", "addu");
		printf("\n");
	}

	if (strncmp(instr, "and", 3) == 0) {
		printf("Operation: %s", "and");
		printf("\n");
	}

	if (strncmp(instr, "jr", 2) == 0) {
		printf("Operation: %s", "jr");
		printf("\n");
	}

	if (strncmp(instr, "nor", 3) == 0) {
		printf("Operation: %s", "nor");
		printf("\n");
	}

	if (strncmp(instr, "or", 2) == 0) {
		printf("Operation: %s", "or");
		printf("\n");
	}

	if (strncmp(instr, "slt", 3) == 0) {
		printf("Operation: %s", "slt");
		printf("\n");
	}

	if (strncmp(instr, "sltu", 4) == 0) {
		printf("Operation: %s", "sltu");
		printf("\n");
	}

	if (strncmp(instr, "sll", 3) == 0) {
		printf("Operation: %s", "sll");	
		printf("\n");
	}

	if (strncmp(instr, "srl", 3) == 0) {
		printf("Operation: %s", "srl");
		printf("\n");
	}

	if (strncmp(instr, "sub", 3) == 0) {
		printf("Operation: %s", "sub");
		printf("\n");
	}

	if (strncmp(instr, "subu", 4) == 0) {
		printf("Operation: %s", "subu");
		printf("\n");
	}
}

// Used to determine the function for the instruction
int instrFunct(char instr[], int function, char machineCode[]) {
	// Compare the instruction string to the function we want to check for
	// strncmp means compare up to the first n characters of the string
	// if strncmp returns 0, instr and "add", "addu", etc. are equal
	if (strncmp(instr, "add", 3) == 0) {
		function = 32;
		// Append the function code (in binary) to the end of the string representing the machine code 
		strcat(machineCode, "100000");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "addu", 4) == 0) {
		function = 33;
		strcat(machineCode, "100001");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "and", 3) == 0) {
		function = 36;
		strcat(machineCode, "100100");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "jr", 2) == 0) {
		function = 8;
		strcat(machineCode, "001000");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "nor", 3) == 0) {
		function = 39;
		strcat(machineCode, "100111");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "or", 2) == 0) {
		function = 37;
		strcat(machineCode, "100101");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "slt", 3) == 0) {
		function = 42;
		strcat(machineCode, "101010");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "sltu", 4) == 0) {
		function = 43;
		strcat(machineCode, "101011");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr,"sll", 3) == 0) {
		function = 0;
		strcat(machineCode, "000000");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "srl", 3) == 0) {
		function = 2;
		strcat(machineCode, "000010");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "sub", 3) == 0) {
		function = 34;
		strcat(machineCode, "100010");
		printf("Funct: %d", function);
		printf("\n");
	}

	if (strncmp(instr, "subu", 4) == 0) {
		function = 35;
		strcat(machineCode, "100011");
		printf("Funct: %d", function);
		printf("\n");
	} 

	return function; // return the function code
}

int instrRd(char instr[], char machineCode[], char* fields[]) {
	int rd;

	if (strcmp(fields[0], "jr") == 0) {
		rd = 0;
		printf("Rd: %d", rd);
		printf("(R0)");
		printf("\n");
		strcat(machineCode, "00000");
	}

	if (strcmp(fields[1], "0") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 0;
		printf("Rd: %d", rd);
		printf("(R0)");
		printf("\n");
		strcat(machineCode, "00000");
	}

	if (strcmp(fields[1], "at") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 1;
		printf("Rd: %d", rd);
		printf("(R1)");
		printf("\n");
		strcat(machineCode, "00001");
	}

	if (strcmp(fields[1], "v0") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 2;
		printf("Rd: %d", rd);
		printf("(R2)");
		printf("\n");
		strcat(machineCode, "00010");
	}

	if (strcmp(fields[1], "v1") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 3;
		printf("Rd: %d", rd);
		printf("(R3)");
		printf("\n");
		strcat(machineCode, "00011");
	}

	if (strcmp(fields[1], "a0") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 4;
		printf("Rd: %d", rd);
		printf("(R4)");
		printf("\n");
		strcat(machineCode, "00100");
	}

	if (strcmp(fields[1], "a1") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 5;
		printf("Rd: %d", rd);
		printf("(R5)");
		printf("\n");
		strcat(machineCode, "00101");
	}

	if (strcmp(fields[1], "a2") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 6;
		printf("Rd: %d", rd);
		printf("(R6)");
		printf("\n");
		strcat(machineCode, "00110");
	}

	if (strcmp(fields[1], "a3") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 7;
		printf("Rd: %d", rd);
		printf("(R7)");
		printf("\n");
		strcat(machineCode, "00111");
	}

	if (strcmp(fields[1], "t0") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 8;
		printf("Rd: %d", rd);
		printf("(R8)");
		printf("\n");
		strcat(machineCode, "01000");
	}

	if (strcmp(fields[1], "t1") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 9;
		printf("Rd: %d", rd);
		printf("(R9)");
		printf("\n");
		strcat(machineCode, "01001");
	}

	if (strcmp(fields[1], "t2") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 10;
		printf("Rd: %d", rd);
		printf("(R10)");
		printf("\n");
		strcat(machineCode, "01010");
	}

	if (strcmp(fields[1], "t3") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 11;
		printf("Rd: %d", rd);
		printf("(R11)");
		printf("\n");
		strcat(machineCode, "01011");
	}

	if (strcmp(fields[1], "t4") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 12;
		printf("Rd: %d", rd);
		printf("(R12)");
		printf("\n");
		strcat(machineCode, "01100");
	}

	if (strcmp(fields[1], "t5") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 13;
		printf("Rd: %d", rd);
		printf("(R13)");
		printf("\n");
		strcat(machineCode, "01101");
	}

	if (strcmp(fields[1], "t6") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 14;
		printf("Rd: %d", rd);
		printf("(R14)");
		printf("\n");
		strcat(machineCode, "01110");
	}

	if (strcmp(fields[1], "t7") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 15;
		printf("Rd: %d", rd);
		printf("(R15)");
		printf("\n");
		strcat(machineCode, "01111");
	}

	if (strcmp(fields[1], "s0") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 16;
		printf("Rd: %d", rd);
		printf("(R16)");
		printf("\n");
		strcat(machineCode, "10000");
	}

	if (strcmp(fields[1], "s1") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 17;
		printf("Rd: %d", rd);
		printf("(R17)");
		printf("\n");
		strcat(machineCode, "10001");
	}

	if (strcmp(fields[1], "s2") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 18;
		printf("Rd: %d", rd);
		printf("(R18)");
		printf("\n");
		strcat(machineCode, "10010");
	}

	if (strcmp(fields[1], "s3") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 19;
		printf("Rd: %d", rd);
		printf("(R19)");
		printf("\n");
		strcat(machineCode, "10011");
	}

	if (strcmp(fields[1], "s4") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 20;
		printf("Rd: %d", rd);
		printf("(R20)");
		printf("\n");
		strcat(machineCode, "10100");
	}

	if (strcmp(fields[1], "s5") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 21;
		printf("Rd: %d", rd);
		printf("(R21)");
		printf("\n");
		strcat(machineCode, "10101");
	}

	if (strcmp(fields[1], "s6") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 22;
		printf("Rd: %d", rd);
		printf("(R22)");
		printf("\n");
		strcat(machineCode, "10110");
	}

	if (strcmp(fields[1], "s7") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 23;
		printf("Rd: %d", rd);
		printf("(R23)");
		printf("\n");
		strcat(machineCode, "10111");
	}

	if (strcmp(fields[1], "t8") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 24;
		printf("Rd: %d", rd);
		printf("(R24)");
		printf("\n");
		strcat(machineCode, "11000");
	}

	if (strcmp(fields[1], "t9") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 25;
		printf("Rd: %d", rd);
		printf("(R25)");
		printf("\n");
		strcat(machineCode, "11001");
	}

	if (strcmp(fields[1], "k0") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 26;
		printf("Rd: %d", rd);
		printf("(R26)");
		printf("\n");
		strcat(machineCode, "11010");
	}

	if (strcmp(fields[1], "k1") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 27;
		printf("Rd: %d", rd);
		printf("(R27)");
		printf("\n");
		strcat(machineCode, "11011");
	}

	if (strcmp(fields[1], "gp") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 28;
		printf("Rd: %d", rd);
		printf("(R28)");
		printf("\n");
		strcat(machineCode, "11100");
	}

	if (strcmp(fields[1], "sp") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 29;
		printf("Rd: %d", rd);
		printf("(R29)");
		printf("\n");
		strcat(machineCode, "11101");
	}

	if (strcmp(fields[1], "fp") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 30;
		printf("Rd: %d", rd);
		printf("(R30)");
		printf("\n");
		strcat(machineCode, "11110");
	}

	if (strcmp(fields[1], "ra") == 0 && strcmp(fields[0], "jr") != 0) {
		rd = 31;
		printf("Rd: %d", rd);
		printf("(R31)");
		printf("\n");
		strcat(machineCode, "11111");
	}

	return rd;
}

int instrRt(char instr[], char machineCode[], char* fields[]) {
	int rt;

	// if the instruction calls for srl or sll, then the token usually used for rs is now used for rt (as the machine code for srl/sll has rs as 00 0000)
	if (strcmp(fields[0], "srl") == 0 || strcmp(fields[0], "sll") == 0) {
		if (strcmp(fields[2], "0") == 0) {
			rt = 0;
			printf("Rt: %d", rt);
			printf("(R0)");
			printf("\n");
			strcat(machineCode, "00000");
		}

		if (strcmp(fields[2], "at") == 0) {
			rt = 1;
			printf("Rt: %d", rt);
			printf("(R1)");
			printf("\n");
			strcat(machineCode, "00001");
		}

		if (strcmp(fields[2], "v0") == 0) {
			rt = 2;
			printf("Rt: %d", rt);
			printf("(R2)");
			printf("\n");
			strcat(machineCode, "00010");
		}

		if (strcmp(fields[2], "v1") == 0) {
			rt = 3;
			printf("Rt: %d", rt);
			printf("(R3)");
			printf("\n");
			strcat(machineCode, "00011");
		}

		if (strcmp(fields[2], "a0") == 0) {
			rt = 4;
			printf("Rt: %d", rt);
			printf("(R4)");
			printf("\n");
			strcat(machineCode, "00100");
		}

		if (strcmp(fields[2], "a1") == 0) {
			rt = 5;
			printf("Rt: %d", rt);
			printf("(R5)");
			printf("\n");
			strcat(machineCode, "00101");
		}

		if (strcmp(fields[2], "a2") == 0) {
			rt = 6;
			printf("Rt: %d", rt);
			printf("(R6)");
			printf("\n");
			strcat(machineCode, "00110");
		}

		if (strcmp(fields[2], "a3") == 0) {
			rt = 7;
			printf("Rt: %d", rt);
			printf("(R7)");
			printf("\n");
			strcat(machineCode, "00111");
		}

		if (strcmp(fields[2], "t0") == 0) {
			rt = 8;
			printf("Rt: %d", rt);
			printf("(R8)");
			printf("\n");
			strcat(machineCode, "01000");
		}

		if (strcmp(fields[2], "t1") == 0) {
			rt = 9;
			printf("Rt: %d", rt);
			printf("(R9)");
			printf("\n");
			strcat(machineCode, "01001");
		}

		if (strcmp(fields[2], "t2") == 0) {
			rt = 10;
			printf("Rt: %d", rt);
			printf("(R10)");
			printf("\n");
			strcat(machineCode, "01010");
		}

		if (strcmp(fields[2], "t3") == 0) {
			rt = 11;
			printf("Rt: %d", rt);
			printf("(R11)");
			printf("\n");
			strcat(machineCode, "01011");
		}

		if (strcmp(fields[2], "t4") == 0) {
			rt = 12;
			printf("Rt: %d", rt);
			printf("(R12)");
			printf("\n");
			strcat(machineCode, "01100");
		}

		if (strcmp(fields[2], "t5") == 0) {
			rt = 13;
			printf("Rt: %d", rt);
			printf("(R13)");
			printf("\n");
			strcat(machineCode, "01101");
		}

		if (strcmp(fields[2], "t6") == 0) {
			rt = 14;
			printf("Rt: %d", rt);
			printf("(R14)");
			printf("\n");
			strcat(machineCode, "01110");
		}

		if (strcmp(fields[2], "t7") == 0) {
			rt = 15;
			printf("Rt: %d", rt);
			printf("(R15)");
			printf("\n");
			strcat(machineCode, "01111");
		}

		if (strcmp(fields[2], "s0") == 0) {
			rt = 16;
			printf("Rt: %d", rt);
			printf("(R16)");
			printf("\n");
			strcat(machineCode, "10000");
		}

		if (strcmp(fields[2], "s1") == 0) {
			rt = 17;
			printf("Rt: %d", rt);
			printf("(R17)");
			printf("\n");
			strcat(machineCode, "10001");
		}

		if (strcmp(fields[2], "s2") == 0) {
			rt = 18;
			printf("Rt: %d", rt);
			printf("(R18)");
			printf("\n");
			strcat(machineCode, "10010");
		}

		if (strcmp(fields[2], "s3") == 0) {
			rt = 19;
			printf("Rt: %d", rt);
			printf("(R19)");
			printf("\n");
			strcat(machineCode, "10011");
		}

		if (strcmp(fields[2], "s4") == 0) {
			rt = 20;
			printf("Rt: %d", rt);
			printf("(R20)");
			printf("\n");
			strcat(machineCode, "10100");
		}

		if (strcmp(fields[2], "s5") == 0) {
			rt = 21;
			printf("Rt: %d", rt);
			printf("(R21)");
			printf("\n");
			strcat(machineCode, "10101");
		}

		if (strcmp(fields[2], "s6") == 0) {
			rt = 22;
			printf("Rt: %d", rt);
			printf("(R22)");
			printf("\n");
			strcat(machineCode, "10110");
		}

		if (strcmp(fields[2], "s7") == 0) {
			rt = 23;
			printf("Rt: %d", rt);
			printf("(R23)");
			printf("\n");
			strcat(machineCode, "10111");
		}

		if (strcmp(fields[2], "t8") == 0) {
			rt = 24;
			printf("Rt: %d", rt);
			printf("(R24)");
			printf("\n");
			strcat(machineCode, "11000");
		}

		if (strcmp(fields[2], "t9") == 0) {
			rt = 25;
			printf("Rt: %d", rt);
			printf("(R25)");
			printf("\n");
			strcat(machineCode, "11001");
		}

		if (strcmp(fields[2], "k0") == 0) {
			rt = 26;
			printf("Rt: %d", rt);
			printf("(R26)");
			printf("\n");
			strcat(machineCode, "11010");
		}

		if (strcmp(fields[2], "k1") == 0) {
			rt = 27;
			printf("Rt: %d", rt);
			printf("(R27)");
			printf("\n");
			strcat(machineCode, "11011");
		}

		if (strcmp(fields[2], "gp") == 0) {
			rt = 28;
			printf("Rt: %d", rt);
			printf("(R28)");
			printf("\n");
			strcat(machineCode, "11100");
		}

		if (strcmp(fields[2], "sp") == 0) {
			rt = 29;
			printf("Rt: %d", rt);
			printf("(R29)");
			printf("\n");
			strcat(machineCode, "11101");
		}

		if (strcmp(fields[2], "fp") == 0) {
			rt = 30;
			printf("Rt: %d", rt);
			printf("(R30)");
			printf("\n");
			strcat(machineCode, "11110");
		}

		if (strcmp(fields[2], "ra") == 0) {
			rt = 31;
			printf("Rt: %d", rt);
			printf("(R31)");
			printf("\n");
			strcat(machineCode, "11111");
		}
	}

	// for every instruction besides srl/sll, operate as normal
	if (strcmp(fields[0], "srl") != 0 || strcmp(fields[0], "sll") != 0) {
		if (strcmp(fields[3], "0") == 0) {
			rt = 0;
			printf("Rt: %d", rt);
			printf("(R0)");
			printf("\n");
			strcat(machineCode, "00000");
		}

		if (strcmp(fields[3], "at") == 0) {
			rt = 1;
			printf("Rt: %d", rt);
			printf("(R1)");
			printf("\n");
			strcat(machineCode, "00001");
		}

		if (strcmp(fields[3], "v0") == 0) {
			rt = 2;
			printf("Rt: %d", rt);
			printf("(R2)");
			printf("\n");
			strcat(machineCode, "00010");
		}

		if (strcmp(fields[3], "v1") == 0) {
			rt = 3;
			printf("Rt: %d", rt);
			printf("(R3)");
			printf("\n");
			strcat(machineCode, "00011");
		}

		if (strcmp(fields[3], "a0") == 0) {
			rt = 4;
			printf("Rt: %d", rt);
			printf("(R4)");
			printf("\n");
			strcat(machineCode, "00100");
		}

		if (strcmp(fields[3], "a1") == 0) {
			rt = 5;
			printf("Rt: %d", rt);
			printf("(R5)");
			printf("\n");
			strcat(machineCode, "00101");
		}

		if (strcmp(fields[3], "a2") == 0) {
			rt = 6;
			printf("Rt: %d", rt);
			printf("(R6)");
			printf("\n");
			strcat(machineCode, "00110");
		}

		if (strcmp(fields[3], "a3") == 0) {
			rt = 7;
			printf("Rt: %d", rt);
			printf("(R7)");
			printf("\n");
			strcat(machineCode, "00111");
		}

		if (strcmp(fields[3], "t0") == 0) {
			rt = 8;
			printf("Rt: %d", rt);
			printf("(R8)");
			printf("\n");
			strcat(machineCode, "01000");
		}

		if (strcmp(fields[3], "t1") == 0) {
			rt = 9;
			printf("Rt: %d", rt);
			printf("(R9)");
			printf("\n");
			strcat(machineCode, "01001");
		}

		if (strcmp(fields[3], "t2") == 0) {
			rt = 10;
			printf("Rt: %d", rt);
			printf("(R10)");
			printf("\n");
			strcat(machineCode, "01010");
		}

		if (strcmp(fields[3], "t3") == 0) {
			rt = 11;
			printf("Rt: %d", rt);
			printf("(R11)");
			printf("\n");
			strcat(machineCode, "01011");
		}

		if (strcmp(fields[3], "t4") == 0) {
			rt = 12;
			printf("Rt: %d", rt);
			printf("(R12)");
			printf("\n");
			strcat(machineCode, "01100");
		}

		if (strcmp(fields[3], "t5") == 0) {
			rt = 13;
			printf("Rt: %d", rt);
			printf("(R13)");
			printf("\n");
			strcat(machineCode, "01101");
		}

		if (strcmp(fields[3], "t6") == 0) {
			rt = 14;
			printf("Rt: %d", rt);
			printf("(R14)");
			printf("\n");
			strcat(machineCode, "01110");
		}

		if (strcmp(fields[3], "t7") == 0) {
			rt = 15;
			printf("Rt: %d", rt);
			printf("(R15)");
			printf("\n");
			strcat(machineCode, "01111");
		}

		if (strcmp(fields[3], "s0") == 0) {
			rt = 16;
			printf("Rt: %d", rt);
			printf("(R16)");
			printf("\n");
			strcat(machineCode, "10000");
		}

		if (strcmp(fields[3], "s1") == 0) {
			rt = 17;
			printf("Rt: %d", rt);
			printf("(R17)");
			printf("\n");
			strcat(machineCode, "10001");
		}

		if (strcmp(fields[3], "s2") == 0) {
			rt = 18;
			printf("Rt: %d", rt);
			printf("(R18)");
			printf("\n");
			strcat(machineCode, "10010");
		}

		if (strcmp(fields[3], "s3") == 0) {
			rt = 19;
			printf("Rt: %d", rt);
			printf("(R19)");
			printf("\n");
			strcat(machineCode, "10011");
		}

		if (strcmp(fields[3], "s4") == 0) {
			rt = 20;
			printf("Rt: %d", rt);
			printf("(R20)");
			printf("\n");
			strcat(machineCode, "10100");
		}

		if (strcmp(fields[3], "s5") == 0) {
			rt = 21;
			printf("Rt: %d", rt);
			printf("(R21)");
			printf("\n");
			strcat(machineCode, "10101");
		}

		if (strcmp(fields[3], "s6") == 0) {
			rt = 22;
			printf("Rt: %d", rt);
			printf("(R22)");
			printf("\n");
			strcat(machineCode, "10110");
		}

		if (strcmp(fields[3], "s7") == 0) {
			rt = 23;
			printf("Rt: %d", rt);
			printf("(R23)");
			printf("\n");
			strcat(machineCode, "10111");
		}

		if (strcmp(fields[3], "t8") == 0) {
			rt = 24;
			printf("Rt: %d", rt);
			printf("(R24)");
			printf("\n");
			strcat(machineCode, "11000");
		}

		if (strcmp(fields[3], "t9") == 0) {
			rt = 25;
			printf("Rt: %d", rt);
			printf("(R25)");
			printf("\n");
			strcat(machineCode, "11001");
		}

		if (strcmp(fields[3], "k0") == 0) {
			rt = 26;
			printf("Rt: %d", rt);
			printf("(R26)");
			printf("\n");
			strcat(machineCode, "11010");
		}

		if (strcmp(fields[3], "k1") == 0) {
			rt = 27;
			printf("Rt: %d", rt);
			printf("(R27)");
			printf("\n");
			strcat(machineCode, "11011");
		}

		if (strcmp(fields[3], "gp") == 0) {
			rt = 28;
			printf("Rt: %d", rt);
			printf("(R28)");
			printf("\n");
			strcat(machineCode, "11100");
		}

		if (strcmp(fields[3], "sp") == 0) {
			rt = 29;
			printf("Rt: %d", rt);
			printf("(R29)");
			printf("\n");
			strcat(machineCode, "11101");
		}

		if (strcmp(fields[3], "fp") == 0) {
			rt = 30;
			printf("Rt: %d", rt);
			printf("(R30)");
			printf("\n");
			strcat(machineCode, "11110");
		}

		if (strcmp(fields[3], "ra") == 0) {
			rt = 31;
			printf("Rt: %d", rt);
			printf("(R31)");
			printf("\n");
			strcat(machineCode, "11111");
		}
	}

	instrRd(instr, machineCode, fields);

	return rt;
}

int instRs(char instr[], char machineCode[], char* fields[]) {
	int rs;

	// if the instruction calls for srl or sll, then set rs to 00 0000
	if (strcmp(fields[0], "srl") == 0 || strcmp(fields[0], "sll") == 0) {
		rs = 0;
		printf("Rs: %d", rs);
		printf("(R0)");
		printf("\n");
		strcat(machineCode, "00000");
	}

	// for every instruction besides srl/sll, operate as normal
	if (strcmp(fields[0], "srl") != 0 || strcmp(fields[0], "sll") != 0) {
		if (strcmp(fields[0], "jr") == 0) { // since jr only uses one register, we want rs to be the register in the instruction. copy whats in fields[1] (rd) into fields[2] (rs)
			fields[2] = fields[1];
		}

		if (strcmp(fields[2], "0") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 0;
			printf("Rs: %d", rs);
			printf("(R0)");
			printf("\n");
			strcat(machineCode, "00000");
		}

		if (strcmp(fields[2], "at") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 1;
			printf("Rs: %d", rs);
			printf("(R1)");
			printf("\n");
			strcat(machineCode, "00001");
		}

		if (strcmp(fields[2], "v0") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 2;
			printf("Rs: %d", rs);
			printf("(R2)");
			printf("\n");
			strcat(machineCode, "00010");
		}

		if (strcmp(fields[2], "v1") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 3;
			printf("Rs: %d", rs);
			printf("(R3)");
			printf("\n");
			strcat(machineCode, "00011");
		}

		if (strcmp(fields[2], "a0") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 4;
			printf("Rs: %d", rs);
			printf("(R4)");
			printf("\n");
			strcat(machineCode, "00100");
		}

		if (strcmp(fields[2], "a1") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 5;
			printf("Rs: %d", rs);
			printf("(R5)");
			printf("\n");
			strcat(machineCode, "00101");
		}

		if (strcmp(fields[2], "a2") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 6;
			printf("Rs: %d", rs);
			printf("(R6)");
			printf("\n");
			strcat(machineCode, "00110");
		}

		if (strcmp(fields[2], "a3") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 7;
			printf("Rs: %d", rs);
			printf("(R7)");
			printf("\n");
			strcat(machineCode, "00111");
		}

		if (strcmp(fields[2], "t0") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 8;
			printf("Rs: %d", rs);
			printf("(R8)");
			printf("\n");
			strcat(machineCode, "01000");
		}

		if (strcmp(fields[2], "t1") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 9;
			printf("Rs: %d", rs);
			printf("(R9)");
			printf("\n");
			strcat(machineCode, "01001");
		}

		if (strcmp(fields[2], "t2") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 10;
			printf("Rs: %d", rs);
			printf("(R10)");
			printf("\n");
			strcat(machineCode, "01010");
		}

		if (strcmp(fields[2], "t3") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 11;
			printf("Rs: %d", rs);
			printf("(R11)");
			printf("\n");
			strcat(machineCode, "01011");
		}

		if (strcmp(fields[2], "t4") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 12;
			printf("Rs: %d", rs);
			printf("(R12)");
			printf("\n");
			strcat(machineCode, "01100");
		}

		if (strcmp(fields[2], "t5") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 13;
			printf("Rs: %d", rs);
			printf("(R13)");
			printf("\n");
			strcat(machineCode, "01101");
		}

		if (strcmp(fields[2], "t6") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 14;
			printf("Rs: %d", rs);
			printf("(R14)");
			printf("\n");
			strcat(machineCode, "01110");
		}

		if (strcmp(fields[2], "t7") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 15;
			printf("Rs: %d", rs);
			printf("(R15)");
			printf("\n");
			strcat(machineCode, "01111");
		}

		if (strcmp(fields[2], "s0") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 16;
			printf("Rs: %d", rs);
			printf("(R16)");
			printf("\n");
			strcat(machineCode, "10000");
		}

		if (strcmp(fields[2], "s1") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 17;
			printf("Rs: %d", rs);
			printf("(R17)");
			printf("\n");
			strcat(machineCode, "10001");
		}

		if (strcmp(fields[2], "s2") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 18;
			printf("Rs: %d", rs);
			printf("(R18)");
			printf("\n");
			strcat(machineCode, "10010");
		}

		if (strcmp(fields[2], "s3") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 19;
			printf("Rs: %d", rs);
			printf("(R19)");
			printf("\n");
			strcat(machineCode, "10011");
		}

		if (strcmp(fields[2], "s4") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 20;
			printf("Rs: %d", rs);
			printf("(R20)");
			printf("\n");
			strcat(machineCode, "10100");
		}

		if (strcmp(fields[2], "s5") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 21;
			printf("Rs: %d", rs);
			printf("(R21)");
			printf("\n");
			strcat(machineCode, "10101");
		}

		if (strcmp(fields[2], "s6") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 22;
			printf("Rs: %d", rs);
			printf("(R22)");
			printf("\n");
			strcat(machineCode, "10110");
		}

		if (strcmp(fields[2], "s7") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 23;
			printf("Rs: %d", rs);
			printf("(R23)");
			printf("\n");
			strcat(machineCode, "10111");
		}

		if (strcmp(fields[2], "t8") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 24;
			printf("Rs: %d", rs);
			printf("(R24)");
			printf("\n");
			strcat(machineCode, "11000");
		}

		if (strcmp(fields[2], "t9") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 25;
			printf("Rs: %d", rs);
			printf("(R25)");
			printf("\n");
			strcat(machineCode, "11001");
		}

		if (strcmp(fields[2], "k0") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 26;
			printf("Rs: %d", rs);
			printf("(R26)");
			printf("\n");
			strcat(machineCode, "11010");
		}

		if (strcmp(fields[2], "k1") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 27;
			printf("Rs: %d", rs);
			printf("(R27)");
			printf("\n");
			strcat(machineCode, "11011");
		}

		if (strcmp(fields[2], "gp") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 28;
			printf("Rs: %d", rs);
			printf("(R28)");
			printf("\n");
			strcat(machineCode, "11100");
		}

		if (strcmp(fields[2], "sp") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 29;
			printf("Rs: %d", rs);
			printf("(R29)");
			printf("\n");
			strcat(machineCode, "11101");
		}

		if (strcmp(fields[2], "fp") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 30;
			printf("Rs: %d", rs);
			printf("(R30)");
			printf("\n");
			strcat(machineCode, "11110");
		}

		if (strcmp(fields[2], "ra") == 0 && strcmp(fields[0], "srl") != 0 && strcmp(fields[0], "sll") != 0) {
			rs = 31;
			printf("Rs: %d", rs);
			printf("(R31)");
			printf("\n");
			strcat(machineCode, "11111");
		}
	}

	instrRt(instr, machineCode, fields);

	return rs;
}

// Convert the integer into its binary representation
void intToBin(int shamt, char machineCode[]) {
	char binary[5] = {'0', '0', '0', '0', '0'};
	int b[5];
	int i;

	for (i = 0; shamt > 0; i++) {
		b[i] = shamt % 2;
		shamt = shamt/2;
	}

	for (i = i - 1; i >= 0; i--) { // this is the binary for our shift amount, except in reverse
		binary[i] = b[i] + '0';
	}

	int k;
	int j = strlen(binary) - 1;

	while (k < j) { // reverse the string
		char temp = binary[k];
		binary[k] = binary[j];
		binary[j] = temp;
		k++;
		j--;
	}
	
	// Append the now converted shift amount to the machine code
	strcat(machineCode, binary);
	
}

void shiftAmt(char instr[], char machineCode[], char* fields[]) {
	int shamt;

	// instructions only have a shift amount if they are sll or srl
	if (strncmp(instr, "sll", 3) == 0) {
		sscanf(fields[3], "%d", &shamt); // convert the char from fields[3] into an int
		printf("Shamt: %d", shamt);
		printf("\n");
		intToBin(shamt, machineCode);
	} else if (strncmp(instr, "srl", 3) == 0) {
		sscanf(fields[3], "%d", &shamt); // convert the char from fields[3] into an int
		printf("Shamt: %d", shamt);
		printf("\n");
		intToBin(shamt, machineCode); // convert shamt into binary
	} else { // if the instruction is neither sll or srl, then that means shift amount = 0, so append to this to the machine code
		strcat(machineCode, "00000");
		shamt = 0;
		printf("Shamt: %d", shamt);
		printf("\n");
	}
}


int main() {
	printf("Enter an R-format instruction: ");
	char instr[30]; // holds the string representing the instruction 

	// Used to break the instruction string into a series of tokens ignoring the delimiter, which is a space and a comma
	char delim[ ] = " ,"; 
	char* token;

	// Read in the user's input to write the string
	fgets(instr, 30, stdin);

	// index
	int i = 0;

	// Get the first token
	token = strtok(instr, delim);

	// stores the fields in the broken apart string for ease of use
	char* fields[4];

	// walk through the other tokens, save each token into the 'fields' char array
	while (token != NULL) {
		fields[i++] = token;
		token = strtok(NULL, delim);
	}

	// String used to hold the machine code
	char machineCode[32];

	// function code
	int function = 0;

	// the opcode for every R format instruction is 00 0000
	for (int i = 0; i < 6; i++) {
		machineCode[i] = '0';
	}

	// Print out the operation
	instrOp(instr);

	// Call the function that determines the registers used by the instruction (rd, rt, and rs)
	instRs(instr, machineCode, fields);

	// Call the function that determines the shift amount, usually 0
	shiftAmt(instr, machineCode, fields);

	// Call the function that gives us the function code for the instruction
	instrFunct(instr, function, machineCode);

	// Print out the final machine code
	printf("Machine code: %s", machineCode);
	printf("\n");

	return 0;
}
