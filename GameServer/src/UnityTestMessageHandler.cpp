#include "UnityTestMessageHandler.h"

UnityTestMessageHandler::UnityTestMessageHandler()
{
}

UnityTestMessageHandler::~UnityTestMessageHandler()
{
}

void UnityTestMessageHandler::OnClientJoin(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On New Client : " << client->GetID();
}

void UnityTestMessageHandler::OnClientLeave(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On Leave Client : " << client->GetID();
}

void UnityTestMessageHandler::OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{		
	int testData1;
	int testData2;
	int testData3;
	*packet >> testData1 >> testData2 >> testData3;

	LOG_INFO(L"Game") << L"Packet Receive : " << testData1 << "," << testData2 << "," << testData3;

	garam::net::NetPacket* sendPacket = garam::net::PacketAllocator::GetInstance().Alloc();

	int sendTestData1 = 100;
	int sendTestData2 = 200;
	int sendTestData3 = 300;
	*sendPacket << sendTestData1 << sendTestData2 << sendTestData3;

	client->SendPacket(sendPacket);

	garam::net::PacketAllocator::GetInstance().Free(sendPacket);
}

void UnityTestMessageHandler::OnUpdate()
{

}
