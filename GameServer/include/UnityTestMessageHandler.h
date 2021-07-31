#pragma once
#include "./NetworkLib.h"

class UnityTestMessageHandler : public garam::net::IMessageHandler
{
public:
	UnityTestMessageHandler();
	~UnityTestMessageHandler();

	void OnClientJoin(garam::net::ClientInfo* client) override;
	void OnClientLeave(garam::net::ClientInfo* client) override;
	void OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet) override;
	void OnUpdate() override;

private:
};