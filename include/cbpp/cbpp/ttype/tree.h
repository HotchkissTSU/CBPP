#ifndef CBPP_TTYPE_TREE
#define CBPP_TTYPE_TREE

#include "cbpp/ttype/segarr.h"

#define CBPP_MAX_TREE_DEPTH 1024

#include <stack>

namespace cbpp {
	template<typename T> struct _stk{
		T* p;
		uint64_t i;
	};
	
	template<typename T> struct TreeNode {
		T value;
		TreeNode<T>* parent;
		SegArray< TreeNode<T>* > children;
	};
	
	template<typename T> struct BinTreeNode {
		T value;
		BinTreeNode<T> *parent, *left, *right;
	};
	
	template<typename T> class Tree {
		public:
			Tree() {
				T zerov;
				_alloc_head(zerov);
			}
			
			Tree(const T& head_value) {
				_alloc_head(const_cast<T&>(head_value));
			}
			
			TreeNode<T>* Head() {
				return head;
			}
			
			TreeNode<T>* PopulateNode(TreeNode<T>* parent, const T& _value) {
				if(parent == nullptr){ return nullptr; }
				
				T& value = const_cast<T&>(_value);
				
				TreeNode<T>* new_node = new TreeNode<T>;
				new_node->value = value;
				new_node->parent = parent;
				
				parent->children.PushBack(new_node);
				
				return new_node;
			}
			
			void IterateDFS(void (*iter_func_ptr)(TreeNode<T>*)){
				std::stack<TreeNode<T>*> stk_; //рекурсия потенциально может засрать встроенный стек, симулируем её
				stk_.push(head);
				
				while(!stk_.empty()){
					TreeNode<T>* curr = stk_.top();
					stk_.pop();
					
					iter_func_ptr(curr); //вызываем функцию-обработчик

					for(std::size_t i = 0; i < curr->children.Length(); i++){
						stk_.push(curr->children.At(i));
					}
				}
			}
			
			void IterateDFS(void (*iter_func_ptr)(Tree<T>*, TreeNode<T>*)){
				std::stack<TreeNode<T>*> stk_; //рекурсия потенциально может засрать встроенный стек, симулируем её
				stk_.push(head);
				
				while(!stk_.empty()){
					TreeNode<T>* curr = stk_.top();
					stk_.pop();
					
					iter_func_ptr(this, curr); //вызываем функцию-обработчик

					for(std::size_t i = 0; i < curr->children.Length(); i++){
						stk_.push(curr->children.At(i));
					}
				}
			}
			
			~Tree(){
				std::stack<TreeNode<T>*> stk_; //рекурсия потенциально может засрать встроенный стек, симулируем её
				stk_.push(head);
				
				while(!stk_.empty()){
					TreeNode<T>* curr = stk_.top();
					stk_.pop();

					for(std::size_t i = 0; i < curr->children.Length(); i++){
						stk_.push(curr->children.At(i));
					}
					
					if(curr->parent != nullptr){
						curr->parent->children.RemoveValue(curr);
					}
					
					delete curr;
				}
			}
			
		private:
			void _alloc_head(T& val) {
				head = new TreeNode<T>;
				head->value = val;
				head->parent = nullptr;
			}
		
			TreeNode<T>* head = nullptr;
	};
}

#endif