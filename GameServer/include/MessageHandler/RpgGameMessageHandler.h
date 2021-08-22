#pragma once
#include <NetworkLib.h>

class RPGGameLogic;

class RpgGameMessageHandler : public garam::net::IMessageHandler
{
public:
	RpgGameMessageHandler();
	~RpgGameMessageHandler();

	void OnClientJoin(garam::net::ClientInfo* info) override;
	void OnClientLeave(garam::net::ClientInfo* info) override;
	void OnPacketReceive(garam::net::ClientInfo* info, garam::net::NetPacket* packet) override;
	void OnUpdate(float deltaTime) override;

	RPGGameLogic* mGameLogic;

private:
	void PacketPlayerMoveStart(garam::net::ClientInfo* info, garam::net::NetPacket* packet);
	void PacketPlayerMoveEnd(garam::net::ClientInfo* info, garam::net::NetPacket* packet);
	void PacketPlayerAttack(garam::net::ClientInfo* info, garam::net::NetPacket* packet);
	void PacketTeleportPleyer(garam::net::ClientInfo* info, garam::net::NetPacket* packet);
};