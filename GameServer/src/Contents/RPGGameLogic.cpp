#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"

RPGGameLogic::RPGGameLogic()
{
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{
	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		Player* player = iter->second;
		player->OnUpdate(deltaTime);
	}

	for (auto iter = mDeletedPlayers.begin(); iter != mDeletedPlayers.end(); ++iter)
	{
		//int id = (*iter)->GetID();
		//mPlayers.erase(id);
	}

	mDeletedPlayers.clear();
}

void RPGGameLogic::AddNewPlayer(garam::net::ClientInfo* info)
{
	Player* player = CreatePlayer(info);
	mPlayers.insert(std::pair(info->GetID(), player));

	SendCreateMyPlayer(player);
	SendExistingPlayerInfo(player);
	BroadcastCreateOtherPlayer(player);
}

void RPGGameLogic::PlayerMoveStart(int id, char dir, short x, short y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);

	BroadcastPlayerMoveStart(player);
}

void RPGGameLogic::PlayerMoveEnd(int id, char dir, short x, short y)
{	
	Player* player = GetPlayer(id);
	player->MoveEnd(dir, x, y);

	BroadcastPlayerMoveEnd(player);
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { (float)(rand() % 15), (float)(rand() % 15) };
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

void RPGGameLogic::BroadcastPacket(garam::net::NetPacket* packet, garam::net::ClientInfo* exceptClient)
{	
	for(auto iter = mPlayers.begin() ; iter != mPlayers.end() ; ++iter)
	{		
		garam::net::ClientInfo* info = iter->second->GetClientInfo();
		
		if (info->GetID() == exceptClient->GetID())
		{
			continue;
		}

		info->SendPacket(packet);
	}
}

void RPGGameLogic::SendCreateMyPlayer(Player* player)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MY_PLAYER;
	char dir = MOVE_DIR_DOWN;
	int id = player->GetID();
	Position playerPos = player->GetPosition();
	*packet << protocol << id << dir << playerPos.x << playerPos.y;

	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::SendExistingPlayerInfo(Player* player)
{
	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{		
		Player* otherPlayer = iter->second;

		if (player->GetID() == otherPlayer->GetID())
		{
			continue;
		}

		garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
		int id = otherPlayer->GetID();
		char dir = otherPlayer->GetDirection();
		Position playerPos = otherPlayer->GetPosition();
		*packet << protocol << id << playerPos.x << playerPos.y;

		player->GetClientInfo()->SendPacket(packet);
		garam::net::NetPacket::Free(packet);
	}	
}

void RPGGameLogic::BroadcastCreateOtherPlayer(Player* player)
{
	//TODO : 나중에는 Sector만 전송해야 함
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
	int id = player->GetClientInfo()->GetID();
	char dir = player->GetDirection();
	Position playerPos = player->GetPosition();		
	*packet << protocol << id << dir << playerPos.x << playerPos.y;

	//garam::net::NetworkComponent::BroadCast(packet, player->GetClientInfo());
	BroadcastPacket(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveStart(Player* player)
{
	/*	 
	 * TODO : 나중에는 Sector만 전송해야 함
	 */		 
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;
	int id = player->GetClientInfo()->GetID();
	char dir = player->GetDirection();
	Position playerPos = player->GetPosition();	
	*packet << protocol << id << dir << playerPos.x << playerPos.y;

	//garam::net::NetworkComponent::BroadCast(packet, player->GetClientInfo());
	BroadcastPacket(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveEnd(Player* player)
{
	/*
	 * TODO : 나중에는 Sector만 전송해야 함
	 */
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;	
	int id = player->GetClientInfo()->GetID();
	char dir = player->GetDirection();
	Position playerPos = player->GetPosition();
	*packet << protocol << id << dir << playerPos.x << playerPos.y;

	//garam::net::NetworkComponent::BroadCast(packet, player->GetClientInfo());
	BroadcastPacket(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}
