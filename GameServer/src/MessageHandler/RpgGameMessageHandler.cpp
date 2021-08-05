#include "./MessageHandler/RpgGameMessageHandler.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"

//float speed = 20;
//
//class Player
//{
//public:
//	Player()
//	{
//		mIsMoving = false;
//		mX = 0;
//		mZ = 0;
//	}
//
//	bool mIsMoving;
//	float mX;
//	float mZ;
//};
//
//Player* gPlayer = nullptr;

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
	/*case CS_PLAYER_MOVE_START:
		PacketPlayerMoveStart(client, packet);
		break;

	case CS_PLAYER_MOVE_END:
		PacketPlayerMoveEnd(client, packet);
		break;*/
	}
}

void RpgGameMessageHandler::OnUpdate(float deltaTime)
{		
	mGameLogic.Update(deltaTime);
}

#pragma region Packet Func
void RpgGameMessageHandler::PacketPlayerMoveStart(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	/*float x;
	float z;
	*packet >> x >> z;

	gPlayer->mIsMoving = true;*/
}

void RpgGameMessageHandler::PacketPlayerMoveEnd(garam::net::ClientInfo* info, garam::net::NetPacket* packet)
{
	/*float x;
	float z;
	*packet >> x >> z;

	gPlayer->mIsMoving = false;

	garam::net::NetPacket* sendPacket = garam::net::NetPacket::Alloc();
	short protocol = SC_PLAYER_MOVE_END;
	*sendPacket << protocol << gPlayer->mX << gPlayer->mZ;

	client->SendPacket(sendPacket);
	garam::net::NetPacket::Free(sendPacket);*/
}
#pragma endregion