#include "./Contents/Player.h"
#include "Common/Protocol.h"
#include "./Contents/RPGGameLogic.h"

Player::Player()
	: BasePlayer(false, nullptr)
	, mGameLogic(nullptr)		
	, mWorld(nullptr)
{
}

Player::~Player()
{
}

void Player::Initialize(garam::net::ClientInfo* client, Position2D pos, RPGGameLogic* gameLogic)
{
	mClient = client;
	mPosition = pos;
	mGameLogic = gameLogic;
	mWorld = gameLogic->GetWorld();
	mID = mClient->GetID();
}

void Player::OnUpdate(float deltaTime)
{
    if (mIsMoving)
    {
        Position2D moveOffset[] = {
            {-MOVE_SPEED, 0},
            {0 , MOVE_SPEED},
            {MOVE_SPEED, 0},
            {0, -MOVE_SPEED}
        };

		Position2D offset = moveOffset[mCurrentDir];
		offset.Multiply(deltaTime);
		Position2D newPos = mPosition + offset;

		//�� ���� ������� Ȯ�� �ʿ�
		if (newPos.x <= 0 || newPos.x >= MAP_SIZE_X || 
			newPos.y <= 0 || newPos.y >= MAP_SIZE_Y)
		{
			return;
		}

		mPosition += offset;				
    }		
}

void Player::OnHit(int damage)
{
}

void Player::OnOtherPlayerLeaveSectorRange(BasePlayer* otherPlayer)
{
	// otherPlayer���� player�� ������ٰ� �����ض�						
	SEND_REMOVE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), GetID());

	//player���� otherPlayer�� �����Ǿ��ٰ� �����ض�						
	SEND_REMOVE_OTHER_PLAYER(*GetClientInfo(), otherPlayer->GetID());
}

void Player::OnOtherPlayerEnterSectorRange(BasePlayer* otherPlayer)
{
	 //otherPlayer���� player�� ���� �����Ǿ��ٰ� �����ض�						
	SEND_CREATE_OTHER_PLAYER(*otherPlayer->GetClientInfo(),
		GetID(),
		GetDirection(),
		GetPosition().x,
		GetPosition().y);

	// player���� otherPlayer�� ���� �����Ǿ��ٰ� �����ض�						
	SEND_CREATE_OTHER_PLAYER(*GetClientInfo(),
		otherPlayer->GetID(),
		otherPlayer->GetDirection(),
		otherPlayer->GetPosition().x,
		otherPlayer->GetPosition().y);

	if (IsMove())
	{
		/*
		 * ���� ���� �̵� ���̶��
		 * ���� ������ ������ �ٸ� �÷��̾��
		 * ���� �̵����̶�� ���� �˷���
		 */
		SEND_PLAYER_MOVE_START(*otherPlayer->GetClientInfo(),
			GetID(),
			GetDirection(),
			GetPosition().x,
			GetPosition().y);
	}

	if (otherPlayer->IsMove())
	{
		/*
		 * ���� ������ ������ �ٸ� �÷��̾ �̵� ���̶��
		 * ������ �� �÷��̾��� ������ ������
		 */
		SEND_PLAYER_MOVE_START(*GetClientInfo(),
			otherPlayer->GetID(),
			otherPlayer->GetDirection(),
			otherPlayer->GetPosition().x,
			otherPlayer->GetPosition().y);
	}
}

void Player::OnOtherMonsterLeaveSectorRange(Entity* otherMonster)
{
	SEND_REMOVE_MONSTER(*GetClientInfo(), otherMonster->GetID());
}

void Player::OnOtherMonsterEnterSectorRange(Entity* otherMonster)
{
	SEND_CREATE_MONSTER(*GetClientInfo(),
						otherMonster->GetID(),
						otherMonster->GetDirection(),
						otherMonster->GetPosition().x,
						otherMonster->GetPosition().y);
}

void Player::MoveStart(BYTE dir, float x, float y)
{
	mCurrentDir = dir;

	mIsMoving = true;
}

void Player::MoveEnd(BYTE dir, float x, float y)
{
	mCurrentDir = dir;
	
	mIsMoving = false;
}