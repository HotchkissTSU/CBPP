/*
На сервере все пользователи разбиты на группы, которые называются кластерами.
*/

#ifndef CBSV_SV_CLUSTER
#define CBSV_SV_CLUSTER

#define CBSV_CLUSTERS 4

#include <cstdint>
#include <vector>
#include <thread>

#include "cbsv/sv_user.h"
#include "cbsv/sh_packet.h"

namespace cbsv{
	static uint16_t cluster_counter;

	extern "C" struct UserCluster{ //группа клиентов, которая обрабатывается в своем выделенном потоке
		UserCluster(uint16_t size);
		~UserCluster();
		
		int32_t AddUser(User* us);
		User* GetUser(uint16_t userid);
		uint16_t GetFreeSlots();
		
		void TickThread();
		
		void _allocate();
		void _free();
		
		std::vector<uint16_t> pkg_idcheck;
		
		uint16_t usercount, clusterid;
		User** user_array;
		bool allocated;
	};
	
	static UserCluster** cluster_arr;
}

#endif