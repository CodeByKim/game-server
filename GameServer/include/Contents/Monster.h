#pragma once
#include <NetworkLib.h>
#include "Common/GameCommon.h"

#define MONSTER_TYPE_A 0
#define MONSTER_TYPE_B 1

class Monster
{
public:
	Monster();
	~Monster();
	
	void Initialize(int id);
	void OnUpdate(float deltaTime);
	void Hit();
	void Clear();

	int GetID();
	int GetHP();
	bool IsDead();
	BYTE GetDirection();
	GridLocation& GetSectorPosition();
	Position& GetPosition();
	void SetSectorPosition(int x, int y);

private:
	int mHP;
	int mID;
	BYTE mCurrentDir;
	BYTE mType;
	Position mPosition;
	GridLocation mSectorPosition;
};
