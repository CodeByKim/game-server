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

	mSectors[sectorY][sectorX].monsters.push_back(monster);
	monster->SetSectorPosition(sectorX, sectorY);
}

void RpgGameWorld::DeadMonster(Monster* monster)
{
	//섹터에서도 제거해야 함...
	garam::net::Sector* sector = GetSector(monster);
	sector->monsters.remove(monster);

	mMonsterManager.DeadMonster(monster);
}

void RpgGameWorld::OnPlayerJoin(garam::net::BasePlayer* player, std::vector<garam::net::BasePlayer*>& otherPlayers, std::vector<garam::net::Entity*>& otherMonsters)
{
	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position2D& playerPos = player->GetPosition();

	SEND_CREATE_MY_PLAYER(*player->GetClientInfo(),
						  id,
						  dir,
						  playerPos.x,
						  playerPos.y);

	BROADCAST_CREATE_OTHER_PLAYER(*this,
								  id,
								  dir,
								  playerPos.x,
								  playerPos.y,
								  player);

	for (auto iter = otherPlayers.begin();
		iter != otherPlayers.end();
		++iter)
	{
		garam::net::BasePlayer* otherPlayer = *iter;

		int id = otherPlayer->GetID();
		BYTE dir = otherPlayer->GetDirection();
		Position2D& playerPos = otherPlayer->GetPosition();

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

	for (auto iter = otherMonsters.begin();
		iter != otherMonsters.end();
		++iter)
	{
		garam::net::Entity* monster = *iter;

		SEND_CREATE_MONSTER(*player->GetClientInfo(),
							monster->GetID(),
							monster->GetDirection(),
							monster->GetPosition().x,
							monster->GetPosition().y);

		/*
		 * 이 몬스터가 이동중이었다면
		 * 이동중이라는 것을 알려야 함
		 */
		 /*if (monster->IsMove())
		 {
			 SEND_PLAYER_MOVE_START(*player->GetClientInfo(),
				 id,
				 dir,
				 playerPos.x,
				 playerPos.y);
		 }*/
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

			//OnOtherPlayerLeaveSectorRange(otherPlayer);
			//OnPlayerLeaveSector()
		}

		std::list<garam::net::Entity*>& monsters = leaveSector->monsters;

		for (auto iter = monsters.begin();
			iter != monsters.end();
			++iter)
		{
			garam::net::Entity* monster = *iter;

			monster->OnLeaveSectorOtherPlayer(player);
			//OnOtherMonsterLeaveSectorRange(monster);
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

			//OnOtherPlayerEnterSectorRange(otherPlayer);
			otherPlayer->OnEnterSectorOtherPlayer(player);
		}

		std::list<garam::net::Entity*>& monsters = enterSector->monsters;

		for (auto iter = monsters.begin();
			iter != monsters.end();
			++iter)
		{
			garam::net::Entity* monster = *iter;

			//OnOtherMonsterEnterSectorRange(monster);
			monster->OnEnterSectorOtherPlayer(player);
		}
	}
}
