#pragma once
#include <NetworkLib.h>

#define CS_PLAYER_MOVE_START 0
#define CS_PLAYER_MOVE_END 1
#define SC_PLAYER_MOVE_START 2
#define SC_PLAYER_MOVE_END 3

class RpgGameMessageHandler : public garam::net::IMessageHandler
{
public:
	RpgGameMessageHandler();
	~RpgGameMessageHandler();

	void OnClientJoin(garam::net::ClientInfo* client) override;
	void OnClientLeave(garam::net::ClientInfo* client) override;
	void OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet) override;
	void OnUpdate(float deltaTime) override;

private:
	void PacketPlayerMoveStart(garam::net::ClientInfo* client, garam::net::NetPacket* packet);
	void PacketPlayerMoveEnd(garam::net::ClientInfo* client, garam::net::NetPacket* packet);
};