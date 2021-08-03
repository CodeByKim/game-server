#pragma once
#include <NetworkLib.h>

#include "./Contents/RPGGameLogic.h"

#define PACKET_SC_CREATE_MY_PLAYER 0;
#define PACKET_SC_CREATE_OTHER_PLAYER 1;

#define PACKET_SC_DELETE_MY_PLAYER 2;
#define PACKET_SC_DELETE_OTHER_PLAYER 3;

#define PACKET_CS_PLAYER_MOVE_START 4;
#define PACKET_CS_PLAYER_MOVE_END 5;
#define PACKET_SC_PLAYER_MOVE_START 6;
#define PACKET_SC_PLAYER_MOVE_END 7;

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

	RPGGameLogic mGameLogic;
};