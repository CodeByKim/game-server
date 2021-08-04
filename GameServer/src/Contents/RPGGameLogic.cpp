#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"

RPGGameLogic::RPGGameLogic()
{
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{
}

void RPGGameLogic::AddNewPlayer(garam::net::ClientInfo* client)
{
	Player* player = CreatePlayer(client);
	mPlayers.insert(std::pair(client->GetID(), player));

	garam::net::NetPacket* sendPacket = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MY_PLAYER;
	Position playerPos = player->GetPosition();
	*sendPacket << protocol << playerPos.x << playerPos.y;

	//전송하려면 ClientInfo를 얻어오야 한다.
	player->GetClientInfo()->SendPacket(sendPacket);
	garam::net::NetPacket::Free(sendPacket);

	{
		//그리고 다른 플레이어에게도 이 캐릭터가 생성되었다고 알려야 함
		//TODO : 나중에는 Sector만 전송해야 함
		garam::net::NetPacket* otherSendPacket = garam::net::NetPacket::Alloc();
		short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
		Position playerPos = player->GetPosition();
		int id = client->GetID();
		*otherSendPacket << protocol << id << playerPos.x << playerPos.y;
		
		//client->SendPacket(otherSendPacket, garam::net::eSendTarget::Broadcast);
		//garam::net::NetComponent::SendPacket(otherSendPacket, client);
		//garam::net::NetworkComponent::BroadCast(otherSendPacket);
		garam::net::NetPacket::Free(otherSendPacket);
	}	
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { rand() % 100, rand() % 100 };
	player->Initialize(client, pos);

	return player;
}

Player* RPGGameLogic::GetPlayer(int id)
{
	if (!IsContainPlayer(id))
		return nullptr;

	return mPlayers[id];
}

bool RPGGameLogic::IsContainPlayer(int id)
{
	if (mPlayers.find(id) == mPlayers.end())
		return false;

	return true;
}
