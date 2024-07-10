#include "cbsv/sv_cluster.h"

#include "cbpp/cb_alloc.h"
#include "cbsv/sv_queue.h"

#include <string>

using std::wstring;

namespace cbsv{	
	bool AddrComp(IPaddress* ip1, IPaddress* ip2){
		return (ip1->host == ip2->host) && (ip1->port == ip2->port);
	}

	UserCluster::UserCluster(uint16_t size){		
		if(size > 0){
			usercount = size;
		}else{
			usercount = 1;
		}
		
		allocated = false;
		user_array = nullptr;
		
		cluster_counter++;
		
		_allocate();
		
		std::thread cluster_thr(TickThread, this);
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
	
	int32_t UserCluster::HasUser(User* us){
		int32_t out = -1;
		
		if(allocated && us != nullptr){
			for(int i = 0; i < usercount; i++){
				User* _us = user_array[i];
				
				if(_us->userid == us->userid && _us->clusterid == us->clusterid){
					out = i;
					break;
				}
			}
		}
		
		return out;
	}
	
	void UserCluster::RemoveUser(User* us, bool disconnect){
		if(allocated && us != nullptr){
			uint16_t check = HasUser(us);
			if(check){
				if(disconnect){ us->Disconnect("Removed from cluster"); }
				user_array[check] = nullptr;
			}
		}
	}
	
	void UserCluster::_free(){
		if(allocated && user_array != nullptr){
			for(uint16_t i = 0; i < usercount; i++){
				if(user_array[i] != nullptr){
					user_array[i]->Disconnect("Cluster termination"); //отключаем всех своих клиентов, чтобы не делать висяков
					
					delete user_array[i];
				}
			}
			
			delete[] user_array;
			user_array = nullptr;
			
			allocated = false;
			
			if(cluster_arr != nullptr){
				for(int i = 0; i < cluster_num; i++){ //удаляем себя из списка кластеров
					if(cluster_arr[i] == this){
						cluster_arr[i] = nullptr;
						break;
					}
				}
			}
			
		}
	}
	
	void UserCluster::_allocate(){
		if(clusters_alloc){
			if(allocated){
				_free();
			}
			
			user_array = cbpp::Allocate<User*>(usercount);
			
			allocated = true;
		
			for(int i = 0; i < CBSV_CLUSTERS; i++){
				if(cluster_arr[i] == nullptr){
					cluster_arr[i] = this;
					
					clusterid = i;
					break;
				}
			}
		}
	}
	
	void UserCluster::TickThread(){
		if(allocated){
			for(uint16_t i = 0; i < usercount; i++){
				User* us = user_array[i];
				
				if(us != nullptr && !SV_PkgQue_busy){
					SV_PkgQue_busy = true;
					
					for(uint16_t j = 0; j < SV_PkgQue.Length(); j++){
						Packet* pkg = SV_PkgQue[j];
						
						if(pkg != nullptr){
							if(AddrComp(&pkg->source, &us->cl_address)){
								
								std::cout<<"CLUSTER "<<clusterid<<"GOT PKG FOR USER "<<j<<'\n';
								
								delete pkg;
								SV_PkgQue[j] = nullptr; //обработали свой пакет и очистили память
							}
						}
					}
					
					SV_PkgQue_busy = false;
				}
			}
		}
	}
	
	UserCluster::~UserCluster(){
		_free();
		cluster_counter = cluster_counter - 1;
	}
	
	void ClearClusters(){
		if(!clusters_alloc){
			for(int i = 0; i < cluster_num; i++){
				cluster_arr[i]->_free();
				delete cluster_arr[i];
				cluster_counter -= 1;
			}
			
			delete[] cluster_arr;
			
			clusters_alloc = false;
		}
	}
	
	void AllocateClusters(uint8_t numc, uint16_t csize){		
		if(clusters_alloc){
			ClearClusters();
		}
		
		cluster_num = numc;
		
		cluster_arr = new UserCluster*[cluster_num];
		for(int i = 0; i < cluster_num; i++){
			cluster_arr[i] = new UserCluster(csize);
		}
			
		clusters_alloc = true;
	}
	
	UserID AddUser(User* us){
		UserID out;
		out.allocated = false;
		
		if(clusters_alloc && us != nullptr){
			for(int i = 0; i < cluster_num; i++){
				if(cluster_arr[i] != nullptr){
					if(cluster_arr[i]->GetFreeSlots() > 0){
						out.user_id = cluster_arr[i]->AddUser(us);
						out.cluster_id = i;
						out.allocated = true;
						break;
					}
				}
			}
		}
		
		return out;
	}
}