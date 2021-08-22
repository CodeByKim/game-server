#include <NetworkLib.h>

#include "./MessageHandler/EchoMessageHandler.h"
#include "./MessageHandler/RpgGameMessageHandler.h"
#include "./Contents/RPGGameLogic.h"

#define SERVER_PORT 6000
#define MAX_CCU 8000

void init_logger()
{	
	garam::logger::Configure gameLogConfig;
	gameLogConfig.SetLoggerName(L"Game");
	gameLogConfig.SetWriter(garam::logger::eLogWriter::Console);
	
	garam::logger::Manager::Create(&gameLogConfig);	
}

void start_server()
{
	RPGGameLogic* logic = new RPGGameLogic();
	RpgGameMessageHandler* handler = new RpgGameMessageHandler(logic);

	garam::net::GameServer server(SERVER_PORT, MAX_CCU, handler, logic);
	LOG_INFO(L"Game") << L"Run Server";
	server.Run();
}

int main()
{				
	init_logger();
		
	start_server();

	//garam::net::NetServer server(SERVER_PORT, MAX_CCU);
	//server.RegisterMessageHandler(new EchoMessageHandler());
}