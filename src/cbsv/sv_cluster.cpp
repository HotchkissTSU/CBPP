#include "cbsv/sv_cluster.h"

namespace cbsv{	
	UserCluster::UserCluster(uint16_t size){
		if(size > 0){
			usercount = size;
		}else{
			usercount = 1;
		}
		
		allocated = false;
		user_array = nullptr;
		
		clusterid = cluster_counter;
		
		cluster_counter++;
		
		_allocate();
	}
	
	int32_t UserCluster::AddUser(User* us){
		if(!allocated){ return -1; }
		if(us == nullptr){ return -1; }

		int32_t found_index = -1;
		
		for(uint16_t i = 0; i < usercount; i++){
			if(user_array[i] == nullptr){
				found_index = (int32_t)i;
				
				us->clusterid = clusterid;
				us->userid = i;				
				
				user_array[i] = us;
				
				break;
			}
		}
		
		return found_index;
	}
	
	uint16_t UserCluster::GetFreeSlots(){
		if(!allocated){ return 0; }
		
		uint16_t used_slots = 0;
		
		for(uint16_t i = 0; i < usercount; i++){
			if(GetUser(i) != nullptr){
				used_slots++;
			}
		}
		
		return usercount - used_slots;
	}
	
	User* UserCluster::GetUser(uint16_t userid){
		if(!allocated){ return nullptr; }
		
		if(userid < usercount){
			return user_array[userid];
		}else{
			return nullptr;
		}
	}
	
	void UserCluster::_free(){
		if(allocated && user_array != nullptr){
			for(uint16_t i = 0; i < usercount; i++){
				if(user_array[i] != nullptr){ delete user_array[i]; }
			}
			
			delete[] user_array;
			
			allocated = false;
		}
	}
	
	void UserCluster::_allocate(){
		if(allocated){
			_free();
		}
		
		user_array = new User*[usercount];
		
		for(uint16_t i = 0; i < usercount; i++){
			user_array[i] = nullptr;
		}
		
		allocated = true;
	}
	
	void UserCluster::TickThread(){
		if(allocated){
			for(uint16_t i = 0; i < usercount; i++){
				User* us = user_array[i];
				
				if(us != nullptr){
					//тут надо почесать список необработанных пакетов со всех клиентов, найти свои и покумекать над ними
				}
			}
		}
	}
	
	UserCluster::~UserCluster(){
		_free();
		cluster_counter = cluster_counter - 1;
	}
}