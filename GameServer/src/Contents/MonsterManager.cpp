#include "./Contents/MonsterManager.h"
#include "./Contents/World.h"
#include "./Common/Protocol.h"
#include "./Contents/BasePlayer.h"

MonsterManager::MonsterManager(World* world)
	: mRespawnJob(this)
	, mWorld(world)
{
	for (int i = 0; i < MAX_MONSTER_COUNT; i++)
	{				
		mMonsters[i] = mMonsterPool.Alloc();		
		mMonsters[i]->Initialize(i);
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

// JobSystem으로 멀티스레딩으로 돌 수 있음...
// 혹시 공유변수에 접근하면 락을 걸자.
void MonsterManager::Reswapn()
{	
	srand((unsigned int)time(NULL));

	int id = mEmptyMonsterIndex.top();
	mEmptyMonsterIndex.pop();
	
	Monster* spawnMonster = mMonsterPool.Alloc();
	mMonsters[id] = spawnMonster;

	spawnMonster->Initialize(id);
	
	//섹터에 넣고	
	mWorld->AddMonster(spawnMonster);

	//몬스터 생성 브로드캐스팅
	std::vector<Sector*> aroundSectors;
	mWorld->GetAroundSector(spawnMonster, &aroundSectors);

	for (auto iter = aroundSectors.begin(); 
		 iter != aroundSectors.end(); 
		 ++iter)
	{
		Sector* sector = *iter;		
		std::list<BasePlayer*>& players = sector->players;

		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			BasePlayer* player = *iter;

			SEND_CREATE_MONSTER(*player->GetClientInfo(), 
								spawnMonster->GetID(), 
								spawnMonster->GetDirection(), 								
								spawnMonster->GetPosition().x, 
								spawnMonster->GetPosition().y);
		}
	}	
	
	LOG_INFO(L"Game") << L"Respawn Monster : " << 
						 spawnMonster->GetPosition().x << 
						 L", " << 
						 spawnMonster->GetPosition().y;
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
