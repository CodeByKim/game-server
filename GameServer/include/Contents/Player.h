#pragma once
#include <NetworkLib.h>

#include "./Common/GameCommon.h"
#include "./Contents/BasePlayer.h"

class RPGGameLogic;
class RpgGameWorld;
class garam::net::ClientInfo;

class Player : public garam::net::BasePlayer
{
public:
	Player(int hp, BYTE dir, Position2D position, garam::net::ClientInfo* client, RPGGameLogic* gameLogic);
	~Player();

	//void Initialize(garam::net::ClientInfo* client, Position2D pos, RPGGameLogic* gameLogic);	
	void MoveStart(BYTE dir, float x, float y);
	void MoveEnd(BYTE dir, float x, float y);	

	void OnUpdate(float deltaTime) override;
	void OnHit(int damage) override;	

	void OnLeaveSectorOtherPlayer(garam::net::BasePlayer* otherPlayer) override;
	void OnEnterSectorOtherPlayer(garam::net::BasePlayer* otherPlayer) override;

protected:	

private:	
	RPGGameLogic* mGameLogic;
	RpgGameWorld* mWorld;
};