#ifndef CBSV_SH_NETSTATE
#define CBSV_SH_NETSTATE

namespace cbsv{
	enum CBSV_NETSTATE {
		CBSV_CLIENT, CBSV_SERVER
	};
	
	static CBSV_NETSTATE net_status = CBSV_CLIENT; //глобальная метка, говорит нам о том, где выполняется код
	
	CBSV_NETSTATE NetStatus(){ return net_status; } //по какой-то причине подобные статические переменные нельзя поменять
	void SetNetStatus(CBSV_NETSTATE new_status){ net_status = new_status; } //или прочитать извне, поэтому нужны эти ф-ции
}

#endif