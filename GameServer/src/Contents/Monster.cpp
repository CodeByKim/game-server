#include "./Contents/Monster.h"

Monster::Monster()
{	
}

Monster::~Monster()
{
}

void Monster::Initialize(int id)
{	
	mID = id;
	mCurrentDir = rand() % 4;
	mPosition = Position2D{ (float)(rand() % 1900), (float)(rand() % 1900) };
	mHP = 100;	
}

void Monster::OnUpdate(float deltaTime)
{	
}

void Monster::Hit()
{
	mHP -= 10;	
}

void Monster::Clear()
{
	mID = -1;
	mCurrentDir = MOVE_DIR_UP;
	mPosition = { -1, -1 };
	mHP = -1;
}

int Monster::GetID()
{
	return mID;
}

int Monster::GetHP()
{
	return mHP;
}

bool Monster::IsDead()
{
	return mHP <= 0;
}

Position2D& Monster::GetPosition()
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

Position2DInt& Monster::GetSectorPosition()
{
	return mSectorPosition;
}