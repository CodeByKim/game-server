#pragma once
#include <NetworkLib.h>

class EchoMessageHandler : public garam::net::IMessageHandler
{
public:
	EchoMessageHandler();
	~EchoMessageHandler();

	void OnClientJoin(garam::net::ClientInfo* client) override;
	void OnClientLeave(garam::net::ClientInfo* client) override;
	void OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet) override;
	void OnUpdate() override;

private:	
};