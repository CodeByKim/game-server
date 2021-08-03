#include "./Contents/Player.h"

Player::Player()	
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

Position Player::GetPosition()
{
	return mPosition;
}

garam::net::ClientInfo* Player::GetClientInfo()
{
	return mClient;
}