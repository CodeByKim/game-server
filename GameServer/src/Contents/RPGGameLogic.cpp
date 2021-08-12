#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"
#include "./Contents/World.h"

RPGGameLogic::RPGGameLogic()
{
	mWorld.Create(50, 50, 10);	
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{		
	mWorld.Update();
	
	for (auto iter = mPlayers.begin(); 
		 iter != mPlayers.end(); 
		 ++iter)
	{				
		Player* player = iter->second;
		player->OnUpdate(deltaTime);
	}

	for (auto iter = mDeletedPlayers.begin(); 
		 iter != mDeletedPlayers.end(); 
		 ++iter)
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
	
	SEND_CREATE_MY_PLAYER(*player->GetClientInfo(), 
						  id, 
						  dir, 
						  playerPos.x, 
						  playerPos.y);

	SendPlayerInfoContainedInSector(player);

	BROADCAST_CREATE_OTHER_PLAYER(mWorld, 
								  id, 
								  dir, 
								  playerPos.x, 
								  playerPos.y, 
								  player);
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
	
	BROADCAST_REMOVE_OTHER_PLAYER(mWorld, 
								  player->GetID(), 
								  player);
}

void RPGGameLogic::PlayerMoveStart(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);
		
	BROADCAST_PLAYER_MOVE_START(mWorld, 
								player->GetID(), 
								player->GetDirection(), 
								player->GetPosition().x, 
								player->GetPosition().y, 
								player);
}

void RPGGameLogic::PlayerMoveEnd(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveEnd(dir, x, y);
			
	BROADCAST_PLAYER_MOVE_END(mWorld, 
							  player->GetID(), 
							  player->GetDirection(), 
							  player->GetPosition().x, 
							  player->GetPosition().y, 
							  player);
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { (float)(rand() % 450), (float)(rand() % 450) };
	player->Initialize(client, pos, this);

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
			
			SEND_CREATE_OTHER_PLAYER(*player->GetClientInfo(), 
									 id, 
									 dir, 
									 playerPos.x, 
									 playerPos.y);

			/*
			 * 생성한 클라가 이동중이었다면
			 * 이동중이라는 것을 알려야 함
			 */
			if (otherPlayer->IsMove())
			{
				SEND_PLAYER_MOVE_START(*player->GetClientInfo(),
									   id,
									   dir,
									   playerPos.x,
									   playerPos.y);
			}
		}
	}
}