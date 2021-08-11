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

	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();

	SendCreateMyPlayer(player, id, dir, playerPos.x, playerPos.y);	
	SendPlayerInfoContainedInSector(player);
	BroadcastCreateOtherPlayer(id, dir, playerPos.x, playerPos.y, player);
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

	BroadcastRemovePlayer(player->GetID(), player);
}

void RPGGameLogic::PlayerMoveStart(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);
	
	BroadcastPlayerMoveStart(id, dir, x, y, player);
}

void RPGGameLogic::PlayerMoveEnd(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveEnd(dir, x, y);
		
	BroadcastPlayerMoveEnd(id, dir, x, y, player);
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { (float)(rand() % 15), (float)(rand() % 15) };
	player->Initialize(client, pos, this);

	return player;
}

Player* RPGGameLogic::GetPlayer(int id)
{
	if (!IsContainPlayer(id))
		return nullptr;

	return mPlayers[id];
}

void RPGGameLogic::SendRemovePlayer(Player* player, int id)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
	*packet << protocol << id;

	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::SendPlayerMoveStart(Player* player, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;
	*packet << protocol << id << dir << x << y;

	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

bool RPGGameLogic::IsContainPlayer(int id)
{
	if (mPlayers.find(id) == mPlayers.end())
		return false;

	return true;
}

void RPGGameLogic::SendCreateMyPlayer(Player* player, int id, BYTE dir, float x, float y)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MY_PLAYER;	
	*packet << protocol << id << dir << x << y;

	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::SendPlayerInfoContainedInSector(Player* player)
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

			int id = otherPlayer->GetID();
			BYTE dir = otherPlayer->GetDirection();
			Position playerPos = otherPlayer->GetPosition();

			SendCreateOtherPlayer(player, id, dir, playerPos.x, playerPos.y);
		}
	}
}

void RPGGameLogic::SendCreateOtherPlayer(Player* player, int id, BYTE dir, float x, float y)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;	
	*packet << protocol << id << dir << x << y;

	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastCreateOtherPlayer(int id, BYTE dir, float x, float y, Player* exceptPlayer)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;	
	*packet << protocol << id << dir << x << y;

	mWorld.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveStart(int id, BYTE dir, float x, float y, Player* exceptPlayer)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;	
	*packet << protocol << id << dir << x << y;	

	mWorld.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveEnd(int id, BYTE dir, float x, float y, Player* exceptPlayer)
{	
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;		
	*packet << protocol << id << dir << x << y;

	mWorld.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastRemovePlayer(int id, Player* exceptPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_DELETE_OTHER_PLAYER;	
	*packet << protocol << id;

	mWorld.Broadcast(packet, exceptPlayer);
	garam::net::NetPacket::Free(packet);
}
