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

	Position& GetPosition();
	garam::net::ClientInfo* GetClientInfo();
	int GetID();

private:
	garam::net::ClientInfo* mClient;
	Position mPosition;
};

