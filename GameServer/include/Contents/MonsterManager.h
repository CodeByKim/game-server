#pragma once
#include "Common/GameCommon.h"
#include "./Contents/Monster.h"

#define MAX_MONSTER_COUNT 1500

class MonsterManager
{
public:
	MonsterManager();
	~MonsterManager();

	void OnUpdate(float deltaTime);
	Monster* GetMonster(int id);

private:
	garam::memory::MemoryPool<Monster> mMonsterPool;

	//어차피 최대 몬스터의 갯수는 정해져 있고
	//몬스터가 죽으면 해당 인덱스의 몬스터를 그냥 비활성화 시키는 방법으로 가자
	//Monster Manager를 만들지 안만들지는 고민.. 만드는게 좋긴 하겟다.
	Monster* mMonsters[MAX_MONSTER_COUNT];
	std::stack<int> mEmptyMonsterIndex;
};

