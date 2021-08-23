#pragma once
#include <NetworkLib.h>
#include "Common/GameCommon.h"

class garam::net::BasePlayer;

class Monster : public garam::net::Entity
{
public:
	Monster();
	~Monster();
	
	void Initialize(int id);
	void Clear();

	void OnUpdate(float deltaTime) override;
	void OnHit(int damage) override;
	void OnLeaveSectorOtherPlayer(garam::net::BasePlayer* otherPlayer) override;
	void OnEnterSectorOtherPlayer(garam::net::BasePlayer* otherPlayer) override;

private:	
};
