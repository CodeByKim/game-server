#include "./Contents/Player.h"
#include "Contents/World.h"
#include "Common/Protocol.h"
#include "./Contents/RPGGameLogic.h"

Player::Player()	
	: mGameLogic(nullptr)
	, mSectorPosition(GridLocation{0, 0})
	, mCurrentDir(MOVE_DIR_RIGHT)
	, mIsMoving(false)	
	, mClient(nullptr)
	, mPosition(Position{ 0,0 })	
{
}

Player::~Player()
{
}

void Player::Initialize(garam::net::ClientInfo* client, Position pos, RPGGameLogic* gameLogic)
{
	mClient = client;
	mPosition = pos;
	mGameLogic = gameLogic;
}

void Player::OnUpdate(float deltaTime)
{
    if (mIsMoving)
    {
        Position moveOffset[] = {
            {-MOVE_SPEED, 0},
            {0 , MOVE_SPEED},
            {MOVE_SPEED, 0},
            {0, -MOVE_SPEED}
        };

		Position offset = moveOffset[mCurrentDir];
		offset.Multiply(deltaTime);
		Position newPos = mPosition + offset;

		//�� ���� ������� Ȯ�� �ʿ�
		if (newPos.x <= 0 || newPos.x >= MAP_SIZE_X || 
			newPos.y <= 0 || newPos.y >= MAP_SIZE_Y)
		{
			return;
		}

		mPosition += offset;		
    }
}

void Player::MoveStart(BYTE dir, float x, float y)
{
	mCurrentDir = dir;

	//�̰� Ŭ�󿡼� ������ ���� Ȯ���ϴ� �뵵
	//mPosition = Position{ (float)x, (float)y };

	mIsMoving = true;
}

void Player::MoveEnd(BYTE dir, float x, float y)
{
	mCurrentDir = dir;
	
	//�̰� Ŭ�󿡼� ������ ���� Ȯ���ϴ� �뵵
	//mPosition = Position{ (float)x, (float)y };
	
	

	mIsMoving = false;
}

void Player::Teleport(BYTE dir, float x, float y)
{
	mCurrentDir = dir;
	mPosition.x = x;
	mPosition.y = y;
}

GridLocation& Player::GetSectorPosition()
{
	return mSectorPosition;
}

void Player::SetSectorPosition(int x, int y)
{
	mSectorPosition.x = x;
	mSectorPosition.y = y;
}

Position& Player::GetPosition()
{
	return mPosition;
}

BYTE Player::GetDirection()
{
	return mCurrentDir;
}

garam::net::ClientInfo* Player::GetClientInfo()
{
	return mClient;
}

int Player::GetID()
{
	return mClient->GetID();
}

bool Player::IsMove()
{
	return mIsMoving;
}

void Player::OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter)
{
	// ���� ������ Leave Sector�� ���� ó��
	ProcessLeaveSector(leave);

	// ���� ������ Enter Sector�� ���� ó��
	ProcessNewEnterSector(enter);
}

void Player::ProcessLeaveSector(std::vector<Sector*>& leaveSectors)
{	
	for (auto iter = leaveSectors.begin();
		 iter != leaveSectors.end();
		 ++iter)
	{
		Sector* leaveSector = *iter;
		std::list<Player*> players = leaveSector->players;

		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (otherPlayer->GetID() == GetID())
				continue;

			// otherPlayer���� player�� ������ٰ� �����ض�						
			SEND_REMOVE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), GetID());

			//player���� otherPlayer�� �����Ǿ��ٰ� �����ض�						
			SEND_REMOVE_OTHER_PLAYER(*GetClientInfo(), otherPlayer->GetID());
		}
	}
}

void Player::ProcessNewEnterSector(std::vector<Sector*>& enterSectors)
{
	for (auto iter = enterSectors.begin();
		 iter != enterSectors.end();
		 ++iter)
	{
		Sector* enterSector = *iter;
		std::list<Player*> players = enterSector->players;

		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (otherPlayer->GetID() == GetID())
				continue;

			// otherPlayer���� player�� ���� �����Ǿ��ٰ� �����ض�						
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
	}
}
