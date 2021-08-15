#include "./Contents/RPGGameLogic.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"
#include "./Contents/World.h"

RPGGameLogic::RPGGameLogic()
{
	mWorld.Create(200, 200, 10);	
}

RPGGameLogic::~RPGGameLogic()
{
}

void RPGGameLogic::Update(float deltaTime)
{			
	mWorld.OnUpdate(deltaTime);

	for (auto iter = mPlayers.begin(); 
		 iter != mPlayers.end(); 
		 ++iter)
	{				
		Player* player = iter->second;
		player->OnUpdate(deltaTime);
	}

	for (auto iter = mDeletedPlayers.begin(); 
		 iter != mDeletedPlayers.end(); 
		 ++iter)
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
	
	BROADCAST_REMOVE_OTHER_PLAYER(mWorld, 
								  player->GetID(), 
								  player);
}

void RPGGameLogic::PlayerMoveStart(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveStart(dir, x, y);
		
	//TODO : ���� Ŭ�� ������ �ϴ� ���
	//CheckPlayerSyncPosition(player, x, y);

	BROADCAST_PLAYER_MOVE_START(mWorld, 
								player->GetID(), 
								player->GetDirection(), 
								player->GetPosition().x, 
								player->GetPosition().y, 
								player);
}

void RPGGameLogic::PlayerMoveEnd(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);
	player->MoveEnd(dir, x, y);
			
	//TODO : ���� Ŭ�� ������ �ϴ� ���
	//CheckPlayerSyncPosition(player, x, y);

	BROADCAST_PLAYER_MOVE_END(mWorld, 
							  player->GetID(), 
							  player->GetDirection(), 
							  player->GetPosition().x, 
							  player->GetPosition().y, 
							  player);
}

void RPGGameLogic::PlayerAttack(int id, BYTE dir, float x, float y)
{
	Player* player = GetPlayer(id);
	
	//�ǰݴ��� ���� ����ؾ� ��
	Sector* sector = mWorld.GetSector(player);
	auto monsters = sector->monsters;
	Monster* hitMonster = nullptr;

	for (auto iter = monsters.begin(); 
		 iter != monsters.end(); 
		 ++iter)
	{
		Monster* monster = *iter;

		Position playerPos = player->GetPosition();
		Position monsterPos = monster->GetPosition();

		// 1. �ϴ� �Ÿ��� ����� ��鸸 �߸���.		
		Position diff = monsterPos - playerPos;

		if (abs(diff.x) <= 1 && abs(diff.y) <= 1)
		{
			// 2. �׸��� ������ �ٶ󺸴��� Ȯ������.
			 			
			if (player->GetDirection() == MOVE_DIR_LEFT)
			{
				//���� ������ �ٶ� �� ���
				//���ʹ� ������ ���ʿ� �־�߸� ��
				if (monsterPos.x - playerPos.x < 0)
				{
					hitMonster = monster;
					break;
				}
			}
			else if (player->GetDirection() == MOVE_DIR_UP)
			{
				//���� ������ �ٶ� �� ���
				//���ʹ� ������ ���� �־�߸� ��
				if (monsterPos.y - playerPos.y > 0)
				{
					hitMonster = monster;
					break;
				}
			}
			else if (player->GetDirection() == MOVE_DIR_RIGHT)
			{
				//���� �������� �ٶ� �� ���
				//���ʹ� ������ �����ʿ� �־�߸� ��
				if (monsterPos.x - playerPos.x > 0)
				{					
					hitMonster = monster;
					break;
				}
			}
			else if (player->GetDirection() == MOVE_DIR_DOWN)
			{
				//���� �Ʒ����� �ٶ� �� ���
				//���ʹ� ������ �Ʒ��� �־�߸� ��
				if (monsterPos.y - playerPos.y < 0)
				{
					hitMonster = monster;
					break;
				}
			}
		}
	}

	if (hitMonster != nullptr)
	{
		/*
		 * ���͸� ã������ �ش� ������ HP�� ���,  
		 * ���� �ǰ� ��Ŷ�� broadcast �Ѵ�.
		 */
		
		hitMonster->Hit();
		BROADCAST_HIT_MONSTER(mWorld, 
							  hitMonster->GetID(), 
							  hitMonster->GetHP());
	}

	BROADCAST_PLAYER_ATTACK(mWorld,
							player->GetID(),
							player->GetDirection(),
							player->GetPosition().x,
							player->GetPosition().y,
							player);

	//���� Ư�� ���Ͱ� �ǰݵǾ��ٸ� �ǰ� ��Ŷ�� �������Ѵ�.
}

void RPGGameLogic::TeleportPlayer(int id, BYTE dir, float x, float y)
{	
	Player* player = GetPlayer(id);

	player->Teleport(dir, x, y);	
	mWorld.ChangeSectorAndNotifyMessageToPlayer(player, x, y);
}

Player* RPGGameLogic::CreatePlayer(garam::net::ClientInfo* client)
{		
	Player* player = mPlayerPool.Alloc();	
	Position pos = { (float)(rand() % 1500), (float)(rand() % 1500) };
	player->Initialize(client, pos, this);

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

void RPGGameLogic::CheckPlayerSyncPosition(Player* player, float x, float y)
{
	Position playerPos = player->GetPosition();
	float xOffset = abs(playerPos.x - x);
	float yOffset = abs(playerPos.y - y);

	if (xOffset >= 1 || yOffset >= 1)
	{
		BROADCAST_SYNC_POSITION(mWorld,
								player->GetID(),
								playerPos.x,
								playerPos.y);
	}
}
