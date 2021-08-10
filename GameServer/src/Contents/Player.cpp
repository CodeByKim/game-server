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
			if ((*iter)->GetID() == GetID())
				continue;

			Player* otherPlayer = *iter;
			//send packet;
			{
				/*
				 * otherPlayer에게 player가 삭제됬다고 전달해라
				 */
				garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
				short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
				int id = GetID();
				*packet << protocol << id;
				otherPlayer->GetClientInfo()->SendPacket(packet);
				garam::net::NetPacket::Free(packet);
			}
			{
				/*
				 * player에게 otherPlayer가 삭제되었다고 전달해라
				 */
				garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
				short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
				int id = otherPlayer->GetID();
				*packet << protocol << id;
				GetClientInfo()->SendPacket(packet);
				garam::net::NetPacket::Free(packet);
			}
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
			if ((*iter)->GetID() == GetID())
				continue;

			Player* otherPlayer = *iter;
			//send packet;
			{
				/*
				 * otherPlayer에게 player가 새로 생성되었다고 전달해라
				 */
				garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
				short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
				int id = GetID();
				BYTE dir = GetDirection();
				Position playerPos = GetPosition();
				*packet << protocol << id << dir << playerPos.x << playerPos.y;

				otherPlayer->GetClientInfo()->SendPacket(packet);
				garam::net::NetPacket::Free(packet);
			}
			{
				/*
				 * player에게 otherPlayer가 새로 생성되었다고 전달해라
				 */
				garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
				short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
				int id = otherPlayer->GetID();
				BYTE dir = otherPlayer->GetDirection();
				std::cout << (int)dir << std::endl;
				Position playerPos = otherPlayer->GetPosition();
				*packet << protocol << id << dir << playerPos.x << playerPos.y;

				GetClientInfo()->SendPacket(packet);
				garam::net::NetPacket::Free(packet);
			}

			if (IsMove())
			{
				garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
				short protocol = PACKET_SC_PLAYER_MOVE_START;
				int id = GetClientInfo()->GetID();
				BYTE dir = GetDirection();
				Position playerPos = GetPosition();
				*packet << protocol << id << dir << playerPos.x << playerPos.y;

				otherPlayer->GetClientInfo()->SendPacket(packet);
				garam::net::NetPacket::Free(packet);
			}

			if (otherPlayer->IsMove())
			{
				garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
				short protocol = PACKET_SC_PLAYER_MOVE_START;
				int id = otherPlayer->GetClientInfo()->GetID();
				BYTE dir = otherPlayer->GetDirection();
				Position playerPos = otherPlayer->GetPosition();
				*packet << protocol << id << dir << playerPos.x << playerPos.y;

				GetClientInfo()->SendPacket(packet);
				garam::net::NetPacket::Free(packet);
			}
		}
	}
}
