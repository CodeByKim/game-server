#pragma once
#include <NetworkLib.h>
#include "./Contents/MonsterManager.h"

class Monster;

class RpgGameWorld : public garam::net::World
{
public:
	RpgGameWorld();
	~RpgGameWorld();

	void OnUpdate(float deltaTime) override;

	void SpawnMonster(int count);
	void AddMonster(Monster* monster);
	void DeadMonster(Monster* monster);

protected:
	void OnPlayerJoin(garam::net::BasePlayer* player, 
					  std::vector<garam::net::BasePlayer*>& otherPlayers, 
					  std::vector<garam::net::Entity*>& otherEntities) override;
	void OnPlayerLeave(garam::net::BasePlayer* leavePlayer) override;
	void ProcessLeaveSector(garam::net::BasePlayer* player, std::vector<garam::net::Sector*>& leaveSectors) override;
	void ProcessNewEnterSector(garam::net::BasePlayer* player, std::vector<garam::net::Sector*>& enterSectors) override;

private:
	MonsterManager mMonsterManager;
};