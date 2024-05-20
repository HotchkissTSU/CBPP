#include <iostream>
#include <cstring>
#include <cstdint>
#include <thread>

#include "cbpp.h"
#include "SDL2/SDL_net.h"

using namespace std;

void Receiver(UDPsocket sock){
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
}

int main(int argc, char* argv[]){
	SDLNet_Init();
	
	char* host = argv[1];
	int port = atoi(argv[2]);
	IPaddress ip;
	
	cout<<"SV IP "<<host<<":"<<port<<endl;
	
	UDPsocket sock = SDLNet_UDP_Open(0);
	SDLNet_ResolveHost(&ip, host, port);
	
	std::cout<<"Socket: "<<sock<<endl;
	
	int channel = SDLNet_UDP_Bind(sock, -1, &ip);
	
	cout<<"Host bind channel: "<<channel<<endl;
	
	std::thread receive(Receiver, sock);
	receive.detach();
	
	char* buff = new char[1024];
	
	while( strcmp(buff, "exit") != 0 ){
		memset(buff, 0, 1024);
		scanf("%s", buff);
		
		cout<<"SENDING: "<<buff<<endl;
		
		UDPpacket* pack = SDLNet_AllocPacket(1024);
			pack->len = strlen(buff);
			pack->maxlen = 1024;
			pack->data = (uint8_t*)buff;
		SDLNet_UDP_Send(sock, pack->channel, pack);
	}
	
	delete[] buff;
	
	return 0;
}