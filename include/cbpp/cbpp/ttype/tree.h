#ifndef CBPP_TTYPE_TREE
#define CBPP_TTYPE_TREE

#include "cbpp/ttype/segarr.h"
#include <cstdint>

#include <stack>

namespace cbpp {
	template<typename T> struct TreeNode_t {
		int32_t parent;
		T node_data;
		SegArray<int32_t> children;
	};

	template<typename T> class Tree {
		public:
			Tree() = default;
			Tree(Tree& other);

			int32_t PopulateNode(T& data);

		private:
			SegArray< TreeNode_t<T> > tree_data;
	};
}

#endif