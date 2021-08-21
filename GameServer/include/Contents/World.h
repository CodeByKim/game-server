#pragma once
#include <NetworkLib.h>
#include "./Contents/MonsterManager.h"

#define MAP_SIZE_X 2000
#define MAP_SIZE_Y 2000

class BasePlayer;
class Entity;

class Sector
{
public:
	Sector()
	{
		x = 0;
		y = 0;
		players.clear();
	}

	bool operator==(Sector& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(Sector& other)
	{
		return !(*this == other);
	}

	std::list<BasePlayer*> players;
	std::list<Monster*> monsters;

	int x;
	int y;
};

class World
{
public:
	World();
	~World();
	
	void Create(int sectorCountX, int sectorCountY, int sectorSize);
	void AddPlayer(BasePlayer* player);
	void RemovePlayer(BasePlayer* player);
	void Broadcast(garam::net::NetPacket* packet, BasePlayer* sender, bool exceptSender = true);
	Sector* GetSector(Entity* entity);
	void GetAroundSector(Entity* entity, std::vector<Sector*>* outAroundSectors);
	void OnUpdate(float deltaTime);	
	void AddMonster(Monster* monster);
	void DeadMonster(Monster* monster);

private:
	void GetPlayerInfoContainedInSector(BasePlayer* player, std::vector<BasePlayer*>& otherPlayers);
	void GetMonsterInfoContainedInSector(BasePlayer* player, std::vector<Monster*>& otherMonsters);
	void ChangeSector(BasePlayer* player, float x, float y);

	Sector** mSectors;
	std::vector<BasePlayer*> mPlayers;
	MonsterManager mMonsterManager;	
	int mSectorSize;
	int mSectorCountX;
	int mSectorCountY;
};