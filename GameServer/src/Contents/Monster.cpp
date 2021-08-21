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

void Monster::OnHit(int damage)
{
	mHP -= damage;
}

void Monster::Clear()
{
	mID = -1;
	mCurrentDir = MOVE_DIR_UP;
	mPosition = { -1, -1 };
	mHP = -1;
}