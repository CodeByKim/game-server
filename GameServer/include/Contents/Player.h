#pragma once
#include <NetworkLib.h>

#include "./Common/GameCommon.h"
//#include "./Contents/Entity.h"
#include "./Contents/BasePlayer.h"
class Sector;
class RPGGameLogic;

class Player : public BasePlayer
{
public:
	Player();
	~Player();

	void Initialize(garam::net::ClientInfo* client, Position2D pos, RPGGameLogic* gameLogic);	
	void MoveStart(BYTE dir, float x, float y);
	void MoveEnd(BYTE dir, float x, float y);
	void Teleport(BYTE dir, float x, float y);	
	//garam::net::ClientInfo* GetClientInfo();		
	//void OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter) override;
	//bool IsMove();

	void OnUpdate(float deltaTime) override;
	void OnHit(int damage) override;
private:
	//void ProcessLeaveSector(std::vector<Sector*>& leaveSectors);
	//void ProcessNewEnterSector(std::vector<Sector*>& enterSectors);

	RPGGameLogic* mGameLogic;
	//garam::net::ClientInfo* mClient;
	//bool mIsMoving;	
};