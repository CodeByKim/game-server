#include "EchoMessageHandler.h"

EchoMessageHandler::EchoMessageHandler()
{
}

EchoMessageHandler::~EchoMessageHandler()
{
}

void EchoMessageHandler::OnClientJoin(garam::net::ClientInfo* client)
{
	std::cout << "On New Client : " << client->GetID() << std::endl;

	__int64 data = 0x7fffffffffffffff;	
	garam::net::NetPacket* sendPacket = garam::net::PacketAllocator::GetInstance().Alloc();
	*sendPacket << data;

	client->SendPacket(sendPacket);
}

void EchoMessageHandler::OnClientLeave(garam::net::ClientInfo* client)
{
	std::cout << "On Leave Client : " << client->GetID() << std::endl;
}

void EchoMessageHandler::OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{
	/*
	 * 엔진이 Alloc해서 넘겨준 Packet은 엔진이 알아서 Free한다.
	 */
	char data[8];
	packet->GetData(data, 8);	
	
	garam::net::NetPacket* sendPacket = garam::net::PacketAllocator::GetInstance().Alloc();
	sendPacket->PutData(data, 8);
		
	client->SendPacket(sendPacket);

	/*
	 * 사용자가 Alloc한건 사용자가 Free해야한다.
	 */
	garam::net::PacketAllocator::GetInstance().Free(sendPacket);
}

void EchoMessageHandler::OnUpdate()
{
}
