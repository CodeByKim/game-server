#pragma once
#include <NetworkLib.h>
#include "./World.h"

class Player;

class RPGGameLogic
{
public:
	RPGGameLogic();
	~RPGGameLogic();

	void Update(float deltaTime);
	void AddNewPlayer(garam::net::ClientInfo* info);
	void LeavePlayer(garam::net::ClientInfo* info);
	void PlayerMoveStart(int id, BYTE dir, float x, float y);
	void PlayerMoveEnd(int id, BYTE dir, float x, float y);

	Player* GetPlayer(int id);

private:
	Player* CreatePlayer(garam::net::ClientInfo* info);
	bool IsContainPlayer(int id);

	//void BroadcastPacket(garam::net::NetPacket* packet, garam::net::ClientInfo* exceptClient);
	void SendCreateMyPlayer(Player* player);
	void SendExistingPlayerInfo(Player* player);
	void BroadcastCreateOtherPlayer(Player* player);
	void BroadcastPlayerMoveStart(Player* player);
	void BroadcastPlayerMoveEnd(Player* player);

	std::unordered_map<int, Player*> mPlayers;
	std::list<Player*> mDeletedPlayers;
	garam::memory::MemoryPool<Player> mPlayerPool;
	World mWorld;
};