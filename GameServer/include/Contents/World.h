#pragma once
#include <NetworkLib.h>

//#define MAP_SIZE_X 500
//#define MAP_SIZE_Y 500

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
	void Broadcast(garam::net::NetPacket* packet, Player* exceptPlayer);
	void GetAroundSector(Player* player, std::vector<Sector*>* outAroundSectors);	
	void Update();	
	void ChangeSector(Player* player, float x, float y);

private:	
	Sector** mSectors;
	std::vector<Player*> mPlayers;
	int mSectorSize;
	int mSectorCountX;
	int mSectorCountY;
};