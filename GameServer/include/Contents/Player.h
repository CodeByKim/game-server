#pragma once
#include <NetworkLib.h>

#include "./Common/GameCommon.h"
#include "./Contents/BasePlayer.h"

class Sector;
class RPGGameLogic;
class RpgGameWorld;
class Monster;
class Entity;

class Player : public garam::net::BasePlayer
{
public:
	Player();
	~Player();

	void Initialize(garam::net::ClientInfo* client, Position2D pos, RPGGameLogic* gameLogic);	
	void MoveStart(BYTE dir, float x, float y);
	void MoveEnd(BYTE dir, float x, float y);	

	void OnUpdate(float deltaTime) override;
	void OnHit(int damage) override;	

protected:
	void OnOtherPlayerLeaveSectorRange(BasePlayer* otherPlayer) override;
	void OnOtherPlayerEnterSectorRange(BasePlayer* otherPlayer) override;
	void OnOtherMonsterLeaveSectorRange(Entity* otherMonster) override;
	void OnOtherMonsterEnterSectorRange(Entity* otherMonster) override;

private:	
	RPGGameLogic* mGameLogic;
	RpgGameWorld* mWorld;
};