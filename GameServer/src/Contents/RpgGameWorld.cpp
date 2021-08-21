#include "./Contents/RpgGameWorld.h"
#include "./Contents/Monster.h"

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
