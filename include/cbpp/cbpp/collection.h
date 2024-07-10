#ifndef CBPP_COLLECTION
#define CBPP_COLLECTION

#include <map>

namespace cbpp{
	template<typename key_t, typename value_t>
	class BaseCollection{
		std::map<key_t, value_t> coll_map;
		value_t default_val;
		
		public:
			BaseCollection(){}
			BaseCollection(value_t def_val){ default_val = def_val; }
			BaseCollection(std::map<key_t, value_t> nmap){ Reset(); coll_map = nmap; }
			
			void SetDefaultValue(value_t def_val){ default_val = def_val; }
			value_t GetDefaultValue(){ return default_val; }
			
			virtual void Set(key_t key, value_t value){
				coll_map[key] = value;
			}
			
			virtual value_t Get(key_t key){
				if(coll_map.find(key) != coll_map.end()){
					return coll_map[key];
				}else{
					return default_val;
				}
			}
			
			void Reset(){
				for(auto it = coll_map.start(); it != coll_map.end(); it++){
					it->second.~value_t;
				}
			}
	};
}

#endif