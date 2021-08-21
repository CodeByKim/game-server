#include "./Contents/Player.h"
#include "Contents/World.h"
#include "Common/Protocol.h"
#include "./Contents/RPGGameLogic.h"

Player::Player()	
	: mGameLogic(nullptr)	
	, mIsMoving(false)	
	, mClient(nullptr)	
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

		//맵 범위 벗어났는지 확인 필요
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

void Player::MoveStart(BYTE dir, float x, float y)
{
	mCurrentDir = dir;

	//이건 클라에서 보내준 값과 확인하는 용도
	//mPosition = Position{ (float)x, (float)y };

	mIsMoving = true;
}

void Player::MoveEnd(BYTE dir, float x, float y)
{
	mCurrentDir = dir;
	
	//이건 클라에서 보내준 값과 확인하는 용도
	//mPosition = Position{ (float)x, (float)y };
	
	mIsMoving = false;
}

void Player::Teleport(BYTE dir, float x, float y)
{
	mCurrentDir = dir;
	mPosition.x = x;
	mPosition.y = y;
}

garam::net::ClientInfo* Player::GetClientInfo()
{
	return mClient;
}

bool Player::IsMove()
{
	return mIsMoving;
}

void Player::OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter)
{	
	// 이전 섹터인 Leave Sector에 관한 처리
	ProcessLeaveSector(leave);

	// 새로 진입한 Enter Sector에 관한 처리
	ProcessNewEnterSector(enter);
}

void Player::ProcessLeaveSector(std::vector<Sector*>& leaveSectors)
{		
	for(int i = 0 ; i < leaveSectors.size() ; i++)
	{		
		Sector* leaveSector = leaveSectors[i];
		std::list<Player*>& players = leaveSector->players;
		
		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (otherPlayer->GetID() == GetID())
				continue;
			
			
			// otherPlayer에게 player가 삭제됬다고 전달해라						
			SEND_REMOVE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), GetID());

			//player에게 otherPlayer가 삭제되었다고 전달해라						
			SEND_REMOVE_OTHER_PLAYER(*GetClientInfo(), otherPlayer->GetID());
			
		}

		std::list<Monster*>& monsters = leaveSector->monsters;

		for (auto iter = monsters.begin();
			 iter != monsters.end();
			 ++iter)
		{
			Monster* monster = *iter;

			SEND_REMOVE_MONSTER(*GetClientInfo(), monster->GetID());
		}
	}	
}

void Player::ProcessNewEnterSector(std::vector<Sector*>& enterSectors)
{	
	for(int i = 0 ; i < enterSectors.size() ; i++)
	{		
		Sector* enterSector = enterSectors[i];
		std::list<Player*>& players = enterSector->players;

		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (otherPlayer->GetID() == GetID())
				continue;

			// otherPlayer에게 player가 새로 생성되었다고 전달해라						
			SEND_CREATE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), 
									 GetID(), 
									 GetDirection(),
									 GetPosition().x,
									 GetPosition().y);

			// player에게 otherPlayer가 새로 생성되었다고 전달해라						
			SEND_CREATE_OTHER_PLAYER(*GetClientInfo(),
									 otherPlayer->GetID(),
									 otherPlayer->GetDirection(),
									 otherPlayer->GetPosition().x,
									 otherPlayer->GetPosition().y);

			if (IsMove())
			{
				/*
				 * 내가 지금 이동 중이라면
				 * 새로 진입한 섹터의 다른 플레이어에게
				 * 내가 이동중이라는 것을 알려라
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
				 * 새로 진입한 섹터의 다른 플레이어가 이동 중이라면
				 * 나한테 그 플레이어의 정보를 보내라
				 */				
				SEND_PLAYER_MOVE_START(*GetClientInfo(),
										otherPlayer->GetID(),
										otherPlayer->GetDirection(),
										otherPlayer->GetPosition().x,
										otherPlayer->GetPosition().y);
			}
		}

		std::list<Monster*>& monsters = enterSector->monsters;

		for (auto iter = monsters.begin();
			iter != monsters.end();
			++iter)
		{
			Monster* monster = *iter;
			
			SEND_CREATE_MONSTER(*GetClientInfo(), 
								monster->GetID(), 
								monster->GetDirection(), 								
								monster->GetPosition().x, 
								monster->GetPosition().y);
		}
	}
}
