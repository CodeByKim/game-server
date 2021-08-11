#pragma once
#include <NetworkLib.h>

#include "Common/GameCommon.h"

class Sector;

class Player
{
public:
	Player();
	~Player();

	void Initialize(garam::net::ClientInfo* client, Position pos);
	void OnUpdate(float deltaTime);
	void MoveStart(BYTE dir, float x, float y);
	void MoveEnd(BYTE dir, float x, float y);

	GridLocation& GetSectorPosition();
	void SetSectorPosition(int x, int y);	
	Position& GetPosition();
	BYTE GetDirection();
	garam::net::ClientInfo* GetClientInfo();
	int GetID();
	bool IsMove();
	
	void OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter);

private:
	void SendRemovePlayerToLeaveSector(Player* sender, Player* leavePlayer);
	void SendCreatePlayerToEnterSector(Player* sender, Player* enterPlayer);
	void SendMovePlayerToEnterSector(Player* sender, Player* enterPlayer);

	GridLocation mSectorPosition;
	BYTE mCurrentDir;
	bool mIsMoving;
	garam::net::ClientInfo* mClient;
	Position mPosition;
};