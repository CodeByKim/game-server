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
	int GetID();

private:
	int mHP;
	int mID;
	BYTE mCurrentDir;
	Position mPosition;
};