#pragma once
#include <NetworkLib.h>

#define SECTOR_SIZE 6
#define SECTOR_COUNT_X 64
#define SECTOR_COUNT_Y 64

class Player;

class Sector
{
public:
	Sector()
	{
		x = 0;
		y = 0;
		mPlayers.clear();
	}

	bool operator==(Sector& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(Sector& other)
	{
		return !(*this == other);
	}

	std::list<Player*> mPlayers;

	int x;
	int y;
};

class SectorManager
{
public:
	SectorManager();
	~SectorManager();
	void AddPlayer(Player* player);
	void RemovePlayer(Player* player);
	
	void PerformAroundPlayers(Player* player, std::function<void(Player*)> func);

	//PerformAroundPlayers 대체 함수
	void Broadcast(garam::net::NetPacket* packet, Player* player);
	void GetAroundSector(int x, int y, Sector** arr);

	void UpdateSector(Player* player, std::function<void(Player*)> old, std::function<void(Player*)> update);

private:
	Sector mSectors[SECTOR_COUNT_Y][SECTOR_COUNT_X];
};