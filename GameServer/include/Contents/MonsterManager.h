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

	//������ �ִ� ������ ������ ������ �ְ�
	//���Ͱ� ������ �ش� �ε����� ���͸� �׳� ��Ȱ��ȭ ��Ű�� ������� ����
	//Monster Manager�� ������ �ȸ������� ���.. ����°� ���� �ϰٴ�.
	Monster* mMonsters[MAX_MONSTER_COUNT];
	std::stack<int> mEmptyMonsterIndex;
};

