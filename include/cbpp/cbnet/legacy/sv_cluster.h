/*
На сервере все пользователи разбиты на группы, которые называются кластерами.
*/

#ifndef CBSV_SV_CLUSTER
#define CBSV_SV_CLUSTER

#define CBSV_CLUSTERS 4 //кол-во кластеров по умолчанию

#include <cstdint>
#include <vector>
#include <thread>
#include <cstring>

#include "cbsv/sv_user.h"
#include "cbsv/sh_packet.h"

namespace cbsv{
	static uint16_t cluster_counter;
	static uint8_t cluster_num = CBSV_CLUSTERS;
	
	extern "C" struct UserID{
		uint16_t cluster_id, user_id;
		bool allocated; //назначен ли юзер в кластер
	};
	
	extern "C" bool AddrComp(IPaddress* ip1, IPaddress* ip2);

	extern "C" struct UserCluster{ //группа клиентов, которая обрабатывается в своем выделенном потоке
		UserCluster(uint16_t size);
		~UserCluster();
		
		int32_t AddUser(User* us);
		User* GetUser(uint16_t userid);
		void RemoveUser(User* us, bool disconnect = true);
		int32_t HasUser(User* us);
		
		uint16_t GetFreeSlots();
		
		void TickThread();
		
		void _allocate();
		void _free();
		
		uint16_t usercount, clusterid;
		User** user_array;
		bool allocated;
	};
	
	static UserCluster** cluster_arr = nullptr;
	static bool clusters_alloc = false;
	
	void AllocateClusters(uint8_t numclusters, uint16_t csize);
	void ClearClusters();
	
	UserID AddUser(User* us);
}

#endif