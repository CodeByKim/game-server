#include "./Contents/Player.h"
#include "Common/Protocol.h"
#include "./Contents/RPGGameLogic.h"

Player::Player()
	: BasePlayer(false, nullptr)
	, mGameLogic(nullptr)		
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

void Player::OnAppendToWorld(std::vector<BasePlayer*>& otherPlayers, std::vector<Entity*>& otherMonsters)
{
	int id = GetID();
	BYTE dir = GetDirection();
	Position2D& playerPos = GetPosition();

	SEND_CREATE_MY_PLAYER(*GetClientInfo(),
						  id,
						  dir,
						  playerPos.x,
						  playerPos.y);
	
	BROADCAST_CREATE_OTHER_PLAYER(*mWorld,
								  id,
								  dir,
								  playerPos.x,
								  playerPos.y,
								  this);

	for (auto iter = otherPlayers.begin(); 
		 iter != otherPlayers.end(); 
		 ++iter)
	{
		BasePlayer* otherPlayer = *iter;

		int id = otherPlayer->GetID();
		BYTE dir = otherPlayer->GetDirection();
		Position2D& playerPos = otherPlayer->GetPosition();

		SEND_CREATE_OTHER_PLAYER(*GetClientInfo(),
								  id,
								  dir,
								  playerPos.x,
								  playerPos.y);

		/*
		 * ������ Ŭ�� �̵����̾��ٸ�
		 * �̵����̶�� ���� �˷��� ��
		 */
		if (otherPlayer->IsMove())
		{
			SEND_PLAYER_MOVE_START(*GetClientInfo(),
								   id,
								   dir,
								   playerPos.x,
								   playerPos.y);
		}
	}

	for (auto iter = otherMonsters.begin();
		 iter != otherMonsters.end();
		 ++iter)
	{
		Entity* monster = *iter;

		SEND_CREATE_MONSTER(*GetClientInfo(),
							monster->GetID(),
							monster->GetDirection(),
							monster->GetPosition().x,
							monster->GetPosition().y);

		/*
		 * �� ���Ͱ� �̵����̾��ٸ�
		 * �̵����̶�� ���� �˷��� ��
		 */
		/*if (monster->IsMove())
		{
			SEND_PLAYER_MOVE_START(*player->GetClientInfo(),
				id,
				dir,
				playerPos.x,
				playerPos.y);
		}*/
	}
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

void Player::Teleport(BYTE dir, float x, float y)
{
	mCurrentDir = dir;
	mPosition.x = x;
	mPosition.y = y;
}
