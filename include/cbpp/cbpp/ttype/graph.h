#ifndef CBPP_GRAPH_H
#define CBPP_GRAPH_H

#include <cstdint>
#include <iostream>
#include "cbpp/cb_alloc.h"

namespace cbpp {
	enum GRAPH_RIB_DIRECTION : uint8_t {
		RIB_NODIR,   //ребро без направления
		RIB_FORWARD, //А -> B
		RIB_BACKWARD //A <- B
	};
	
	template<typename T> struct GraphRib;
	
	template<typename T> struct GraphNode {
		T value;
		uint64_t uid = 0;
		
		uint64_t ribs = 0;
		GraphRib<T>** ribarr = nullptr;
	};
	
	template<typename T> struct GraphRib {
		uint64_t uid = 0;
		GraphNode<T> *a, *b;
		float weight = 1.0f;
		GRAPH_RIB_DIRECTION dir = RIB_NODIR;
	};
	
	template<typename T> GraphNode<T>* GetOtherNode(GraphRib<T>* rib, GraphNode<T>* curr) {
		if(rib == nullptr || curr == nullptr) { return nullptr; }
		
		if(curr->uid == rib->a->uid){
			return rib->b;
		}else{
			return rib->a;
		}
	}
	
	template<typename T> class Graph {
		public:
			Graph() {
				head = new GraphNode<T>;
				head->uid = 0;
				
				node_uid_cnt++;
			}
			
			Graph(const T& head_val) {
				head = new GraphNode<T>;
				head->uid = 0;
				
				node_uid_cnt++;
				
				head->value = head_val;
			}
			
			GraphRib<T>* ConnectNodes(GraphNode<T>* a, GraphNode<T>* b, float weight, GRAPH_RIB_DIRECTION rib_dir) {
				bool has_connection = false;
				
				for(uint64_t i = 0; i < a->ribs; i++){
					GraphRib<T>* cur_rib = a->ribarr[i];
					
					if(cur_rib->a->uid == a->uid && cur_rib->b->uid == b->uid){ //проверяем, нет ли между узлами связи
						has_connection = true;
						break;
					}
				}
				
				GraphRib<T>* new_rib = new GraphRib<T>;
				new_rib->a = a;
				new_rib->b = b;
				new_rib->uid = rib_uid_cnt; rib_uid_cnt++; //создаём новое ребро
				new_rib->weight = weight;
				new_rib->dir = rib_dir;
				
				ArrayPushBack(a->ribarr, a->ribs, new_rib); a->ribs++; //сообщаем узлам о новом ребре
				ArrayPushBack(b->ribarr, b->ribs, new_rib); b->ribs++;
				
				return new_rib;
			}
			
			GraphNode<T>* PopulateNode(GraphNode<T>* parent, const T& value, float weight = 1.0f, GRAPH_RIB_DIRECTION rib_dir = RIB_NODIR) {
				GraphNode<T>* new_node = CreateNode(value);
				
				ConnectNodes(parent, new_node, weight, rib_dir);
				
				return new_node;
			}
			
			void Print() {
				
				std::cout<<":)\n";
			}
			
			GraphNode<T>* Head() { return head; }
			
			GraphNode<T>* CreateNode(const T& value) {
				GraphNode<T>* new_node = new GraphNode<T>;
				new_node->value = value;
				new_node->uid = node_uid_cnt; node_uid_cnt++;
				
				return new_node;
			}
		
		private:		
			uint64_t node_uid_cnt = 0;
			uint64_t rib_uid_cnt = 0;
			GraphNode<T>* head = nullptr;
	};
}

#endif