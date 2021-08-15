#include "./Contents/Monster.h"

Monster::Monster()
{	
}

Monster::~Monster()
{
}

void Monster::Initialize(int id, BYTE dir, Position pos, int hp)
{
	mID = id;
	mCurrentDir = dir;
	mPosition = pos;
	mHP = hp;
}

void Monster::OnUpdate(float deltaTime)
{	
}

void Monster::Hit()
{
	mHP -= 10;

	if (mHP <= 0)
	{
		//TODO : 몬스터 Dead 처리 필요
	}
}

int Monster::GetID()
{
	return mID;
}

int Monster::GetHP()
{
	return mHP;
}

Position& Monster::GetPosition()
{
	return mPosition;
}

void Monster::SetSectorPosition(int x, int y)
{
	mSectorPosition.x = x;
	mSectorPosition.y = y;
}

BYTE Monster::GetDirection()
{
	return mCurrentDir;
}