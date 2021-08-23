#include "./Contents/Monster.h"
#include "./Common/Protocol.h"

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

void Monster::OnLeaveSectorOtherPlayer(garam::net::BasePlayer* otherPlayer)
{
	SEND_REMOVE_MONSTER(*otherPlayer->GetClientInfo(), 
						GetID());
}

void Monster::OnEnterSectorOtherPlayer(garam::net::BasePlayer* otherPlayer)
{
	SEND_CREATE_MONSTER(*otherPlayer->GetClientInfo(),
						GetID(),
						GetDirection(),
						GetPosition().x,
						GetPosition().y);

	/*
	 * 이 몬스터가 이동중이었다면
	 * 이동중이라는 것을 알려야 함
	 */
	/*if (entity->IsMove())
	{
		SEND_MONSTER_MOVE_START(*player->GetClientInfo(),
								id,
								dir,
								playerPos.x,
								playerPos.y);
	}*/
}

void Monster::Clear()
{
	mID = -1;
	mCurrentDir = MOVE_DIR_UP;
	mPosition = { -1, -1 };
	mHP = -1;
}