#include "./Contents/Player.h"
#include "Contents/World.h"
#include "Common/Protocol.h"

Player::Player()	
	: mClient(nullptr)
	, mPosition(Position{ 0,0 })
	, mCurrentDir(MOVE_DIR_RIGHT)
	, mIsMoving(false)
{
}

Player::~Player()
{
}

void Player::Initialize(garam::net::ClientInfo* client, Position pos)
{
	mClient = client;
	mPosition = pos;
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

        /*GridLocation pos = mPlayerPosition + moveOffset[mMoveDir];
        if (pos.x < 0 || pos.x > 6400 || pos.y < 0 || pos.y > 6400)
            return;*/

		Position offset = moveOffset[mCurrentDir];
		offset.Multiply(deltaTime);

		mPosition += offset;
    }
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
	for (auto iter = leave.begin(); 
		 iter != leave.end(); 
		 ++iter)
	{
		Sector* leaveSector = *iter;
		std::list<Player*> players = leaveSector->players;

		for (auto iter = players.begin(); 
			 iter != players.end(); 
			 ++iter)
		{			
			Player* otherPlayer = *iter;

			//TODO : 이 부분 수상...
			if (otherPlayer->GetID() == GetID())
				continue;

			// otherPlayer에게 player가 삭제됬다고 전달해라
			SendRemovePlayerToLeaveSector(otherPlayer, this);

			//player에게 otherPlayer가 삭제되었다고 전달해라
			SendRemovePlayerToLeaveSector(this, otherPlayer);			
		}		
	}	

	for (auto iter = enter.begin();
		 iter != enter.end();
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
			
			// otherPlayer에게 player가 새로 생성되었다고 전달해라
			SendCreatePlayerToEnterSector(otherPlayer, this);
							
			// player에게 otherPlayer가 새로 생성되었다고 전달해라
			SendCreatePlayerToEnterSector(this, otherPlayer);									  
			
			if (IsMove())
			{			
				/*
				 * 내가 지금 이동 중이라면
				 * 새로 진입한 섹터의 다른 플레이어에게 
				 * 내가 이동중이라는 것을 알려라
				 */
				SendMovePlayerToEnterSector(otherPlayer, this);
			}

			if (otherPlayer->IsMove())
			{		
				/*
				 * 새로 진입한 섹터의 다른 플레이어가 이동 중이라면
				 * 나한테 그 플레이어의 정보를 보내라				 
				 */
				SendMovePlayerToEnterSector(this, otherPlayer);
			}
		}
	}
}

void Player::SendRemovePlayerToLeaveSector(Player* sender, Player* leavePlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
	int id = leavePlayer->GetID();

	*packet << protocol << id;
	sender->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void Player::SendCreatePlayerToEnterSector(Player* sender, Player* enterPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
	int id = enterPlayer->GetID();
	BYTE dir = enterPlayer->GetDirection();
	float x = enterPlayer->GetPosition().x;
	float y = enterPlayer->GetPosition().y;

	*packet << protocol << id << dir << x << y;
	sender->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void Player::SendMovePlayerToEnterSector(Player* sender, Player* enterPlayer)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();

	short protocol = PACKET_SC_PLAYER_MOVE_START;
	int id = enterPlayer->GetID();
	BYTE dir = enterPlayer->GetDirection();
	Position playerPos = enterPlayer->GetPosition();

	*packet << protocol << id << dir << playerPos.x << playerPos.y;
	sender->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}
