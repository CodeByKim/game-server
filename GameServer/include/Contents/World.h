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

	int x;
	int y;
};

class World
{
public:
	World();
	~World();

	void AddPlayer(Player* player);
	void RemovePlayer(Player* player);
	void Broadcast(garam::net::NetPacket* packet, Player* player);	
	void GetAroundSector(Player* player, std::vector<Sector*>* outAroundSectors);	
	void Update();

private:
	Sector mSectors[SECTOR_COUNT_Y][SECTOR_COUNT_X];
	std::vector<Player*> mPlayers;
};