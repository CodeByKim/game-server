#include "RpgGameMessageHandler.h"

float speed = 20;

class Player
{
public:
	Player()
	{
		mIsMoving = false;
		mX = 0;
		mZ = 0;
	}

	bool mIsMoving;
	float mX;
	float mZ;
};

Player* gPlayer = nullptr;

RpgGameMessageHandler::RpgGameMessageHandler()
{
	
}

RpgGameMessageHandler::~RpgGameMessageHandler()
{
}

void RpgGameMessageHandler::OnClientJoin(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On New Client : " << client->GetID();
	gPlayer = new Player();
}

void RpgGameMessageHandler::OnClientLeave(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On Leave Client : " << client->GetID();
}

void RpgGameMessageHandler::OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{		
	short protocol;	
	*packet >> protocol;

	switch (protocol)
	{
	case CS_PLAYER_MOVE_START:
		PacketPlayerMoveStart(client, packet);
		break;

	case CS_PLAYER_MOVE_END:
		PacketPlayerMoveEnd(client, packet);
		break;
	}
}

void RpgGameMessageHandler::OnUpdate(float deltaTime)
{	
	//LOG_INFO(L"Game") << deltaTime;

	if (gPlayer != nullptr)
	{
		//무조건 오른쪽으로만 이동한다고 하자.. 원래는 방향도 같이 가야하겠지만...
		if (gPlayer->mIsMoving)
		{			
			gPlayer->mX += speed * deltaTime;
		}
	}
}

#pragma region Packet Func
void RpgGameMessageHandler::PacketPlayerMoveStart(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{
	float x;
	float z;
	*packet >> x >> z;

	gPlayer->mIsMoving = true;
}

void RpgGameMessageHandler::PacketPlayerMoveEnd(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{
	float x;
	float z;
	*packet >> x >> z;

	gPlayer->mIsMoving = false;

	garam::net::NetPacket* sendPacket = garam::net::NetPacket::Alloc();
	short protocol = SC_PLAYER_MOVE_END;
	*sendPacket << protocol << gPlayer->mX << gPlayer->mZ;

	client->SendPacket(sendPacket);
	garam::net::NetPacket::Free(sendPacket);
}
#pragma endregion