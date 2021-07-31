#include "UnityTestMessageHandler.h"

Timer::Time Timer::s_MainTimer = Timer::Time();
float speed = 20;

class Player
{
public:
	Player()
	{
		mIsMoving = false;
		mX = 0;
		mZ = 0;
	}

	bool mIsMoving;
	float mX;
	float mZ;
};

Player* gPlayer = nullptr;

UnityTestMessageHandler::UnityTestMessageHandler()
{
	Timer::Reset();	
}

UnityTestMessageHandler::~UnityTestMessageHandler()
{
}

void UnityTestMessageHandler::OnClientJoin(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On New Client : " << client->GetID();
	gPlayer = new Player();
}

void UnityTestMessageHandler::OnClientLeave(garam::net::ClientInfo* client)
{
	LOG_INFO(L"Game") << L"On Leave Client : " << client->GetID();
}

void UnityTestMessageHandler::OnPacketReceive(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{		
	short protocol;	
	*packet >> protocol;

	switch (protocol)
	{
	case CS_PLAYER_MOVE_START:
		PacketPlayerMoveStart(client, packet);
		break;

	case CS_PLAYER_MOVE_END:
		PacketPlayerMoveEnd(client, packet);
		break;
	}
}

void UnityTestMessageHandler::OnUpdate()
{
	//TODO : delta time은 나중에 OnUpdate의 인자로 넘겨주자...
	Timer::Update();

	//LOG_INFO(L"Game") << Timer::DeltaTime();

	if (gPlayer != nullptr)
	{
		//무조건 오른쪽으로만 이동한다고 하자.. 원래는 방향도 같이 가야하겠지만...
		if (gPlayer->mIsMoving)
		{			
			gPlayer->mX += speed * Timer::DeltaTime();
		}
	}
}

#pragma region Packet Func
void UnityTestMessageHandler::PacketPlayerMoveStart(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{
	float x;
	float z;
	*packet >> x >> z;

	gPlayer->mIsMoving = true;
}

void UnityTestMessageHandler::PacketPlayerMoveEnd(garam::net::ClientInfo* client, garam::net::NetPacket* packet)
{
	float x;
	float z;
	*packet >> x >> z;

	gPlayer->mIsMoving = false;

	garam::net::NetPacket* sendPacket = garam::net::PacketAllocator::GetInstance().Alloc();
	short protocol = SC_PLAYER_MOVE_END;
	*sendPacket << protocol << gPlayer->mX << gPlayer->mZ;

	client->SendPacket(sendPacket);
	garam::net::PacketAllocator::GetInstance().Free(sendPacket);
}
#pragma endregion