#pragma comment(lib, "ServerLib")

#include <NetworkLib.h>
#include "./MessageHandler/EchoMessageHandler.h"
#include "./MessageHandler/RpgGameMessageHandler.h"
#include "./Contents/RpgGameWorld.h"

#define SERVER_PORT 6000
#define MAX_CCU 8000

void init_logger()
{	
	garam::logger::Configure gameLogConfig;
	gameLogConfig.SetLoggerName(L"Game");
	gameLogConfig.SetWriter(garam::logger::eLogWriter::Console);
	
	garam::logger::Manager::Create(&gameLogConfig);	
}

int main()
{				
	init_logger();
	
	//garam::net::NetServer server(SERVER_PORT, MAX_CCU);	
	garam::net::GameServer server(SERVER_PORT, MAX_CCU);	
	//server.RegisterMessageHandler(new EchoMessageHandler());
	//server.RegisterGameWorld(new RpgGameWorld());
	server.RegisterMessageHandler(new RpgGameMessageHandler());

	LOG_INFO(L"Game") << L"Run Server";
	server.Run();
} 