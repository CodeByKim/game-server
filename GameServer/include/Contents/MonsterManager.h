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

	//어차피 최대 몬스터의 갯수는 정해져 있고
	//몬스터가 죽으면 해당 인덱스의 몬스터를 그냥 비활성화 시키는 방법으로 가자
	//Monster Manager를 만들지 안만들지는 고민.. 만드는게 좋긴 하겟다.
	Monster* mMonsters[MAX_MONSTER_COUNT];
	std::stack<int> mEmptyMonsterIndex;
};