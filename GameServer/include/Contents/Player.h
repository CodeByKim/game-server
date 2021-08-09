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
	void MoveStart(BYTE dir, short x, short y);
	void MoveEnd(BYTE dir, short x, short y);

	GridLocation& GetSectorPosition();
	void SetSectorPosition(int x, int y);	
	Position& GetPosition();
	BYTE GetDirection();
	garam::net::ClientInfo* GetClientInfo();
	int GetID();
	bool IsMove();
	
private:
	GridLocation mSectorPosition;
	BYTE mCurrentDir;
	bool mIsMoving;
	garam::net::ClientInfo* mClient;
	Position mPosition;
};