#pragma comment(lib, "ServerLib")

#include "NetworkLib.h"
#include "EchoMessageHandler.h"

#define SERVER_PORT 6000
#define MAX_CCU 3000

void init_logger()
{
	Log::Configure networkLogConfig;
	networkLogConfig.SetLoggerName(L"Network");
	networkLogConfig.SetWriter(Log::eLogWriter::Console);	
	networkLogConfig.SetWriter(Log::eLogWriter::File);

	Log::Configure gameLogConfig;
	gameLogConfig.SetLoggerName(L"Game");
	gameLogConfig.SetWriter(Log::eLogWriter::Console);
	gameLogConfig.SetWriter(Log::eLogWriter::File);

	Log::Manager::Create(&networkLogConfig);
	Log::Manager::Create(&gameLogConfig);
}

int main()
{			
	init_logger();

	LOG_INFO(L"Network") << 1;
	LOG_ERROR(L"Game") << 2;
	LOG_INFO(L"Network") << 3;

	//garam::net::NetServer server(SERVER_PORT, MAX_CCU);
	garam::net::GameServer server(SERVER_PORT, MAX_CCU);	
	server.RegisterMessageHandler(new EchoMessageHandler());

	server.Run();
}