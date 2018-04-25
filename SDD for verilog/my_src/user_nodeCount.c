/* #################### user defined sdd node count #################### */
#include "sddapi.h"

// recusively count sdd node
SddSize udef_node_count(SddNode *node){
	if(sdd_node_bit(node))		return 0;
	sdd_node_set_bit(1, node);
	SddSize count = 1;
	if(!sdd_node_is_decision(node))	return count;
	SddNodeSize size = sdd_node_size(node);
	SddNode **element = sdd_node_elements(node);
	for(unsigned int i = 0; i < size; i ++){
		// count += udef_node_count(element[i]);		// previous
		count += udef_node_count(element[2 * i]);		// now
		count += udef_node_count(element[2 * i + 1]);
	}
	return count;
}

// reset user-bit of sdd node(recommended by Sdd manual)
void udef_node_info_reset(SddNode *node){
	if(!sdd_node_bit(node)) 	return;	// has been reset
	sdd_node_set_bit(0, node);		// reset
	if(!sdd_node_is_decision(node))	return;
	SddNodeSize size = sdd_node_size(node);
	SddNode **element = sdd_node_elements(node);
	for(unsigned int i = 0; i < size; i ++){
		// udef_node_info_reset(element[i]);			// previous
		udef_node_info_reset(element[2 * i]);			// now
		udef_node_info_reset(element[2 * i + 1]);
	}
}
