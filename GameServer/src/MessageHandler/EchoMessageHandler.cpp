#include "./MessageHandler/EchoMessageHandler.h"

EchoMessageHandler::EchoMessageHandler()
{
}

EchoMessageHandler::~EchoMessageHandler()
{
}

void EchoMessageHandler::OnClientJoin(garam::net::ClientInfo* client)
{	
	LOG_INFO(L"Game") << L"On New Client : " << client->GetID();

	__int64 data = 0x7fffffffffffffff;
	garam::net::NetPacket* sendPacket = garam::net::NetPacket::Alloc();
	*sendPacket << data;

	client->SendPacket(sendPacket);	
}

void EchoMessageHandler::OnClientLeave(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On Leave Client : " << client->GetID();
}

void EchoMessageHandler::OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{
	/*
	 * ������ Alloc�ؼ� �Ѱ��� Packet�� ������ �˾Ƽ� Free�Ѵ�.
	 */
	char data[8];
	packet->GetData(data, 8);

	garam::net::NetPacket* sendPacket = garam::net::NetPacket::Alloc();
	sendPacket->PutData(data, 8);
	
	client->SendPacket(sendPacket);	

	/*
	 * ����ڰ� Alloc�Ѱ� ����ڰ� Free�ؾ��Ѵ�.
	 */
	garam::net::NetPacket::Free(sendPacket);
}

void EchoMessageHandler::OnUpdate(float deltaTime)
{
}
