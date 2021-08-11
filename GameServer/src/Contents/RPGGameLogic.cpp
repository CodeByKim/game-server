#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"
#include "./Contents/World.h"

RPGGameLogic::RPGGameLogic()
{
	
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{
	mWorld.Update();

	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{				
		Player* player = iter->second;
		player->OnUpdate(deltaTime);
	}

	for (auto iter = mDeletedPlayers.begin(); iter != mDeletedPlayers.end(); ++iter)
	{
		int id = (*iter)->GetID();
		mPlayers.erase(id);
	}

	mDeletedPlayers.clear();
}

void RPGGameLogic::AddNewPlayer(garam::net::ClientInfo* info)
{
	Player* player = CreatePlayer(info);
	mPlayers.insert(std::pair(info->GetID(), player));
	mWorld.AddPlayer(player);

	SendCreateMyPlayer(player);	
	SendExistingPlayerInfo(player);	
	BroadcastCreateOtherPlayer(player);
}

void RPGGameLogic::LeavePlayer(garam::net::ClientInfo* info)
{
	/*
	 * 우선 mPlayers에서 삭제, 
	 * 현재 접속중인 플레이어에 info에 해당하는 유저가 삭제됬다고 알려줘야 함
	 */
	Player* player = GetPlayer(info->GetID());
	mDeletedPlayers.push_back(player);
	mWorld.RemovePlayer(player);

	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
	int id = player->GetID();	

	*packet << protocol << id;
	mWorld.Broadcast(packet, player);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::PlayerMoveStart(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);

	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;	
	*packet << protocol << id << dir << x << y;
	mWorld.Broadcast(packet, player);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::PlayerMoveEnd(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveEnd(dir, x, y);
	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;	
	*packet << protocol << id << dir << x << y;
	mWorld.Broadcast(packet, player);
	garam::net::NetPacket::Free(packet);
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

void RPGGameLogic::SendCreateMyPlayer(Player* player)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_CREATE_MY_PLAYER;
	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();

	*packet << protocol << id << dir << playerPos.x << playerPos.y;	
	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::SendExistingPlayerInfo(Player* player)
{
	std::vector<Sector*> aroundSectors;
	mWorld.GetAroundSector(player, &aroundSectors);

	for (auto iter = aroundSectors.begin(); 
		 iter != aroundSectors.end(); 
		 ++iter)
	{
		Sector* sector = *iter;
		auto players = sector->players;

		for (auto iter = players.begin(); 
			 iter != players.end(); 
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (player->GetID() == otherPlayer->GetID())
			{
				continue;
			}

			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

			short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
			int id = otherPlayer->GetID();
			BYTE dir = otherPlayer->GetDirection();
			Position playerPos = otherPlayer->GetPosition();

			*packet << protocol << id << dir << playerPos.x << playerPos.y;
			player->GetClientInfo()->SendPacket(packet);
			garam::net::NetPacket::Free(packet);
		}
	}
}

void RPGGameLogic::BroadcastCreateOtherPlayer(Player* player)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();

	*packet << protocol << id << dir << playerPos.x << playerPos.y;
	mWorld.Broadcast(packet, player);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveStart(Player* player)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_PLAYER_MOVE_START;
	int id = player->GetClientInfo()->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();	

	*packet << protocol << id << dir << playerPos.x << playerPos.y;	
	mWorld.Broadcast(packet, player);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveEnd(Player* player)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_PLAYER_MOVE_END;	
	int id = player->GetClientInfo()->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();

	*packet << protocol << id << dir << playerPos.x << playerPos.y;
	mWorld.Broadcast(packet, player);
	garam::net::NetPacket::Free(packet);
}
