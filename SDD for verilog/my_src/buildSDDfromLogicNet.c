#include "readVerilog.h"
#include "sddapi.h"

// returns an SDD node representing ( node1 => node2 )
SddNode* sdd_imply(SddNode* node1, SddNode* node2, SddManager* manager) {
	SddNode* neg_node1 = sdd_negate(node1, manager);
	sdd_ref(neg_node1, manager);
	SddNode* alpha = sdd_disjoin(neg_node1, node2, manager);
	sdd_deref(neg_node1, manager);
	return alpha;
}

// returns an SDD node representing ( node1 <=> node2 )
SddNode* sdd_equiv(SddNode* node1, SddNode* node2, SddManager* manager) {
	SddNode* imply1 = sdd_imply(node1, node2, manager);
	sdd_ref(imply1, manager);
	SddNode* imply2 = sdd_imply(node2, node1, manager);
	sdd_ref(imply2, manager);
	SddNode* alpha = sdd_conjoin(imply1, imply2, manager);
	sdd_deref(imply1, manager); sdd_deref(imply2, manager);
	return alpha;
}

// returns an SDD node representing ( node1 XOR node2 )
SddNode* sdd_exor(SddNode* node1, SddNode* node2, SddManager* manager) {
	SddNode *equiv = sdd_equiv(node1, node2, manager);
	sdd_ref(equiv, manager);
	SddNode *alpha = sdd_negate(equiv, manager);
	sdd_deref(equiv, manager);
	return alpha;
}

/* FUNCTION-DESCIPTION:		Build SDD formula.
 * PRE-CONDITION:		All literals have been built.
 * POST-CONDITION:		
 * */

void buildSDD(logicVar *var, SddManager *manager) {
	// build SDD literal
	if(VARTYPE(var->info) == _INPUT_){
		var->func = sdd_manager_literal(var->varIndex, manager);
		sdd_ref((SddNode *)(var->func), manager);
		return;
	}
	// build SDD formulae
	unsigned int gateType = GATETYPE(var->info);
	if(gateType == _XOR_ || gateType == _AND_ || gateType == _OR_){
		buildSDD(var->A, manager);	buildSDD(var->B, manager);
		SddNode *fA = (SddNode *)(var->A->func);
		SddNode *fB = (SddNode *)(var->B->func);
		if(gateType == _XOR_)		{ var->func = sdd_exor(fA, fB, manager); }
		else if(gateType == _AND_)	{ var->func = sdd_conjoin(fA, fB, manager); }
		else				{ var->func = sdd_disjoin(fA, fB, manager); }
		sdd_ref((SddNode *)(var->func), manager);	// ref deref
		sdd_deref(fA, manager);	sdd_deref(fB, manager);
	}else{	// INV or BUF
		buildSDD(var->A, manager);
		SddNode *fA = (SddNode *)(var->A->func);
		
		if(gateType == _INV_)		{ var->func = sdd_negate(fA, manager); }
		else				{ var->func = fA; }
		sdd_ref((SddNode *)(var->func), manager);	// ref deref
		sdd_deref(fA, manager);
	}

	return;
}


/* FUNCTION-DESCIPTION:		Build SDD formula(with manually gc).
 * PRE-CONDITION:		All literals have been built.
 * POST-CONDITION:		
 * */

void buildSDD_with_gc(logicVar *var, SddManager *manager) {
	// build SDD literal
	if(VARTYPE(var->info) == _INPUT_){
		var->func = sdd_manager_literal(var->varIndex, manager);
		sdd_ref((SddNode *)(var->func), manager);
		return;
	}
	// build SDD formulae
	unsigned int gateType = GATETYPE(var->info);
	if(gateType == _XOR_ || gateType == _AND_ || gateType == _OR_){
		buildSDD(var->A, manager);	buildSDD(var->B, manager);
		SddNode *fA = (SddNode *)(var->A->func);
		SddNode *fB = (SddNode *)(var->B->func);
		if(gateType == _XOR_)		{ var->func = sdd_exor(fA, fB, manager); }
		else if(gateType == _AND_)	{ var->func = sdd_conjoin(fA, fB, manager); }
		else				{ var->func = sdd_disjoin(fA, fB, manager); }
		sdd_ref((SddNode *)(var->func), manager);	// ref deref
		sdd_deref(fA, manager);	sdd_deref(fB, manager);
	}else{	// INV or BUF
		buildSDD(var->A, manager);
		SddNode *fA = (SddNode *)(var->A->func);
		
		if(gateType == _INV_)		{ var->func = sdd_negate(fA, manager); }
		else				{ var->func = fA; }
		sdd_ref((SddNode *)(var->func), manager);	// ref deref
		sdd_deref(fA, manager);
	}
	
	// mually garbage collection
	SddSize dead = sdd_manager_dead_count(manager);
	SddSize live = sdd_manager_live_count(manager);
	if(dead == live)	{ sdd_manager_garbage_collect(manager); }

	return;
}
