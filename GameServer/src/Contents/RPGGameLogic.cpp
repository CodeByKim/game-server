#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "Components/NetworkComponent.h"

RPGGameLogic::RPGGameLogic()
{
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{
}

void RPGGameLogic::AddNewPlayer(garam::net::ClientInfo* info)
{
	Player* player = CreatePlayer(info);
	mPlayers.insert(std::pair(info->GetID(), player));

	SendCreateMyPlayer(player);
	BroadcastCreateOtherPlayer(player);
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { rand() % 100, rand() % 100 };
	player->Initialize(client, pos);

	return player;
}

Player* RPGGameLogic::GetPlayer(int id)
{
	if (!IsContainPlayer(id))
		return nullptr;

	return mPlayers[id];
}

bool RPGGameLogic::IsContainPlayer(int id)
{
	if (mPlayers.find(id) == mPlayers.end())
		return false;

	return true;
}

void RPGGameLogic::SendCreateMyPlayer(Player* player)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MY_PLAYER;
	Position playerPos = player->GetPosition();
	*packet << protocol << playerPos.x << playerPos.y;

	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastCreateOtherPlayer(Player* player)
{
	//TODO : 나중에는 Sector만 전송해야 함
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
	Position playerPos = player->GetPosition();	
	int id = player->GetClientInfo()->GetID();
	*packet << protocol << id << playerPos.x << playerPos.y;

	garam::net::NetworkComponent::BroadCast(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}
