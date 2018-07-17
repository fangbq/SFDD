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


// traversal vtree and get variable order of vtree
void get_var_order_from_vtree(Vtree *vtree, SddLiteral *order, SddSize *idx){
	if(vtree == NULL)		return;
	Vtree *l_vtree = sdd_vtree_left(vtree);
	Vtree *r_vtree = sdd_vtree_right(vtree);
	get_var_order_from_vtree(l_vtree, order, idx);
	if(sdd_vtree_is_leaf(vtree)){ order[(*idx) ++] =  sdd_vtree_var(vtree); }
	get_var_order_from_vtree(r_vtree, order, idx);
	return;
}// END of function

int main(int argc, char** argv) {

	Vtree* aux_vtree = sdd_vtree_read(argv[1]);
	SddLiteral varCnt = sdd_vtree_var_count(aux_vtree);
	// var-order array, 	var-order array index
	SddLiteral order[1000];	SddSize idx = 0;

	// traversal vtree, store variable order
	get_var_order_from_vtree(aux_vtree, order, &idx);
	// free aux_vtree
	sdd_vtree_free(aux_vtree);

	// create new vtree with variable order, manager
	Vtree *vtree = sdd_vtree_new_with_var_order(varCnt, order, "right");
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

	char save_vtree[_MAX_CHAR_];
	memset(save_vtree, 0, _MAX_CHAR_);
	strcat(save_vtree, "test3_output/vtree/");
	strcat(save_vtree, basename(argv[2]));
	strcat(save_vtree, ".vtree");
	sdd_vtree_save(save_vtree, sdd_manager_vtree(manager));
	//printf("save vtree done\n");	


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
