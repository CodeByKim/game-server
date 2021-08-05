#include "./MessageHandler/RpgGameMessageHandler.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"

RpgGameMessageHandler::RpgGameMessageHandler()
{
	
}

RpgGameMessageHandler::~RpgGameMessageHandler()
{
}

void RpgGameMessageHandler::OnClientJoin(garam::net::ClientInfo* info)
{
	LOG_INFO(L"Game") << L"On New Client : " << info->GetID();
		
	mGameLogic.AddNewPlayer(info);
}

void RpgGameMessageHandler::OnClientLeave(garam::net::ClientInfo* info)
{
	LOG_INFO(L"Game") << L"On Leave Client : " << info->GetID();
}

void RpgGameMessageHandler::OnPacketReceive(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{		
	short protocol;	
	*packet >> protocol;

	switch (protocol)
	{
	case PACKET_CS_PLAYER_MOVE_START:
		PacketPlayerMoveStart(info, packet);
		break;

	case PACKET_CS_PLAYER_MOVE_END:
		PacketPlayerMoveEnd(info, packet);
		break;	
	}
}

void RpgGameMessageHandler::OnUpdate(float deltaTime)
{		
	mGameLogic.Update(deltaTime);
}

#pragma region Packet Func
void RpgGameMessageHandler::PacketPlayerMoveStart(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	char dir;
	short x;
	short y;
	*packet >> dir >> x >> y;

	mGameLogic.PlayerMoveStart(info->GetID(), dir, x, y);
}

void RpgGameMessageHandler::PacketPlayerMoveEnd(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	char dir;
	short x;
	short y;
	*packet >> dir >> x >> y;

	mGameLogic.PlayerMoveEnd(info->GetID(), dir, x, y);
}
#pragma endregion