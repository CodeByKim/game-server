#pragma once
#include <NetworkLib.h>

#include "Common/GameCommon.h"

class Player
{
public:
	Player();
	~Player();

	void Initialize(garam::net::ClientInfo* client, Position pos);
	void OnUpdate(float deltaTime);
	void MoveStart(char dir, short x, short y);
	void MoveEnd(char dir, short x, short y);

	Position& GetPosition();
	char GetDirection();
	garam::net::ClientInfo* GetClientInfo();
	int GetID();

private:
	char mCurrentDir;
	bool mIsMoving;
	garam::net::ClientInfo* mClient;
	Position mPosition;
};