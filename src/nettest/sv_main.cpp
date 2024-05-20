#include <iostream>
#include <cstring>

#include "cbpp.h"
#include "SDL2/SDL_Net.h"

using namespace std;

int main(int argc, char* argv[]){
	int port = atoi(argv[1]);
	
	cout<<"Sv port "<<port<<endl;
	
	SDLNet_Init();
	
	UDPsocket sock = SDLNet_UDP_Open(port);
	
	UDPpacket* packet = SDLNet_AllocPacket(1024);
	
	while(1){
		int res = SDLNet_UDP_Recv(sock, packet);
		
		if(res){
			cout<<"Got packet!"<<endl;
			cout<<"\tPacket length: "<<packet->len<<endl;
			cout<<"\tPacket data: "<<packet->data<<endl;
			memset(packet->data, 0, 1024);
		}
	}
	
	return 0;
}