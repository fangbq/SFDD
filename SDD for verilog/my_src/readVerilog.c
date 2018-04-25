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

logicNet *readVerilog(char *filename) {
	FILE *filev;
	if((filev = fopen(filename, "r" )) == NULL){
		printf("readVerilog: open file failed %s\n", filename);	
		return NULL;
	}
	logicNet *net = (logicNet *)malloc(sizeof(logicNet));
	if(net == NULL){
		printf("readVerilog: init logic net failed\n");
		fclose(filev);
		return NULL;
	}
	char cha;
	fscanf(filev, "%c", &cha);
	while(cha != ';')
		fscanf(filev, "%c", &cha);
	net->input = readInput(filev, &(net->Nin));
	if(net->input == NULL){
		free(net);
		fclose(filev);
		return NULL;
	}
	net->output = readOutput(filev, &(net->Nout));
	if(net->output == NULL){
		free(net->input);		free(net);
		fclose(filev);
		return NULL;
	}
	net->wire = readWire(filev, &(net->Nwire));
	if(net->wire == NULL){
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
 * 				Return an input-array contains all input variables.
 * 						
 * */
logicVar *readInput(FILE *filev, unsigned int *Nin) {
	char varName[_MAX_CHAR_];
	unsigned int inputIdx = 0;

	logicVar *input = (logicVar *)malloc(sizeof(logicVar));
	if(input == NULL){
		printf("readVerilog: init input-array failed\n");
		return NULL;
	}
       	fscanf(filev, "%s", varName);
	while(strcmp("input", varName) != 0)
        	fscanf(filev, "%s", varName);

	fscanf(filev, "%s", varName);
	while(varName[strlen(varName) - 1] != ';') {
		varName[strlen(varName) - 1] = '\0';
		input[inputIdx].name = malloc(sizeof(char) * (strlen(varName) - 1));
		strcpy(input[inputIdx].name, varName);

		input[inputIdx].varIndex = inputIdx + 1;
		input[inputIdx].info = _INPUT_;
		input[inputIdx].func = NULL;
		input[inputIdx].A = NULL;
		input[inputIdx].B = NULL;
		inputIdx ++;
		input = realloc(input, sizeof(logicVar) * (inputIdx + 1));

		fscanf(filev, "%s", varName);
	}
	varName[strlen(varName) - 1] = '\0';
	input[inputIdx].name = malloc(sizeof(char) * (strlen(varName) - 1));
	strcpy(input[inputIdx].name, varName);
	
	input[inputIdx].varIndex = inputIdx + 1;
	input[inputIdx].info = _INPUT_;
	input[inputIdx].func = NULL;
	input[inputIdx].A = NULL;
	input[inputIdx].B = NULL;

	(*Nin) = inputIdx + 1;

	return input;
}// END of function


/* FUNCTION DESCRIPTION:	Read output segment of Verilog file
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 				Return an output-array contains all output variables.
 * */
logicVar *readOutput(FILE *filev, unsigned int *Nout) {
	char varName[_MAX_CHAR_];
	unsigned int outputIdx = 0;

	logicVar *output = (logicVar *)malloc(sizeof(logicVar));
	if(output == NULL){
		printf("readVerilog: init output-array failed\n");
		return NULL;
	}
       	fscanf(filev, "%s", varName);
	while(strcmp("output", varName) != 0)
        	fscanf(filev, "%s", varName);

	fscanf(filev, "%s", varName);
	while(varName[strlen(varName) - 1] != ';') {
		varName[strlen(varName) - 1] = '\0';
		output[outputIdx].name = malloc(sizeof(char) * (strlen(varName) - 1));
		strcpy(output[outputIdx].name, varName);

		output[outputIdx].varIndex = outputIdx;
		output[outputIdx].info = _OUTPUT_;
		output[outputIdx].func = NULL;
		output[outputIdx].A = NULL;
		output[outputIdx].B = NULL;
		outputIdx ++;
		output = realloc(output, sizeof(logicVar) * (outputIdx + 1));

		fscanf(filev, "%s", varName);
	}
	varName[strlen(varName) - 1] = '\0';
	output[outputIdx].name = malloc(sizeof(char) * (strlen(varName) - 1));
	strcpy(output[outputIdx].name, varName);

	output[outputIdx].varIndex = outputIdx;
	output[outputIdx].info = _OUTPUT_;
	output[outputIdx].func = NULL;
	output[outputIdx].A = NULL;
	output[outputIdx].B = NULL;

	(*Nout) = outputIdx + 1;

	return output;
}// END of function


/* FUNCTION DESCRIPTION:	Read wire segment of Verilog file
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 				Return a wire-array contains all wire variables.
 * */
logicVar *readWire(FILE *filev, unsigned int *Nwire) {
	char wireName[_MAX_CHAR_];
	unsigned int wireIdx = 0;

	logicVar *wire = (logicVar *)malloc(sizeof(logicVar));
	if(wire == NULL){
		printf("readVerilog: init wire-array failed\n");
		return NULL;
	}	
	fscanf(filev, "%s", wireName);
	while(strcmp("wire", wireName) !=0 )
        	fscanf(filev, "%s", wireName);

	fscanf(filev, "%s", wireName);
	while(wireName[strlen(wireName) - 1] != ';') {

		wireName[strlen(wireName) - 1] = '\0';
		wire[wireIdx].name = malloc(sizeof(char) * (strlen(wireName) - 1));
		strcpy(wire[wireIdx].name, wireName);

		wire[wireIdx].varIndex = wireIdx;
		wire[wireIdx].info = _WIRE_;
		wire[wireIdx].func = NULL;
		wire[wireIdx].A = NULL;
		wire[wireIdx].B = NULL;
		wireIdx ++;
		wire = realloc(wire, sizeof(logicVar) * (wireIdx + 1));

		fscanf(filev, "%s", wireName);
	}
	wireName[strlen(wireName) - 1] = '\0';
	wire[wireIdx].name = malloc(sizeof(char) * (strlen(wireName) - 1));
	strcpy(wire[wireIdx].name, wireName);

	wire[wireIdx].varIndex = wireIdx;
	wire[wireIdx].info = _WIRE_;
	wire[wireIdx].func = NULL;
	wire[wireIdx].A = NULL;
	wire[wireIdx].B = NULL;

	(*Nwire) = wireIdx + 1;

	return wire;
}// END of function


/* FUNCTION DESCRIPTION:	Read input source of every gate, input variable has no input source.
 * PRE-CONDITION:		Empty.
 * POST-CONDITION:		If success:
 * 				Return poitnter of input source.
 * 				If failed:
 * 				Return NULL.
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
 * 				Return pointer of output variable.
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
	logicVar *iA;
	logicVar *iB;
	logicVar *O;

        fscanf(filev, "%s", str);
	char cha;
	while(strcmp("endmodule", str) != 0) {
		if((strcmp("xorx",str) == 0) || (strcmp("xor",str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			iA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			iB = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			O = readOperandOut(filev, output, Nout, wire, Nwire);
			O->A = iA;	O->B = iB;
			O->info |= _XOR_;
		}else if((strcmp("andx", str) == 0) || (strcmp("and", str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			iA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			iB = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			O = readOperandOut(filev, output, Nout, wire, Nwire);	
			O->A = iA;	O->B = iB;
			O->info |= _AND_;
		}else if((strcmp("orx", str) == 0) || (strcmp("or", str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			iA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			iB = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			O = readOperandOut(filev, output, Nout, wire, Nwire);	
			O->A = iA;	O->B = iB;
			O->info |= _OR_;
		}else if((strcmp("invx",str) == 0) || (strcmp("inv",str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			iA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			O = readOperandOut(filev, output, Nout, wire, Nwire);	
			O->A = iA;
			O->info |= _INV_;
		}else if((strcmp("bufx",str) == 0) || (strcmp("buf",str) == 0)){
			fscanf(filev, "%c", &cha);
			while(cha != '(') 
				fscanf(filev, "%c", &cha);
			iA = readOperandIn(filev, input, Nin, wire, Nwire, output, Nout);
			O = readOperandOut(filev, output, Nout, wire, Nwire);
			O->A = iA;
			O->info |= _BUF_;
		}else {
			printf("readVerilog: only andx, orx, xorx, invx and bufx gates are allowed\n");
			printf("%s is invalid\n", str);
		}

		// skip current line
		fscanf(filev, "%s", str);
		//read next line
		fscanf(filev, "%s", str);
	}
	return;
}// END of function



