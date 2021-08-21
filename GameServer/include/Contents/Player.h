#pragma once
#include <NetworkLib.h>

#include "./Common/GameCommon.h"
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

	void OnUpdate(float deltaTime) override;
	void OnHit(int damage) override;
private:	
	RPGGameLogic* mGameLogic;	
};