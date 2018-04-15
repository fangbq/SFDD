/* #################### C file of read verilog file #################### */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "readVerilog.h"

/* FUNCTION-DESCRIPTION:	Read verilog file.
 * PRE-CONDITION:		Valid filename, valid verilog file.
 * POST-CONDITION:		If success:
 *						Return a Logic Net.
 *				If failed:
 *						Return NULL.
 */

logicNet *readVerilog(const char *filename) {
	FILE *filev;

	if((filev = fopen(filename, "r" )) == NULL){
		printf("Read Verilog error#1: open file failed\n");	
		return NULL;
	}
	
	logicNet *net = (logicNet *)malloc(sizeof(logicNet));
	if(net == NULL){
		printf("Read Verilog error#2: request memory for logic net failed\n");
		fclose(filev);
		return NULL;
	}

	char cha;
	fscanf(filev, "%c", &cha);
	while(cha != ';')
		fscanf(filev, "%c", &cha);

	net->input = readInput(filev, &(net->Nin));
	if(net->input == NULL){
		printf("Read Verilog error#3: no memory for input variable, or invalid number of input variable\n");
		free(net);
		fclose(filev);
		return NULL;
	}

	net->output = readOutput(filev, &(net->Nout));
	if(net->output == NULL){
		printf("Read Verilog error#4: no memory for output variable, or invalid number of output variable\n");
		free(net->input);		free(net);
		fclose(filev);
		return NULL;
	}
	net->wire = readWire(filev, &(net->Nwire));
	if(net->wire == NULL){
		printf("Read Verilog error#5: no memory for wire\n");
		// roll back
		free(net->output);	free(net->input);	free(net);
		fclose(filev);
		return NULL;
	}

	// connect input, output, wire to construct logic net
	readFunctions(filev, net->input, net->Nin, net->output, net->Nout, net->wire, net->Nwire);
	
	fclose(filev);
	// printf("close file %s\n", filename);

	return net;
}// END of function


/* FUNCTION DESCRIPTION: 	Free memory logic net and its components.
 * PRE-CONDITION:		Pointer not NULL.
 * POST-CONDITION:		
 * */

void freeLogicNet(logicNet *net){
	free(net->input);
	// printf("free verilog input-array\n");
	free(net->output);
	// printf("free verilog output-array\n");
	free(net->wire);
	// printf("free verilog wire-array\n");
	free(net);
	// printf("free logic net\n");
	return;
}// END of function


/* FUNCTION DESCRIPTION:	Read input segment of Verilog file 
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 						Return an input-array contains all input variables.
 * 						
 * */
logicVar *readInput(FILE *filev, unsigned int *Nin) {
	char varName[_MAX_CHAR_];
	unsigned int inputIdx = 0;

	logicVar *input = (logicVar *)malloc(sizeof(logicVar));
	if(input == NULL){
		printf("Read Verilog readInput error#1: request for input-array failed\n");
		return NULL;
	}
       	fscanf(filev, "%s", varName);
	while(strcmp("input", varName) != 0)
        	fscanf(filev, "%s", varName);

	fscanf(filev, "%s", varName);
	while(varName[strlen(varName) - 1] != ';') {
		varName[strlen(varName) - 1] = '\0';
		input[inputIdx].name = (char *)(malloc(sizeof(char) * (strlen(varName) - 1)));
		strcpy(input[inputIdx].name, varName);

		input[inputIdx].varIndex = inputIdx + 1;
		input[inputIdx].info = _INPUT_;
		input[inputIdx].func = -1;
		input[inputIdx].A = NULL;
		input[inputIdx].B = NULL;
		inputIdx ++;
		input = (logicVar *)(realloc(input, sizeof(logicVar) * (inputIdx + 1)));

		fscanf(filev, "%s", varName);
	}
	varName[strlen(varName) - 1] = '\0';
	input[inputIdx].name = (char *)(malloc(sizeof(char) * (strlen(varName) - 1)));
	strcpy(input[inputIdx].name, varName);
	
	input[inputIdx].varIndex = inputIdx + 1;
	input[inputIdx].info = _INPUT_;
	input[inputIdx].func = -1;
	input[inputIdx].A = NULL;
	input[inputIdx].B = NULL;

	(*Nin) = inputIdx + 1;

	return input;
}// END of function


/* FUNCTION DESCRIPTION:	Read output segment of Verilog file
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 						Return an output-array contains all output variables.
 * */
logicVar *readOutput(FILE *filev, unsigned int *Nout) {
	char varName[_MAX_CHAR_];
	unsigned int outputIdx = 0;

	logicVar *output = (logicVar *)malloc(sizeof(logicVar));
	if(output == NULL){
		printf("Read Verilog readOutput error#1: request for output-array failed\n");
		return NULL;
	}
       	fscanf(filev, "%s", varName);
	while(strcmp("output", varName) != 0)
        	fscanf(filev, "%s", varName);

	fscanf(filev, "%s", varName);
	while(varName[strlen(varName) - 1] != ';') {
		varName[strlen(varName) - 1] = '\0';
		output[outputIdx].name = (char *)(malloc(sizeof(char) * (strlen(varName) - 1)));
		strcpy(output[outputIdx].name, varName);

		output[outputIdx].varIndex = outputIdx;
		output[outputIdx].info = _OUTPUT_;
		output[outputIdx].func = -1;
		output[outputIdx].A = NULL;
		output[outputIdx].B = NULL;
		outputIdx ++;
		output = (logicVar *)(realloc(output, sizeof(logicVar) * (outputIdx + 1)));

		fscanf(filev, "%s", varName);
	}
	varName[strlen(varName) - 1] = '\0';
	output[outputIdx].name = (char *)(malloc(sizeof(char) * (strlen(varName) - 1)));
	strcpy(output[outputIdx].name, varName);

	output[outputIdx].varIndex = outputIdx;
	output[outputIdx].info = _OUTPUT_;
	output[outputIdx].func = -1;
	output[outputIdx].A = NULL;
	output[outputIdx].B = NULL;

	(*Nout) = outputIdx + 1;

	return output;
}// END of function


/* FUNCTION DESCRIPTION:	Read wire segment of Verilog file
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 						Return a wire-array contains all wire variables.
 * */
logicVar *readWire(FILE *filev, unsigned int *Nwire) {
	char wireName[_MAX_CHAR_];
	unsigned int wireIdx = 0;

	logicVar *wire = (logicVar *)(malloc(sizeof(logicVar)));
	if(wire == NULL){
		printf("Read Verilog readWire error#1: request for wire-array failed\n");
		return NULL;
	}	
	fscanf(filev, "%s", wireName);
	while(strcmp("wire", wireName) !=0 )
        	fscanf(filev, "%s", wireName);

	fscanf(filev, "%s", wireName);
	while(wireName[strlen(wireName) - 1] != ';') {

		wireName[strlen(wireName) - 1] = '\0';
		wire[wireIdx].name = (char *)(malloc(sizeof(char) * (strlen(wireName) - 1)));
		strcpy(wire[wireIdx].name, wireName);

		wire[wireIdx].varIndex = wireIdx;
		wire[wireIdx].info = 0;
		wire[wireIdx].func = -1;
		wire[wireIdx].A = NULL;
		wire[wireIdx].B = NULL;
		wireIdx ++;
		wire = (logicVar *)(realloc(wire, sizeof(logicVar) * (wireIdx + 1)));

		fscanf(filev, "%s", wireName);
	}
	wireName[strlen(wireName) - 1] = '\0';
	wire[wireIdx].name = (char *)(malloc(sizeof(char) * (strlen(wireName) - 1)));
	strcpy(wire[wireIdx].name, wireName);

	wire[wireIdx].varIndex = wireIdx;
	wire[wireIdx].info = 0;
	wire[wireIdx].func = -1;
	wire[wireIdx].A = NULL;
	wire[wireIdx].B = NULL;

	(*Nwire) = wireIdx + 1;

	return wire;
}// END of function


/* FUNCTION DESCRIPTION:	Read input source of every gate, input variable has no input source.
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 						Return poitnter of input source.
 * 				If failed:
 * 						Return NULL.
 * */
logicVar *readOperandIn(FILE *filev, logicVar *input, unsigned int Nin, logicVar *wire, unsigned int Nwire, logicVar *output, unsigned int Nout) {
	char cha;
	char varName[_MAX_CHAR_];
	unsigned int chaIdx = 0;
	
	fscanf(filev, "%c", &cha);
	while(cha != '(')
		fscanf(filev, "%c", &cha);
	fscanf(filev, "%c", &cha);
	while(cha == ' ')
		fscanf(filev, "%c", &cha);
	
	while((cha != ')') && (cha != ' ')) {
		varName[chaIdx ++] = cha;
		fscanf(filev, "%c", &cha);
	}
	varName[chaIdx] = '\0';
	for(unsigned int i = 0; i < Nin; i ++)
		if(strcmp(input[i].name, varName) == 0)
			return &input[i];

	for(unsigned int i = 0; i < Nwire; i ++)
		if(strcmp(wire[i].name, varName) == 0)
			return &wire[i];

	for(unsigned int i = 0; i < Nout; i ++)
		if(strcmp(output[i].name, varName) == 0)
			return &output[i];
	return NULL;
}// END of function


/* FUNCTION DESCRIPTION:	Read output of gate.
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 						Return pointer of output variable.
 * */
logicVar *readOperandOut(FILE *filev, logicVar *output, unsigned int Nout, logicVar *wire, unsigned int Nwire) {
	char cha;
	char varName[_MAX_CHAR_];
	unsigned int chaIdx = 0;

	fscanf(filev, "%c", &cha);
	while(cha != '(') 
		fscanf(filev, "%c", &cha);
	fscanf(filev, "%c", &cha);
	while(cha == ' ') {
		fscanf(filev, "%c", &cha);
	}
	while((cha != ')') && (cha != ' ')) {
		varName[chaIdx ++] = cha;
		fscanf(filev, "%c", &cha);
	}
	varName[chaIdx] = '\0';
	for(unsigned int i = 0; i < Nout; i ++)
		if(strcmp(output[i].name, varName) == 0)
			return &output[i];

	for(unsigned int i = 0; i < Nwire; i ++)
		if(strcmp(wire[i].name, varName) == 0)
			return &wire[i];
	return NULL;
}// END of function


/* FUNCTION DESCRIPTION:	Read function(gate) segment of Verilog file, and connect input, output, wire, to construct logic net.
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		
 * */
void readFunctions(FILE *filev, logicVar *input, unsigned int Nin, logicVar *output, unsigned int Nout, logicVar *wire, unsigned int Nwire) {	
	char str[_MAX_CHAR_];
	logicVar *sourceA;
	logicVar *sourceB;
	logicVar *destination;

        fscanf(filev, "%s", str);
	char cha;
	while(strcmp("endmodule", str) != 0) {
		if((strcmp("xorx",str) == 0) || (strcmp("xor",str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);

			sourceA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceA == NULL){ printf("sourceA is NULL\n"); return; }
			sourceB = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceB == NULL){ printf("sourceB is NULL\n"); return; }
			destination = readOperandOut(filev, output, Nout, wire, Nwire);
			if(destination == NULL) { printf("destination is NULL\n"); return; }

			destination->A = sourceA;	destination->B = sourceB;
			destination->info |= _XOR_;
			//printf("%s = %s XOR %s ", destination->name, sourceA->name, sourceB->name);
		}else if((strcmp("andx", str) == 0) || (strcmp("and", str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
		
			sourceA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceA == NULL){ printf("sourceA is NULL\n"); return; }
			sourceB = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceB == NULL){ printf("sourceB is NULL\n"); return; }
			destination = readOperandOut(filev, output, Nout, wire, Nwire);
			if(destination == NULL) { printf("destination is NULL\n"); return; }			
	
			destination->A = sourceA;	destination->B = sourceB;
			destination->info |= _AND_;
			//printf("%s = %s AND %s ", destination->name, sourceA->name, sourceB->name);
		}else if((strcmp("orx", str) == 0) || (strcmp("or", str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			
			sourceA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceA == NULL){ printf("sourceA is NULL\n"); return; }
			sourceB = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceB == NULL){ printf("sourceB is NULL\n"); return; }
			destination = readOperandOut(filev, output, Nout, wire, Nwire);
			if(destination == NULL) { printf("destination is NULL\n"); return; }			

			destination->A = sourceA;	destination->B = sourceB;
			destination->info |= _OR_;
			//printf("%s = %s OR %s ", destination->name, sourceA->name, sourceB->name);
		}else if((strcmp("invx",str) == 0) || (strcmp("inv",str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			
			sourceA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceA == NULL){ printf("sourceA is NULL\n"); return; }
			destination = readOperandOut(filev, output, Nout, wire, Nwire);
			if(destination == NULL) { printf("destination is NULL\n"); return; }			

			destination->A = sourceA;
			destination->info |= _INV_;
			//printf("%s = INV %s ", destination->name, sourceA->name);
		}else if((strcmp("bufx",str) == 0) || (strcmp("buf",str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);

			sourceA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			if(sourceA == NULL){ printf("sourceA is NULL\n"); return; }
			destination = readOperandOut(filev, output, Nout, wire, Nwire);
			if(destination == NULL) { printf("destination is NULL\n"); return; }
				
			destination->A = sourceA;
			destination->info |= _BUF_;
			//printf("%s = BUF %s ", destination->name, sourceA->name);
		}else {
			printf("Read Verilog readFunctions error#: only andx, orx, xorx, invx and bufx gates are allowed\n");
			printf("%s is invalid\n", str);
		}

		// skip current line
		fscanf(filev, "%s", str);
		//read next line
		fscanf(filev, "%s", str);
		//printf("read next line\n");
	}
	return;
}// END of function



