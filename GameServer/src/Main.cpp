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

int main()
{				
	init_logger();
	
	//TODO : 임시로 그냥 이렇게...
	RPGGameLogic* logic = new RPGGameLogic();
	RpgGameMessageHandler* handler = new RpgGameMessageHandler();
	handler->mGameLogic = logic;

	//garam::net::NetServer server(SERVER_PORT, MAX_CCU);	
	garam::net::GameServer server(SERVER_PORT, MAX_CCU, handler, logic);	
	//server.RegisterMessageHandler(new EchoMessageHandler());	
	
	LOG_INFO(L"Game") << L"Run Server";
	server.Run();
} 