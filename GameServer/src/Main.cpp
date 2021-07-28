#pragma comment(lib, "ServerLib")

#include "NetworkLib.h"
#include "EchoMessageHandler.h"

#define SERVER_PORT 6000
#define MAX_CCU 3000

int main()
{			
	//garam::net::NetServer* server = garam::net::NetServer::Create(SERVER_PORT, MAX_CCU);
	garam::net::GameServer* server = garam::net::GameServer::Create(SERVER_PORT, MAX_CCU);
	server->RegisterMessageHandler(new EchoMessageHandler());

	server->Run();
}