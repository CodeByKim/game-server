#pragma once
#include <NetworkLib.h>

#include "Common/GameCommon.h"

class Sector;
class RPGGameLogic;

class Player
{
public:
	Player();
	~Player();

	void Initialize(garam::net::ClientInfo* client, Position pos, RPGGameLogic* gameLogic);
	void OnUpdate(float deltaTime);
	void MoveStart(BYTE dir, float x, float y);
	void MoveEnd(BYTE dir, float x, float y);
	void Teleport(BYTE dir, float x, float y);
	GridLocation& GetSectorPosition();
	void SetSectorPosition(int x, int y);	
	Position& GetPosition();
	BYTE GetDirection();
	garam::net::ClientInfo* GetClientInfo();
	int GetID();
	bool IsMove();
	
	void OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter);	

private:
	void ProcessLeaveSector(std::vector<Sector*>& leaveSectors);
	void ProcessNewEnterSector(std::vector<Sector*>& enterSectors);

	RPGGameLogic* mGameLogic;
	GridLocation mSectorPosition;
	BYTE mCurrentDir;
	bool mIsMoving;
	garam::net::ClientInfo* mClient;
	Position mPosition;		
};