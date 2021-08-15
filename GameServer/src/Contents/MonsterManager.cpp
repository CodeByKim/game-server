#include "./Contents/MonsterManager.h"

MonsterManager::MonsterManager()	
{
	for (int i = 0; i < MAX_MONSTER_COUNT; i++)
	{		
		BYTE dir = rand() % 4;
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
