#include "./Contents/Player.h"

Player::Player()	
	: mClient(nullptr)
	, mPosition(Position{ 0,0 })
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
}

Position& Player::GetPosition()
{
	return mPosition;
}

garam::net::ClientInfo* Player::GetClientInfo()
{
	return mClient;
}

int Player::GetID()
{
	return mClient->GetID();
}
