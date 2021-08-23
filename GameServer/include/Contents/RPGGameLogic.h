#pragma once
#include <NetworkLib.h>
#include "./Common/GameCommon.h"
#include "./Contents/RpgGameWorld.h"
class Player;

class RPGGameLogic : public garam::net::GameLogic
{
public:
	RPGGameLogic();
	~RPGGameLogic();

	void OnUpdate(float deltaTime) override;
	void AddNewPlayer(garam::net::ClientInfo* info, bool isDummy = false);
	void LeavePlayer(garam::net::ClientInfo* info);
	void PlayerMoveStart(int id, BYTE dir, float x, float y);
	void PlayerMoveEnd(int id, BYTE dir, float x, float y);	
	void PlayerAttack(int id, BYTE dir, float x, float y);	

	Player* GetPlayer(int id);
	RpgGameWorld* GetWorld();

private:
	Player* CreatePlayer(garam::net::ClientInfo* info);
	void ReleasePlayer(Player* player);
	bool IsContainPlayer(int id);			
	void CheckPlayerSyncPosition(Player* player, float x, float y);

	std::unordered_map<int, Player*> mPlayers;
	std::list<Player*> mDeletedPlayers;
	garam::memory::MemoryPool<Player> mPlayerPool;
	RpgGameWorld mGameWorld;	
};