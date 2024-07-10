#include "graph.h"
#include "cbpp/cb_alloc.h"

namespace cbpp{
	Graph::Graph(){
		nodes = cbpp::Allocate<GraphNode*>(1);
		nodesnum = 1;
		ribs = cbpp::Allocate<GraphRib*>(1);
		ribsnum = 1;
	}
	
	uint32_t Graph::AddNode(){
		
	}
}