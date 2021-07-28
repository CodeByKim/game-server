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
	 * ������ Alloc�ؼ� �Ѱ��� Packet�� ������ �˾Ƽ� Free�Ѵ�.
	 */
	char data[8];
	packet->GetData(data, 8);	
	
	garam::net::NetPacket* sendPacket = garam::net::PacketAllocator::GetInstance().Alloc();
	sendPacket->PutData(data, 8);
		
	client->SendPacket(sendPacket);

	/*
	 * ����ڰ� Alloc�Ѱ� ����ڰ� Free�ؾ��Ѵ�.
	 */
	garam::net::PacketAllocator::GetInstance().Free(sendPacket);
}

void EchoMessageHandler::OnUpdate()
{
}
