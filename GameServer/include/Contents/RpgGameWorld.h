#pragma once
#include <NetworkLib.h>
#include "./Contents/MonsterManager.h"

class Monster;

class RpgGameWorld : public garam::net::World
{
public:
	RpgGameWorld();
	~RpgGameWorld();

	void OnUpdate(float deltaTime) override;

	void SpawnMonster(int count);
	void AddMonster(Monster* monster);
	void DeadMonster(Monster* monster);

private:
	MonsterManager mMonsterManager;
};