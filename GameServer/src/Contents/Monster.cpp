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

int Monster::GetID()
{
	return mID;
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