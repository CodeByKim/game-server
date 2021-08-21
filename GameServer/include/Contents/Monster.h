#pragma once
#include <NetworkLib.h>
#include "Common/GameCommon.h"
#include "./Entity.h"

class Monster : public Entity
{
public:
	Monster();
	~Monster();
	
	void Initialize(int id);
	void Clear();

	void OnUpdate(float deltaTime) override;
	void OnHit(int damage) override;

private:	
};
