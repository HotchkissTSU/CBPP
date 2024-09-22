#ifndef CBPP_TTYPE_TREE
#define CBPP_TTYPE_TREE

#include "cbpp/ttype/segarr.h"
#include <cstdint>

#define CBPP_MAX_TREE_DEPTH 1024

#include <stack>

namespace cbpp {
	template<typename T> struct TreeNode_t {
		T node_data;
		int32_t parent;
		SegArray<int32_t> children;
	};

	template<typename T> class Tree {
		public:
			Tree() = default;
			Tree(Tree& other);

		private:
			SegArray< TreeNode_t<T> > tree_data;
	};
}

#endif