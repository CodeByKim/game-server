#pragma once
#include <NetworkLib.h>

#include "Common/GameCommon.h"
#include "./Contents/Monster.h"

#define MAX_MONSTER_COUNT 10000

class MonsterManager;
class RpgGameWorld;

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
	MonsterManager(RpgGameWorld* world);
	~MonsterManager();

	void OnUpdate(float deltaTime);
	void DeadMonster(Monster* monster);
	void Reswapn();
	Monster* GetMonster(int id);

private:
	garam::memory::MemoryPool<Monster> mMonsterPool;
	MonsterRespawnJob mRespawnJob;
	RpgGameWorld* mWorld;
	Monster* mMonsters[MAX_MONSTER_COUNT];
	std::stack<int> mEmptyMonsterIndex;
};