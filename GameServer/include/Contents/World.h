#pragma once
#include <NetworkLib.h>
#include "./Contents/MonsterManager.h"

#define MAP_SIZE_X 2000
#define MAP_SIZE_Y 2000

class Player;

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

	std::list<Player*> players;
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
	void AddPlayer(Player* player);
	void RemovePlayer(Player* player);
	void Broadcast(garam::net::NetPacket* packet, Player* sender, bool exceptSender = true);
	Sector* GetSector(Player* player);
	Sector* GetSector(Monster* monster);
	void GetAroundSector(Player* player, std::vector<Sector*>* outAroundSectors);	
	void GetAroundSector(Monster* monster, std::vector<Sector*>* outAroundSectors);
	void OnUpdate(float deltaTime);
	void ChangeSectorAndNotifyMessageToPlayer(Player* player, float x, float y);	
	void AddMonster(Monster* monster);
	void DeadMonster(Monster* monster);

private:
	void SendPlayerInfoContainedInSector(Player* player);
	void SendMonsterInfoContainedInSector(Player* player);
	void ChangeSector(Player* player, float x, float y);

	Sector** mSectors;
	std::vector<Player*> mPlayers;
	int mSectorSize;
	int mSectorCountX;
	int mSectorCountY;
	MonsterManager mMonsterManager;	
};