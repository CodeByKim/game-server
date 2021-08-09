#include "./Contents/Player.h"

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

void Player::MoveStart(BYTE dir, short x, short y)
{
	mCurrentDir = dir;

	//이건 클라에서 보내준 값과 확인하는 용도
	//mPosition = Position{ (float)x, (float)y };
	mIsMoving = true;
}

void Player::MoveEnd(BYTE dir, short x, short y)
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
