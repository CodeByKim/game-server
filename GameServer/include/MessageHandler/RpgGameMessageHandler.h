#pragma once
#include <NetworkLib.h>
#include "./Contents/RPGGameLogic.h"

class RpgGameMessageHandler : public garam::net::IMessageHandler
{
public:
	RpgGameMessageHandler();
	~RpgGameMessageHandler();

	void OnClientJoin(garam::net::ClientInfo* info) override;
	void OnClientLeave(garam::net::ClientInfo* info) override;
	void OnPacketReceive(garam::net::ClientInfo* info, garam::net::NetPacket* packet) override;
	void OnUpdate(float deltaTime) override;

private:
	void PacketPlayerMoveStart(garam::net::ClientInfo* info, garam::net::NetPacket* packet);
	void PacketPlayerMoveEnd(garam::net::ClientInfo* info, garam::net::NetPacket* packet);
	void PacketTeleportPleyer(garam::net::ClientInfo* info, garam::net::NetPacket* packet);

	RPGGameLogic mGameLogic;
};