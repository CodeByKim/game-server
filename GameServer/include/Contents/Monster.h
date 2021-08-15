#pragma once
#include <NetworkLib.h>
#include "Common/GameCommon.h"

class Monster
{
public:
	Monster();
	~Monster();

	void Initialize(int id, BYTE dir, Position pos, int hp);
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
	Position mPosition;
	GridLocation mSectorPosition;
};
