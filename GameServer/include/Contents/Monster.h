#pragma once
#include <NetworkLib.h>
#include "Common/GameCommon.h"

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
	Position2DInt& GetSectorPosition();
	Position2D& GetPosition();
	void SetSectorPosition(int x, int y);

private:
	int mHP;
	int mID;
	BYTE mCurrentDir;	
	Position2D mPosition;
	Position2DInt mSectorPosition;
};
