#include "cbpp.h"
#include <iostream>
#include <windows.h>

#include "cb_main/settings.h"

using namespace cbpp;

int main(){
	SDLNet_Init();
	
	cbsv::RunServer(228);
	
	while(1){
		Sleep(1000);
	}
}