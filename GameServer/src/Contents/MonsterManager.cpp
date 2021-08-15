#include "./Contents/MonsterManager.h"

MonsterManager::MonsterManager()	
{
	for (int i = 0; i < MAX_MONSTER_COUNT; i++)
	{		
		BYTE dir = rand() % 4;

		//TODO : 생성 위치는 사실 맵 크기에 비례해야한다. World.Create함수에서 맵 크기를 지정
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

Monster* MonsterManager::GetMonster(int id)
{
	return mMonsters[id];
}
