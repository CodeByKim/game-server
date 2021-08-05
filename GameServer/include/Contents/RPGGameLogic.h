#pragma once
#include <NetworkLib.h>

#define PACKET_SC_CREATE_MY_PLAYER 0;
#define PACKET_SC_CREATE_OTHER_PLAYER 1;

#define PACKET_SC_DELETE_MY_PLAYER 2;
#define PACKET_SC_DELETE_OTHER_PLAYER 3;

#define PACKET_CS_PLAYER_MOVE_START 4;
#define PACKET_CS_PLAYER_MOVE_END 5;
#define PACKET_SC_PLAYER_MOVE_START 6;
#define PACKET_SC_PLAYER_MOVE_END 7;

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