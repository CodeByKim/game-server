#include "./Contents/Player.h"
#include "Common/Protocol.h"
#include "./Contents/RPGGameLogic.h"

Player::Player(int hp, BYTE dir, Position2D position, garam::net::ClientInfo* client, RPGGameLogic* gameLogic)
	: BasePlayer(hp, client->GetID(), dir, position, client, false)
	, mGameLogic(gameLogic)	
	, mWorld(gameLogic->GetWorld())
{		
}

Player::~Player()
{	
}

//void Player::Initialize(garam::net::ClientInfo* client, Position2D pos, RPGGameLogic* gameLogic)
//{
//	//mClient = client;
//	mPosition = pos;
//	//mGameLogic = gameLogic;
//	//mWorld = gameLogic->GetWorld();
//	//mID = mClient->GetID();
//}

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

void Player::OnLeaveSectorOtherPlayer(garam::net::BasePlayer* otherPlayer)
{
	// otherPlayer���� player�� ������ٰ� �����ض�						
	SEND_REMOVE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), GetID());

	//player���� otherPlayer�� �����Ǿ��ٰ� �����ض�						
	SEND_REMOVE_OTHER_PLAYER(*GetClientInfo(), otherPlayer->GetID());
}

void Player::OnEnterSectorOtherPlayer(garam::net::BasePlayer* otherPlayer)
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