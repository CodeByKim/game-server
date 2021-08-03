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

		//TODO : BroadCast 함수가 필요한데..		
		client->SendPacket(otherSendPacket, garam::net::eSendTarget::Broadcast);
		garam::net::NetPacket::Free(otherSendPacket);
	}	

	/*
	 * broadcast 관련 함수는 라이브러리 차원에서 제공해주는게 맞는거 같은데.. 
	 * 만약 ID만 부여한다면? 이 ID를 가지고 모든걸 얻어올 수 있어야 하는데
	 * 
	 * 근데 ID를 주면 매번 찾아와야하는 단점이 있음...
	 * 애초에 프록시 객체를 주면 다음에는 검색해야 하는 단점은 없음
	 * 
	 * SendPacket 함수를 오버로딩해서 BroadCast 버전을 만들자.
	 * enum class Target
	 * {
	 *		Single,
	 *		Sector,
	 *		Broadcast
	 * }
	 * 
	 * client.SendPacket(packet, Target::Sector, false);
	 * 마지막 인자의 bool형은 나도 포함해서 전송할것인가, 나는 제외하고 보낼 것인가, default 값은 false로 하면 됨
	 */	
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
