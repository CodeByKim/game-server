#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"

RPGGameLogic::RPGGameLogic()
{
	
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{
	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		Player* player = iter->second;
		player->OnUpdate(deltaTime);

		// �׽�Ʈ
		mWorld.Update();

		//mWorld.UpdateSector(player,
		//	[&](Player* otherPlayer) {
		//		{					
		//			/*
		//			 * otherPlayer���� player�� ������ٰ� �����ض�
		//			 */
		//			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		//			short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
		//			int id = player->GetID();
		//			*packet << protocol << id;
		//			otherPlayer->GetClientInfo()->SendPacket(packet);
		//			garam::net::NetPacket::Free(packet);
		//		}
		//		{
		//			/*
		//			 * player���� otherPlayer�� �����Ǿ��ٰ� �����ض�
		//			 */
		//			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		//			short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
		//			int id = otherPlayer->GetID();
		//			*packet << protocol << id;
		//			player->GetClientInfo()->SendPacket(packet);
		//			garam::net::NetPacket::Free(packet);
		//		}
		//	},
		//	[&](Player* otherPlayer) {
		//		//SC_CREATE_OTHER_CHARACTER(otherPlayer->GetID(), player);
		//		//SC_CREATE_OTHER_CHARACTER(player->GetID(), otherPlayer);

		//		//���ο� OTHER ĳ���͵��� �����̴� ���̾��ٸ�?
		//		//�ϴ� �� �κ��� ���߿� ó������.. ��� �ϴ��� �����ϴϱ�...

		//		/*if (player->IsMove())
		//		{
		//			SC_MOVE_START(otherPlayer->GetID(), player);
		//		}*/

		//		{
		//			/*
		//			 * otherPlayer���� player�� ���� �����Ǿ��ٰ� �����ض�
		//			 */
		//			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		//			short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
		//			int id = player->GetID();
		//			BYTE dir = player->GetDirection();
		//			Position playerPos = player->GetPosition();
		//			*packet << protocol << id << dir << playerPos.x << playerPos.y;

		//			otherPlayer->GetClientInfo()->SendPacket(packet);
		//			garam::net::NetPacket::Free(packet);
		//		}
		//		{
		//			/*
		//			 * player���� otherPlayer�� ���� �����Ǿ��ٰ� �����ض�
		//			 */
		//			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		//			short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
		//			int id = otherPlayer->GetID();
		//			BYTE dir = otherPlayer->GetDirection();
		//			std::cout << (int)dir << std::endl;
		//			Position playerPos = otherPlayer->GetPosition();
		//			*packet << protocol << id << dir << playerPos.x << playerPos.y;

		//			player->GetClientInfo()->SendPacket(packet);
		//			garam::net::NetPacket::Free(packet);
		//		}

		//		if (player->IsMove())
		//		{					
		//			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		//			short protocol = PACKET_SC_PLAYER_MOVE_START;
		//			int id = player->GetClientInfo()->GetID();
		//			BYTE dir = player->GetDirection();					
		//			Position playerPos = player->GetPosition();
		//			*packet << protocol << id << dir << playerPos.x << playerPos.y;

		//			otherPlayer->GetClientInfo()->SendPacket(packet);
		//			garam::net::NetPacket::Free(packet);					
		//		}

		//		if (otherPlayer->IsMove())
		//		{
		//			garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		//			short protocol = PACKET_SC_PLAYER_MOVE_START;
		//			int id = otherPlayer->GetClientInfo()->GetID();
		//			BYTE dir = otherPlayer->GetDirection();
		//			Position playerPos = otherPlayer->GetPosition();
		//			*packet << protocol << id << dir << playerPos.x << playerPos.y;

		//			player->GetClientInfo()->SendPacket(packet);
		//			garam::net::NetPacket::Free(packet);
		//		}
		//	}
		//);
	}

	for (auto iter = mDeletedPlayers.begin(); iter != mDeletedPlayers.end(); ++iter)
	{
		int id = (*iter)->GetID();
		mPlayers.erase(id);
	}

	mDeletedPlayers.clear();
}

void RPGGameLogic::AddNewPlayer(garam::net::ClientInfo* info)
{
	Player* player = CreatePlayer(info);
	mPlayers.insert(std::pair(info->GetID(), player));
	mWorld.AddPlayer(player);

	SendCreateMyPlayer(player);
	SendExistingPlayerInfo(player);
	BroadcastCreateOtherPlayer(player);
}

void RPGGameLogic::LeavePlayer(garam::net::ClientInfo* info)
{
	/*
	 * �켱 mPlayers���� ����, 
	 * ���� �������� �÷��̾ info�� �ش��ϴ� ������ ������ٰ� �˷���� ��
	 */
	Player* player = GetPlayer(info->GetID());
	mDeletedPlayers.push_back(player);
	mWorld.RemovePlayer(player);

	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_DELETE_OTHER_PLAYER;
	int id = player->GetID();	
	*packet << protocol << id;

	BroadcastPacket(packet, info);
}

void RPGGameLogic::PlayerMoveStart(int id, BYTE dir, short x, short y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);

	//BroadcastPlayerMoveStart(player);
	
	mWorld.PerformAroundPlayers(player, [&](Player* otherPlayer)
	{
		//SC_MOVE_START(otherPlayer->GetID(), player);

		garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		short protocol = PACKET_SC_PLAYER_MOVE_START;
		int id = player->GetClientInfo()->GetID();
		BYTE dir = player->GetDirection();
		Position playerPos = player->GetPosition();
		*packet << protocol << id << dir << playerPos.x << playerPos.y;

		otherPlayer->GetClientInfo()->SendPacket(packet);
		garam::net::NetPacket::Free(packet);
	});
}

void RPGGameLogic::PlayerMoveEnd(int id, BYTE dir, short x, short y)
{	
	Player* player = GetPlayer(id);
	player->MoveEnd(dir, x, y);

	//BroadcastPlayerMoveEnd(player);
	mWorld.PerformAroundPlayers(player, [&](Player* otherPlayer)
	{
		//SC_MOVE_STOP(otherPlayer->GetID(), player);

		garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		short protocol = PACKET_SC_PLAYER_MOVE_END;
		int id = player->GetClientInfo()->GetID();
		BYTE dir = player->GetDirection();
		Position playerPos = player->GetPosition();
		*packet << protocol << id << dir << playerPos.x << playerPos.y;

		otherPlayer->GetClientInfo()->SendPacket(packet);
		garam::net::NetPacket::Free(packet);
	});
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { (float)(rand() % 15), (float)(rand() % 15) };
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

void RPGGameLogic::BroadcastPacket(garam::net::NetPacket* packet, garam::net::ClientInfo* exceptClient)
{	
	for(auto iter = mPlayers.begin() ; iter != mPlayers.end() ; ++iter)
	{		
		garam::net::ClientInfo* info = iter->second->GetClientInfo();
		
		if (info->GetID() == exceptClient->GetID())
		{
			continue;
		}

		info->SendPacket(packet);
	}
}

void RPGGameLogic::SendCreateMyPlayer(Player* player)
{
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_MY_PLAYER;
	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();
	*packet << protocol << id << dir << playerPos.x << playerPos.y;	
	player->GetClientInfo()->SendPacket(packet);
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::SendExistingPlayerInfo(Player* player)
{
	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{		
		Player* otherPlayer = iter->second;

		if (player->GetID() == otherPlayer->GetID())
		{
			continue;
		}

		garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
		short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
		int id = otherPlayer->GetID();
		BYTE dir = otherPlayer->GetDirection();
		Position playerPos = otherPlayer->GetPosition();
		*packet << protocol << id << dir << playerPos.x << playerPos.y;

		player->GetClientInfo()->SendPacket(packet);
		garam::net::NetPacket::Free(packet);
	}	
}

void RPGGameLogic::BroadcastCreateOtherPlayer(Player* player)
{
	//TODO : ���߿��� Sector�� �����ؾ� ��
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_CREATE_OTHER_PLAYER;
	int id = player->GetClientInfo()->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();		
	*packet << protocol << id << dir << playerPos.x << playerPos.y;

	//garam::net::NetworkComponent::BroadCast(packet, player->GetClientInfo());
	BroadcastPacket(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveStart(Player* player)
{
	/*	 
	 * TODO : ���߿��� Sector�� �����ؾ� ��
	 */		 
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_START;
	int id = player->GetClientInfo()->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();	
	*packet << protocol << id << dir << playerPos.x << playerPos.y;
	
	BroadcastPacket(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}

void RPGGameLogic::BroadcastPlayerMoveEnd(Player* player)
{
	/*
	 * TODO : ���߿��� Sector�� �����ؾ� ��
	 */
	garam::net::NetPacket* packet = garam::net::NetPacket::Alloc();
	short protocol = PACKET_SC_PLAYER_MOVE_END;	
	int id = player->GetClientInfo()->GetID();
	BYTE dir = player->GetDirection();
	Position playerPos = player->GetPosition();
	*packet << protocol << id << dir << playerPos.x << playerPos.y;

	BroadcastPacket(packet, player->GetClientInfo());
	garam::net::NetPacket::Free(packet);
}
