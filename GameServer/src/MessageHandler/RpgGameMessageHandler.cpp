#include "./MessageHandler/RpgGameMessageHandler.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"
#include "./Contents/RPGGameLogic.h"

RpgGameMessageHandler::RpgGameMessageHandler(RPGGameLogic* logic)
	: mGameLogic(logic)
{
}

RpgGameMessageHandler::~RpgGameMessageHandler()
{
}

void RpgGameMessageHandler::OnClientJoin(garam::net::ClientInfo* info)
{
	LOG_INFO(L"Game") << L"On New Client : " << info->GetID();		
}

void RpgGameMessageHandler::OnClientLeave(garam::net::ClientInfo* info)
{
	LOG_INFO(L"Game") << L"On Leave Client : " << info->GetID();

	mGameLogic->LeavePlayer(info);
}

void RpgGameMessageHandler::OnPacketReceive(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{		
	short protocol;	
	*packet >> protocol;

	switch (protocol)
	{
	case PACKET_CS_CREATE_MY_PLAYER:
		mGameLogic->AddNewPlayer(info);
		break;

	case PACKET_CS_PLAYER_MOVE_START:
		PacketPlayerMoveStart(info, packet);
		break;

	case PACKET_CS_PLAYER_MOVE_END:
		PacketPlayerMoveEnd(info, packet);
		break;

	case PACKET_CS_PLAYER_ATTACK:
		PacketPlayerAttack(info, packet);
		break;

	case PACKET_CS_TELEPORT_PLAYER:
		PacketTeleportPleyer(info, packet);
		break;

	case PACKET_CS_CREATE_DUMMY_PLAYER:
		mGameLogic->AddNewPlayer(info, true);
		break;
	}
}

void RpgGameMessageHandler::OnUpdate(float deltaTime)
{			
}

#pragma region Packet Func
void RpgGameMessageHandler::PacketPlayerMoveStart(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	BYTE dir;
	float x;
	float y;
	*packet >> dir >> x >> y;

	mGameLogic->PlayerMoveStart(info->GetID(), dir, x, y);
}

void RpgGameMessageHandler::PacketPlayerMoveEnd(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	BYTE dir;
	float x;
	float y;
	*packet >> dir >> x >> y;

	mGameLogic->PlayerMoveEnd(info->GetID(), dir, x, y);
}

void RpgGameMessageHandler::PacketPlayerAttack(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	BYTE dir;
	float x;
	float y;
	*packet >> dir >> x >> y;

	mGameLogic->PlayerAttack(info->GetID(), dir, x, y);
}

void RpgGameMessageHandler::PacketTeleportPleyer(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	/*BYTE dir;
	float x;
	float y;
	*packet >> dir >> x >> y;

	mGameLogic->TeleportPlayer(info->GetID(), dir, x, y);*/
}
#pragma endregion