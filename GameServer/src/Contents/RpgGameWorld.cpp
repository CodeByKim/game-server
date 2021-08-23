#include "./Contents/RpgGameWorld.h"
#include "./Contents/Monster.h"
#include "./Common/Protocol.h"

RpgGameWorld::RpgGameWorld()
	: mMonsterManager(this)
{
}

RpgGameWorld::~RpgGameWorld()
{
}

void RpgGameWorld::OnUpdate(float deltaTime)
{
	World::OnUpdate(deltaTime);

	mMonsterManager.OnUpdate(deltaTime);
}

void RpgGameWorld::SpawnMonster(int count)
{
	for (int i = 0; i < count; i++)
	{
		AddMonster(mMonsterManager.GetMonster(i));
	}
}

void RpgGameWorld::AddMonster(Monster* monster)
{
	Position2D& monsterPos = monster->GetPosition();
	int sectorX = (int)(monsterPos.x / mSectorSize);
	int sectorY = (int)(monsterPos.y / mSectorSize);

	mSectors[sectorY][sectorX].entities.push_back(monster);
	monster->SetSectorPosition(sectorX, sectorY);
}

void RpgGameWorld::DeadMonster(Monster* monster)
{	
	garam::net::Sector* sector = GetSector(monster);
	sector->entities.remove(monster);

	mMonsterManager.DeadMonster(monster);
}

void RpgGameWorld::OnPlayerJoin(garam::net::BasePlayer* player, std::vector<garam::net::Entity*>& otherEntities)
{
	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position2D& playerPos = player->GetPosition();

	SEND_CREATE_MY_PLAYER(*player->GetClientInfo(),
						  id,
						  dir,
						  playerPos.x,
						  playerPos.y);

	for (auto iter = otherEntities.begin();
		 iter != otherEntities.end();
		 ++iter)
	{
		garam::net::Entity* entity = *iter;

		entity->OnEnterSectorOtherPlayer(player);
	}
}

void RpgGameWorld::OnPlayerLeave(garam::net::BasePlayer* leavePlayer)
{
	BROADCAST_REMOVE_OTHER_PLAYER(*this,
								  leavePlayer->GetID(),
								  leavePlayer);
}

void RpgGameWorld::ProcessLeaveSector(garam::net::BasePlayer* player, std::vector<garam::net::Sector*>& leaveSectors)
{
	for (int i = 0; i < leaveSectors.size(); i++)
	{
		garam::net::Sector* leaveSector = leaveSectors[i];
		std::list<garam::net::BasePlayer*>& players = leaveSector->players;
		
		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			garam::net::BasePlayer* otherPlayer = *iter;

			if (otherPlayer->GetID() == player->GetID())
				continue;

			otherPlayer->OnLeaveSectorOtherPlayer(player);			
		}

		std::list<garam::net::Entity*>& otherEntities = leaveSector->entities;

		for (auto iter = otherEntities.begin();
			 iter != otherEntities.end();
			 ++iter)
		{
			garam::net::Entity* entity = *iter;

			entity->OnLeaveSectorOtherPlayer(player);
		}
	}
}

void RpgGameWorld::ProcessNewEnterSector(garam::net::BasePlayer* player, std::vector<garam::net::Sector*>& enterSectors)
{
	for (int i = 0; i < enterSectors.size(); i++)
	{
		garam::net::Sector* enterSector = enterSectors[i];
		std::list<garam::net::BasePlayer*>& players = enterSector->players;

		for (auto iter = players.begin();
			iter != players.end();
			++iter)
		{
			garam::net::BasePlayer* otherPlayer = *iter;

			if (otherPlayer->GetID() == player->GetID())
				continue;
			
			otherPlayer->OnEnterSectorOtherPlayer(player);
		}

		std::list<garam::net::Entity*>& otherEntities = enterSector->entities;

		for (auto iter = otherEntities.begin();
			 iter != otherEntities.end();
			 ++iter)
		{
			garam::net::Entity* entity = *iter;
			
			entity->OnEnterSectorOtherPlayer(player);
		}
	}
}
