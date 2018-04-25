#include <string.h>
#include <libgen.h>
#include "sddapi.h"
#include "readVerilog.h"
#include "time.h"

// copy from sdd test-4.c

// returns an SDD node representing ( node1 => node2 )
extern SddNode* sdd_imply(SddNode* node1, SddNode* node2, SddManager* manager);

// returns an SDD node representing ( node1 <=> node2 )
extern SddNode* sdd_equiv(SddNode* node1, SddNode* node2, SddManager* manager);

// returns an SDD node representing ( node1 XOR node2 )
extern SddNode* sdd_exor(SddNode* node1, SddNode* node2, SddManager* manager);

// buildSDDfromLogicNet.c
void buildSDD_with_gc(logicVar *var, SddManager *manager);


//	argv[1] is vtree file, argv[2] is verilog file


int main(int argc, char** argv) {

	Vtree* vtree = sdd_vtree_read(argv[1]);
	SddManager* manager = sdd_manager_new(vtree);
	// close gc and minimize
	sdd_manager_auto_gc_and_minimize_off(manager);

	//printf("start reading\n");
	clock_t start = clock();
	logicNet *net = readVerilog(argv[2]);
	
	char savefile[_MAX_CHAR_];

// output MCNC Experiment
FILE *f_out = fopen("mcnc-output-table.txt", "a" );
fprintf(f_out, "%s\t", basename(argv[1]));
fprintf(f_out, "%d\t", net->Nin);
fprintf(f_out, "%d\t", net->Nout);
fprintf(f_out, "%d\t", net->Nwire);

	for(unsigned int i = 0; i < net->Nout; i ++){
		buildSDD_with_gc(&(net->output[i]), manager);
/*
		printf("saving formula: %s\n", net->output[i].name);
		memset(savefile, 0, _MAX_CHAR_);
		strcat(savefile, "test3_output/sdd/");
		strcat(savefile, basename(argv[2]));
		strcat(savefile, "_");
		strcat(savefile, net->output[i].name);
		strcat(savefile, ".sdd");
		
		//printf("save output sdd %s done\n", savefile);
		sdd_save(savefile, (net->output[i]).func);
*/
	}
/*
	char save_vtree[_MAX_CHAR_];
	memset(save_vtree, 0, _MAX_CHAR_);
	strcat(save_vtree, "test3_output/vtree/");
	strcat(save_vtree, basename(argv[2]));
	strcat(save_vtree, ".vtree");
	sdd_vtree_save(save_vtree, sdd_manager_vtree(manager));
	//printf("save vtree done\n");	
*/

fprintf(f_out, "%u\t", sdd_manager_live_size(manager));

	clock_t finish = clock();
	double ptime = (double)(finish - start) / CLOCKS_PER_SEC;  // runtime

fprintf(f_out, "%.4lf\n", ptime);

	sdd_vtree_free(vtree);
	sdd_manager_free(manager);
	freeLogicNet(net);

fclose(f_out);

	printf("Finish reading file vtree(%s) and verilog(%s)\n", basename(argv[1]), basename(argv[2]));

  return 0;
}
