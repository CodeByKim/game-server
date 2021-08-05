#pragma once
#include <NetworkLib.h>

class Player;

class RPGGameLogic
{
public:
	RPGGameLogic();
	~RPGGameLogic();

	void Update(float deltaTime);
	void AddNewPlayer(garam::net::ClientInfo* info);
	void PlayerMoveStart(int id, char dir, short x, short y);
	void PlayerMoveEnd(int id, char dir, short x, short y);

	Player* GetPlayer(int id);

private:
	Player* CreatePlayer(garam::net::ClientInfo* info);
	bool IsContainPlayer(int id);

	void SendCreateMyPlayer(Player* player);
	void BroadcastCreateOtherPlayer(Player* player);
	void BroadcastPlayerMoveStart(Player* player);
	void BroadcastPlayerMoveEnd(Player* player);

	std::unordered_map<int, Player*> mPlayers;
	std::list<Player*> mDeletedPlayers;
	garam::memory::MemoryPool<Player> mPlayerPool;
};