
#include <string.h>
#include <libgen.h>
#include "sddapi.h"
#include "readVerilog.h"

// returns an SDD node representing ( node1 => node2 )
extern SddNode* sdd_imply(SddNode* node1, SddNode* node2, SddManager* manager);

// returns an SDD node representing ( node1 <=> node2 )
extern SddNode* sdd_equiv(SddNode* node1, SddNode* node2, SddManager* manager);

// returns an SDD node representing ( node1 XOR node2 )
extern SddNode* sdd_exor(SddNode* node1, SddNode* node2, SddManager* manager);

// buildSDDfromLogicNet.c
extern void buildSDD(logicVar *var, SddManager *manager);

// user_nodeCount.c
extern void udef_node_info_reset(SddNode *node);
extern SddSize udef_node_count(SddNode *node);

int main(int argc, char** argv) {

	// char filename[] = "";
	// strcat(filename, argv[1]);

	logicNet *net = readVerilog(argv[1]);
	SddLiteral var_count = net->Nin;

	// active auto garbage collection and minimization
	int auto_gc_and_minimize = 1;
	SddManager *manager = sdd_manager_create(var_count, auto_gc_and_minimize);
	clock_t start = clock();
	// using file name to generate save file name
	char savefile[_MAX_CHAR_];

// output MCNC Experiment
FILE *f_out = fopen("mcnc-output-table.txt", "a" );
fprintf(f_out, "%s\t", basename(argv[1]));
fprintf(f_out, "%d\t", net->Nin);
fprintf(f_out, "%d\t", net->Nout);
fprintf(f_out, "%d\t", net->Nwire);

	// build SDD formulae and save output formulae
	// printf("saving sdd and vtree ... ");
	// sdd node count
	SddSize nodeCnt = 0;
	for(unsigned int i = 0; i < net->Nout; i ++){
		buildSDD(&(net->output[i]), manager);
		// count node
		// SddNode *node = (SddNode *)((net->output[i]).func);
		// nodeCnt += udef_node_count(node);
		
		// fake save, only output file name
		// printf("saving formula: %s\n", net->output[i].name);
		// memset(savefile, 0, _MAX_CHAR_);
		// strcat(savefile, "output/");
		// strcat(savefile, basename(argv[1]));
		// strcat(savefile, "_");
		// strcat(savefile, net->output[i].name);
		// strcat(savefile, ".sdd");
		

		// printf("Test message(fake save): save as %s\n", savefile);
		// save sdd in file
		//sdd_save(savefile, (net->output[i]).func);
		
		// garbage collection
		/*
		printf("dead sdd nodes = %zu\n", sdd_manager_dead_count(manager));
		printf("garbage collection ...\n");
		sdd_manager_garbage_collect(manager);
		printf("dead sdd nodes = %zu\n", sdd_manager_dead_count(manager));
		*/
	}

	// generate file name for saving vtree
	char save_vtree[_MAX_CHAR_];
	memset(save_vtree, 0, _MAX_CHAR_);
	strcat(save_vtree, "output_vtree/042201/");
	strcat(save_vtree, basename(argv[1]));
	strcat(save_vtree, ".vtree");
	// save vtree
	sdd_vtree_save(save_vtree, sdd_manager_vtree(manager));

	// optional: reset user_bit of Sdd node
	// for(unsigned int i = 0; i < net->Nout; i ++){
	// 	SddNode *node = (SddNode *)((net->output[i]).func);
	// 	udef_node_info_reset(node);
	// }	

fprintf(f_out, "%u\t", sdd_manager_live_size(manager));

	clock_t finish = clock();
	double ptime = (double)(finish - start) / CLOCKS_PER_SEC;  // runtime

fprintf(f_out, "%.4lf\n", ptime);

//	printf("sdd live node = %u\n", sdd_manager_live_count(manager));	
//	printf("sdd_manager_count = %u\n", sdd_manager_count(manager));	
//	printf("sdd graph contains %u nodes\n", nodeCnt);

	// printf("saving SDD done\n");

	sdd_manager_free(manager);
	// printf("free SDD manager\n");

fclose(f_out);

	freeLogicNet(net);	

	printf("Finish reading file %s\n", argv[1]);

	return 0;
}
