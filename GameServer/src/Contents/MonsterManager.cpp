#include "./Contents/MonsterManager.h"
#include "./Contents/World.h"
#include "./Common/Protocol.h"
#include "./Contents/Player.h"

MonsterManager::MonsterManager(World* world)
	: mRespawnJob(this)
	, mWorld(world)
{
	for (int i = 0; i < MAX_MONSTER_COUNT; i++)
	{		
		BYTE dir = rand() % 4;

		//TODO : ���� ��ġ�� ��� �� ũ�⿡ ����ؾ��Ѵ�. World.Create�Լ����� �� ũ�⸦ ����
		Position position = Position{ (float)(rand() % 1900), (float)(rand() % 1900) };

		mMonsters[i] = mMonsterPool.Alloc();
		mMonsters[i]->Initialize(i, dir, position, 100);		
	}	
}

MonsterManager::~MonsterManager()
{

}

void MonsterManager::OnUpdate(float deltaTime)
{
	for (int i = 0; i < MAX_MONSTER_COUNT; i++)
	{
		if (mMonsters[i] == nullptr)
			continue;

		mMonsters[i]->OnUpdate(deltaTime);
	}
}

void MonsterManager::DeadMonster(Monster* monster)
{
	int id = monster->GetID();
	monster->Clear();
	mMonsterPool.Free(monster);
	mMonsters[id] = nullptr;
	mEmptyMonsterIndex.push(id);

	garam::jobsystem::JobManager::GetInstance().Register(&mRespawnJob, 3000, 1);
}

// JobSystem���� ��Ƽ���������� �� �� ����...
// Ȥ�� ���������� �����ϸ� ���� ����.
void MonsterManager::Reswapn()
{
	int id = mEmptyMonsterIndex.top();
	mEmptyMonsterIndex.pop();
	
	BYTE dir = rand() % 4;

	//TODO : ���� ��ġ�� ��� �� ũ�⿡ ����ؾ��Ѵ�. World.Create�Լ����� �� ũ�⸦ ����
	//Position position = Position{ 1, 1 };	
	Position position = Position{ (float)(rand() % 1900), (float)(rand() % 1900) };	

	Monster* spawnMonster = mMonsterPool.Alloc();
	mMonsters[id] = spawnMonster;

	spawnMonster->Initialize(id, dir, position, 100);

	//���Ϳ� �ְ�	
	mWorld->AddMonster(spawnMonster);

	//���� ���� ��ε�ĳ����
	std::vector<Sector*> aroundSectors;
	mWorld->GetAroundSector(spawnMonster, &aroundSectors);

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
			Player* player = *iter;

			SEND_CREATE_MONSTER(*player->GetClientInfo(), 
								spawnMonster->GetID(), 
								spawnMonster->GetDirection(), 
								spawnMonster->GetPosition().x, 
								spawnMonster->GetPosition().y);
		}
	}	
}

Monster* MonsterManager::GetMonster(int id)
{
	return mMonsters[id];
}

MonsterRespawnJob::MonsterRespawnJob(MonsterManager* monsterManager)
{
	mMonsterManager = monsterManager;
}

void MonsterRespawnJob::Execute()
{
	mMonsterManager->Reswapn();
}
