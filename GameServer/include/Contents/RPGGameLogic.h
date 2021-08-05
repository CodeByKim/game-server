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
	
	Player* GetPlayer(int id);

private:
	Player* CreatePlayer(garam::net::ClientInfo* info);
	bool IsContainPlayer(int id);

	void SendCreateMyPlayer(Player* player);
	void BroadcastCreateOtherPlayer(Player* player);

	std::unordered_map<int, Player*> mPlayers;
	garam::memory::MemoryPool<Player> mPlayerPool;
};