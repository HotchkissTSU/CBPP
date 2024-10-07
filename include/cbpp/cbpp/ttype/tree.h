#ifndef CBPP_TTYPE_TREE
#define CBPP_TTYPE_TREE

#include "cbpp/ttype/segarr.h"
#include "cbpp/error.h"

#include <cstdint>
#include <stdio.h>
#include <stack>

namespace cbpp {
	typedef int32_t treeindex_t;

	template<typename T> struct TreeNode_t {
		treeindex_t parent;
		T node_data;
		uint8_t valid = false;
		SegArray<treeindex_t> children;
	};

	template<typename T> class Tree {
		public:
			Tree() {
				TreeNode_t<T> head;
				head.parent = -1;
				tree_data.PushBack(head);
			}

			Tree(Tree& other) {
				tree_data = other.GetRawData();
			}

			void RemoveNode(treeindex_t id);
			treeindex_t PopulateNode(treeindex_t id, T& data);
			treeindex_t Find(T& data);

			TreeNode_t<T>& operator[]( treeindex_t id ) {
				return this->At(id);
			}

			TreeNode_t<T>& At( treeindex_t id ) {
				if(!is_valid(id)){
					char err_log[256];
					snprintf(err_log, 255, "Attempt to get node with an invalid index '%d'", id);
					CbThrowError(err_log);
				}

				return tree_data.At(id);
			}

			SegArray<TreeNode_t<T>>& GetRawData() {
				return tree_data;
			}

			void Print() {
				
			}

		private:
			SegArray< TreeNode_t<T> > tree_data;

			void traverse_tree(treeindex_t id, void (*iter)(treeindex_t)) {
				if (!is_valid(id)) return;

				std::stack<treeindex_t> s;
				s.push(id);

				while (!s.empty()) {
					treeindex_t nodeid = s.top();
					s.pop();

					for (int i = At(nodeid).children.Length() - 1; i >= 0; i--) {
						s.push(At(nodeid).children[i]);
					}
				}
			}

			bool is_valid(treeindex_t node_id) {
				if(node_id >= 0 && node_id < tree_data.Length()) {
					TreeNode_t<T>& tmp = tree_data.At(node_id);
					return (bool)tmp.valid;
				}else{
					return false;
				}
			}
	};
}

#endif