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
	void SendCreateOtherPlayer(Player* player, int id, BYTE dir, float x, float y);
	void SendRemovePlayer(Player* player, int id);
	void SendPlayerMoveStart(Player* player, int id, BYTE dir, float x, float y);

	Player* GetPlayer(int id);

private:
	Player* CreatePlayer(garam::net::ClientInfo* info);
	bool IsContainPlayer(int id);	
	void SendCreateMyPlayer(Player* player, int id, BYTE dir, float x, float y);
	void SendPlayerInfoContainedInSector(Player* player);
	void BroadcastCreateOtherPlayer(int id, BYTE dir, float x, float y, Player* exceptPlayer);
	void BroadcastPlayerMoveStart(int id, BYTE dir, float x, float y, Player* exceptPlayer);
	void BroadcastPlayerMoveEnd(int id, BYTE dir, float x, float y, Player* exceptPlayer);
	void BroadcastRemovePlayer(int id, Player* exceptPlayer);

	std::unordered_map<int, Player*> mPlayers;
	std::list<Player*> mDeletedPlayers;
	garam::memory::MemoryPool<Player> mPlayerPool;
	World mWorld;
};