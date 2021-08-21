#pragma once
#include <NetworkLib.h>
#include "./Contents/Entity.h"

class Sector;
class Monster;
class Entity;
class World;

class BasePlayer : public Entity
{
public :
	BasePlayer(bool isMoving, garam::net::ClientInfo* client);
	~BasePlayer();

	bool IsMove();
	garam::net::ClientInfo* GetClientInfo();
	void OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter);
	World* GetWorld();
	virtual void OnAppendToWorld(std::vector<BasePlayer*>& otherPlayers, std::vector<Monster*>& otherMonsters) = 0;

protected:		
	virtual void OnOtherPlayerLeaveSectorRange(BasePlayer* otherPlayer) = 0;
	virtual void OnOtherPlayerEnterSectorRange(BasePlayer* otherPlayer) = 0;
	virtual void OnOtherMonsterLeaveSectorRange(Monster* otherMonster) = 0;
	virtual void OnOtherMonsterEnterSectorRange(Monster* otherMonster) = 0;

	garam::net::ClientInfo* mClient;
	bool mIsMoving;

private:
	void ProcessLeaveSector(std::vector<Sector*>& leaveSectors);
	void ProcessNewEnterSector(std::vector<Sector*>& enterSectors);	

};

