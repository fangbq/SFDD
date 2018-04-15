/* #################### Head file of read verilog file #################### */

#ifndef _READ_VERILOG_H_
#define _READ_VERILOG_H_

#include <stdint.h>
#include <stdio.h>
#include "define.h"

/* #################### Some Macro #################### */

// (1, 0) for input variable; (0, 1) for output variable; (0, 0) for wire

// max character of variable name
#define _MAX_CHAR_	256

#define VARTYPE(lv)	((lv) & 0x60000000)				// 0110 0000 ....
#define _INPUT_		0x40000000					// 0100 0000 ....
#define _OUTPUT_	0x20000000					// 0010 0000 ....
#define _WIRE_		0x00000000					// 0000 0000 ....
#define GATETYPE(lv)	((lv) & 0x00000007)

#define _INVALID_GATE_	0x00
#define _XOR_		0x01
#define	_AND_		0x02

#define _OR_		0x03
#define _INV_		0x04
#define _BUF_		0x05


/* #################### DATA STRUCTURE #################### */

typedef struct logicVar logicVar;
typedef struct logicNet logicNet;

/* Logic Net(copy from BBDD package, with some modification), which contains: 
 * NIn:		Number of input variables.
 * Nout:	Number of output variables.
 * Nwire:	Number of wire variable. 
 * input:	Array of input variable.
 * output:
 * wire:
 * */

struct logicNet {
	unsigned int Nin;
	unsigned int Nout;
	unsigned int Nwire;

	logicVar *input;
	logicVar *output;
	logicVar *wire;
};

/* Logic variable(copy from BBDD package, with some modification), which contains:
 * name:	True variable name, copy from verilog file(input, output, wire).
 * varIndex:	Fake variable name, for standardize reason, input variable denoted by xi, 
 * 		Output variable denoted by yi, wire denoted by ni, so, only need to store index.
 * 
 * info:	Variable type(leftmost 2nd, 3rd bit) + gate type(rightmost 3bit).
 *		Gate type, include AND, OR, XOR, INV, BUF.
 *
 * func:	Formula represented by logic variable.
 *
 * A:		Input source of logic variable(input variable has no source).
 * B:		Same as A. For INV and BUF gate, B remain NULL.
 *
 * */

using addr_t = long long int;

struct logicVar {
	char *name;
	unsigned int varIndex;			// index is useless for wire and output
	uint32_t info;
	addr_t func;

	logicVar *A;
	logicVar *B;
};

/* #################### FUNCTIONS #################### */

// Read verilog file
logicNet *readVerilog(const char *filename);

// Free verilog input, output, wire
void freeLogicNet(logicNet *net);

/* ---------- Following functions are COPY from BBDD package, with some Modification ---------- */
// Read Verilog input-segment
logicVar *readInput(FILE *filev, unsigned int *Nin);

// Read Verilog output-segment
logicVar *readOutput(FILE *filev, unsigned int *Nout);

// Read Verilog wire-segment
logicVar *readWire(FILE *filev, unsigned int *Nwire);

// Read Verilog gate input source
logicVar *readOperandIn(FILE *filev, logicVar *input, unsigned int Nin, logicVar *wire, unsigned int Nwire, logicVar *output, unsigned int Nout);
// Read Verilog gate output source
logicVar *readOperandOut(FILE *filev, logicVar *output, unsigned int Nout, logicVar *wire, unsigned int Nwire);
// Read Verilog gate
void readFunctions(FILE *filev, logicVar *input, unsigned int Nin, logicVar *output, unsigned int Nout, logicVar *wire, unsigned int Nwire);
/* -------------------- END -------------------- */


#endif
