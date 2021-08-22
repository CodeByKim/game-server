#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"

RPGGameLogic::RPGGameLogic()
{	
	mGameWorld.Create(200, 200, 10);
	mGameWorld.SpawnMonster(MAX_MONSTER_COUNT);
	RegisterWorld(&mGameWorld);	
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::OnUpdate(float deltaTime)
{				
	GameLogic::OnUpdate(deltaTime);

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

void RPGGameLogic::AddNewPlayer(garam::net::ClientInfo* info, bool isDummy)
{
	Player* player = CreatePlayer(info);

	if (isDummy)
	{
		Position2D pos = { (float)(rand() % 1500), (float)(rand() % 1500) };

		player->GetPosition().x = pos.x;
		player->GetPosition().y = pos.y;
	}

	mPlayers.insert(std::pair(info->GetID(), player));

	mGameWorld.AddPlayer(player);
	
}

void RPGGameLogic::LeavePlayer(garam::net::ClientInfo* info)
{
	/*
	 * 우선 mPlayers에서 삭제, 
	 * 현재 접속중인 플레이어에 info에 해당하는 유저가 삭제됬다고 알려줘야 함
	 */
	Player* player = GetPlayer(info->GetID());
	mDeletedPlayers.push_back(player);
	mGameWorld.RemovePlayer(player);
}

void RPGGameLogic::PlayerMoveStart(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);
			
	CheckPlayerSyncPosition(player, x, y);

	BROADCAST_PLAYER_MOVE_START(mGameWorld,
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
				
	CheckPlayerSyncPosition(player, x, y);

	BROADCAST_PLAYER_MOVE_END(mGameWorld,
							  player->GetID(), 
							  player->GetDirection(), 
							  player->GetPosition().x, 
							  player->GetPosition().y, 
							  player);
}

void RPGGameLogic::PlayerAttack(int id, BYTE dir, float x, float y)
{
	Player* player = GetPlayer(id);
	
	//피격당한 몬스터 계산해야 함
	garam::net::Sector* sector = mGameWorld.GetSector(player);	
	std::list<garam::net::Entity*>& monsters = sector->monsters;

	Monster* hitMonster = nullptr;

	for (auto iter = monsters.begin(); 
		 iter != monsters.end(); 
		 ++iter)
	{
		garam::net::Entity* monster = *iter;
		
		Position2D playerPos = player->GetPosition();
		Position2D monsterPos = monster->GetPosition();

		// 1. 일단 거리가 가까운 놈들만 추리자.		
		Position2D diff = monsterPos - playerPos;

		if (abs(diff.x) <= 3 && abs(diff.y) <= 3)
		{
			// 2. 그리고 방향을 바라보는지 확인하자.			 			
			if (player->GetDirection() == MOVE_DIR_LEFT)
			{
				//내가 왼쪽을 바라 볼 경우
				//몬스터는 나보다 왼쪽에 있어야만 함
				if (monsterPos.x - playerPos.x < 0)
				{
					hitMonster = (Monster*)monster;
					break;
				}
			}
			else if (player->GetDirection() == MOVE_DIR_UP)
			{
				//내가 위쪽을 바라 볼 경우
				//몬스터는 나보다 위에 있어야만 함
				if (monsterPos.y - playerPos.y > 0)
				{
					hitMonster = (Monster*)monster;
					break;
				}
			}
			else if (player->GetDirection() == MOVE_DIR_RIGHT)
			{
				//내가 오른쪽을 바라 볼 경우
				//몬스터는 나보다 오른쪽에 있어야만 함
				if (monsterPos.x - playerPos.x > 0)
				{					
					hitMonster = (Monster*)monster;
					break;
				}
			}
			else if (player->GetDirection() == MOVE_DIR_DOWN)
			{
				//내가 아래쪽을 바라 볼 경우
				//몬스터는 나보다 아래에 있어야만 함
				if (monsterPos.y - playerPos.y < 0)
				{
					hitMonster = (Monster*)monster;
					break;
				}
			}
		}
	}

	if (hitMonster != nullptr)
	{		
		/*
		 * 몬스터를 찾았으니 해당 몬스터의 HP를 깎고,  
		 * 몬스터 피격 패킷을 broadcast 한다.
		 */		
		hitMonster->OnHit(DAMAGE);
		if (hitMonster->IsDead())
		{			
			BROADCAST_DEAD_MONSTER(mGameWorld,
								   hitMonster->GetID(),									
								   player);

			mGameWorld.DeadMonster(hitMonster);
		}
		else
		{
			BROADCAST_HIT_MONSTER(mGameWorld,
								  hitMonster->GetID(),
								  hitMonster->GetHP(),
								  player);
		}		
	}

	BROADCAST_PLAYER_ATTACK(mGameWorld,
							player->GetID(),
							player->GetDirection(),
							player->GetPosition().x,
							player->GetPosition().y,
							player);	
}

void RPGGameLogic::TeleportPlayer(int id, BYTE dir, float x, float y)
{	
	/*Player* player = GetPlayer(id);

	player->Teleport(dir, x, y);	
	mWorld.ChangeSectorAndNotifyMessageToPlayer(player, x, y);*/
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position2D pos = { 10, 10 };
	player->Initialize(client, pos, this);

	return player;
}

Player* RPGGameLogic::GetPlayer(int id)
{
	if (!IsContainPlayer(id))
		return nullptr;

	return mPlayers[id];
}

RpgGameWorld* RPGGameLogic::GetWorld()
{
	return &mGameWorld;
}

bool RPGGameLogic::IsContainPlayer(int id)
{
	if (mPlayers.find(id) == mPlayers.end())
		return false;

	return true;
}

void RPGGameLogic::CheckPlayerSyncPosition(Player* player, float x, float y)
{
	Position2D playerPos = player->GetPosition();
	float xOffset = abs(playerPos.x - x);
	float yOffset = abs(playerPos.y - y);

	if (xOffset >= 1 || yOffset >= 1)
	{
		BROADCAST_SYNC_POSITION(mGameWorld,
								player->GetID(),								
								playerPos.x,
								playerPos.y, 
								player);

		//printf("SYNC !!! %f, %f\n", xOffset, yOffset);
	}
}
