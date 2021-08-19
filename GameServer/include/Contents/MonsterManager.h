#pragma once
#include "Common/GameCommon.h"
#include "./Contents/Monster.h"
#include <NetworkLib.h>

#define MAX_MONSTER_COUNT 10000

class MonsterManager;
class World;

class MonsterRespawnJob : public garam::jobsystem::IJob
{
public:
	MonsterRespawnJob(MonsterManager* monsterManager);
	void Execute() override;

private:
	MonsterManager* mMonsterManager;
};

class MonsterManager
{
public:
	MonsterManager(World* world);
	~MonsterManager();

	void OnUpdate(float deltaTime);
	void DeadMonster(Monster* monster);
	void Reswapn();
	Monster* GetMonster(int id);

private:
	garam::memory::MemoryPool<Monster> mMonsterPool;
	MonsterRespawnJob mRespawnJob;
	World* mWorld;	
	Monster* mMonsters[MAX_MONSTER_COUNT];
	std::stack<int> mEmptyMonsterIndex;
};