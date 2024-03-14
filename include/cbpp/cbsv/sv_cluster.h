/*
На сервере все пользователи разбиты на группы, которые называются кластерами.
*/

#ifndef CBSV_SV_CLUSTER
#define CBSV_SV_CLUSTER

#include <cstdint>
#include "cbsv/sv_user.h"

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
		
		uint16_t usercount, clusterid;
		User** user_array;
		bool allocated;
	};
}

#endif