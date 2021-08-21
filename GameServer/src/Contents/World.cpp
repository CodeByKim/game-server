#include "./Contents/World.h"
#include "./Contents/Player.h"
#include "./Common/Protocol.h"
World::World()
	: mSectorSize(0)
	, mSectorCountX(0)
	, mSectorCountY(0)
	, mSectors(nullptr)
	, mMonsterManager(this)
{		
}

World::~World()
{
	for (int y = 0; y < mSectorCountY; y++)
	{
		delete[] mSectors[y];
	}

	delete[] mSectors;
}

void World::Create(int sectorCountX, int sectorCountY, int sectorSize)
{
	mSectorSize = sectorSize;
	mSectorCountX = sectorCountX;
	mSectorCountY = sectorCountY;

	mSectors = new Sector*[mSectorCountY];
	for (int y = 0; y < mSectorCountY; y++)
	{
		mSectors[y] = new Sector[mSectorCountX];

		for (int x = 0; x < mSectorCountX; x++)
		{
			mSectors[y][x].x = x;
			mSectors[y][x].y = y;

			mSectors[y][x].players.clear();
		}		
	}

	for (int i = 0; i < MAX_MONSTER_COUNT; i++)
	{
		AddMonster(mMonsterManager.GetMonster(i));
	}
}

void World::AddPlayer(Player* player)
{
	int sectorX = (int)(player->GetPosition().x / mSectorSize);
	int sectorY = (int)(player->GetPosition().y / mSectorSize);

	mSectors[sectorY][sectorX].players.push_back(player);
	player->SetSectorPosition(sectorX, sectorY);
	mPlayers.push_back(player);

	int id = player->GetID();
	BYTE dir = player->GetDirection();
	Position2D playerPos = player->GetPosition();

	SEND_CREATE_MY_PLAYER(*player->GetClientInfo(),
						  id,
						  dir,
						  playerPos.x,
						  playerPos.y);

	SendPlayerInfoContainedInSector(player);

	BROADCAST_CREATE_OTHER_PLAYER(*this,
								  id,
								  dir,
								  playerPos.x,
								  playerPos.y,
								  player);

	/*
	 * ���� ������ ����� ��	 
	 */
	SendMonsterInfoContainedInSector(player);
}

void World::RemovePlayer(Player* player)
{	
	Position2DInt sectorPos = player->GetSectorPosition();
	mSectors[sectorPos.y][sectorPos.x].players.remove(player);
	
	for (auto iter = mPlayers.begin(); 
		 iter != mPlayers.end(); 
		 ++iter)
	{
		if ((*iter)->GetID() == player->GetID())
		{
			mPlayers.erase(iter);
			break;
		}
	}
}

void World::Broadcast(garam::net::NetPacket* packet, Player* sender, bool exceptSender)
{	
	int sectorX = sender->GetSectorPosition().x;
	int sectorY = sender->GetSectorPosition().y;

	Sector* aroundSectors[9] = { nullptr, };
	Position2DInt offset[] = {
		{0, 0},
		{-1, 0},
		{-1, -1},
		{0, -1},
		{1, -1},
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}
	};

	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		int x = sectorX + offset[i].x;
		int y = sectorY + offset[i].y;

		if (x < 0 || x >= mSectorCountX || 
			y < 0 || y >= mSectorCountY)
			continue;

		aroundSectors[count] = &mSectors[y][x];
		count += 1;
	}

	for (int i = 0; i < count; i++)
	{		
		for (auto iter = aroundSectors[i]->players.begin();
			 iter != aroundSectors[i]->players.end(); 
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (exceptSender)
			{
				if (otherPlayer->GetID() == sender->GetID())
				{
					continue;
				}					
			}
				
			otherPlayer->GetClientInfo()->SendPacket(packet);						
		}
	}
}

Sector* World::GetSector(Player* player)
{
	Position2DInt grid = player->GetSectorPosition();
	return &mSectors[grid.y][grid.x];
}

Sector* World::GetSector(Monster* monster)
{
	Position2DInt grid = monster->GetSectorPosition();
	return &mSectors[grid.y][grid.x];
}

void World::GetAroundSector(Player* player, std::vector<Sector*>* outAroundSectors)
{
	Position2DInt grid = player->GetSectorPosition();

	Position2DInt offset[] = {
		   {0, 0},
		   {-1, 0},
		   {-1, -1},
		   {0, -1},
		   {1, -1},
		   {1, 0},
		   {1, 1},
		   {0, 1},
		   {-1, 1}
	};

	Sector sector = mSectors[grid.y][grid.x];

	for (int i = 0; i < 9; i++)
	{
		int x = sector.x + offset[i].x;
		int y = sector.y + offset[i].y;

		if (x < 0 || x > mSectorCountX || 
			y < 0 || y > mSectorCountY)
			continue;
		
		outAroundSectors->push_back(&mSectors[y][x]);
	}
}

void World::GetAroundSector(Monster* monster, std::vector<Sector*>* outAroundSectors)
{
	Position2DInt grid = monster->GetSectorPosition();

	Position2DInt offset[] = {
		   {0, 0},
		   {-1, 0},
		   {-1, -1},
		   {0, -1},
		   {1, -1},
		   {1, 0},
		   {1, 1},
		   {0, 1},
		   {-1, 1}
	};

	Sector sector = mSectors[grid.y][grid.x];

	for (int i = 0; i < 9; i++)
	{
		int x = sector.x + offset[i].x;
		int y = sector.y + offset[i].y;

		if (x < 0 || x > mSectorCountX ||
			y < 0 || y > mSectorCountY)
			continue;

		outAroundSectors->push_back(&mSectors[y][x]);
	}
}

void World::OnUpdate(float deltaTime)
{	
	mMonsterManager.OnUpdate(deltaTime);

	for(int i = 0 ; i < mPlayers.size() ; i++)
	{		
		Player* player = mPlayers[i];

		// �÷��̾ �̵������� �����鼭
		// ���ÿ� ���� ������Ʈ�� �ʿ���� ��� ����
		if (!player->IsMove())
			continue;

		Position2DInt oldPos = player->GetSectorPosition();
		Position2DInt currentPos = {
			(int)(player->GetPosition().x / mSectorSize),
			(int)(player->GetPosition().y / mSectorSize)
		};

		if (oldPos == currentPos)
		{
			continue;			
		}
							
		//���� ������Ʈ!
		mSectors[oldPos.y][oldPos.x].players.remove(player);
		mSectors[currentPos.y][currentPos.x].players.push_back(player);

		player->SetSectorPosition(currentPos.x, currentPos.y);

		//�¿��� ��� �̵��ߴٸ� diff�� ����� ��������
		//�쿡�� �·� �̵��ߴٸ� diff�� ������ ����
		Position2DInt diff = currentPos - oldPos;

		//���� ����ó���� ������...
		//���� �ֺ� 9�� ���� ��������
		Sector* leaveSectors[3][3] = { nullptr, };
		Sector* enterSectors[3][3] = { nullptr, };
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				//������ �ִ� �۾�			
				if (oldPos.x + x < 0 || oldPos.x + x >= mSectorCountX ||
					oldPos.y + y < 0 || oldPos.y + y >= mSectorCountY)
					continue;

				leaveSectors[y + 1][x + 1] = &mSectors[oldPos.y + y][oldPos.x + x];
			}
		}

		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				if (currentPos.x + x < 0 || currentPos.x + x >= mSectorCountX ||
					currentPos.y + y < 0 || currentPos.y + y >= mSectorCountY)
					continue;

				enterSectors[y + 1][x + 1] = &mSectors[currentPos.y + y][currentPos.x + x];
			}
		}

		//���� �̷��� �ϸ� ���ʿ��� ���������� �̵���������..
		int diffX = diff.x;
		if (diff.x >= 3)
			diffX = 3;

		if (diff.x <= -3)
			diffX = -3;

		int diffY = diff.y;
		if (diff.y >= 3)
			diffY = 3;

		if (diff.y <= -3)
			diffY = -3;

		//�ϴ� �ֺ����͵� �� �־���...
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				//����� X�� ��ȭ�� ������ ��
				//������ �͸� ����� �������� nullptr�� �����ؾ� ��
				//1ĭ �����̸� 2�پ� ������			
				if (diffX > 0)		//���ʿ��� ���������� �̵��� ���̽�
				{
					if (x >= diffX)
						leaveSectors[y][x] = nullptr;

					if (x <= diffX)
						enterSectors[y][x] = nullptr;
				}
				else if (diffX < 0)		//�����ʿ��� �������� �̵��� ���̽�
				{
					//���... x�� 0,1�� ���;���
					int count = diffX * -1;		//1
					//���� ���� �����ؾ� ��
					if (x <= count)
						leaveSectors[y][x] = nullptr;

					//�ڿ� ���� �����ؾ� ��
					if (x >= count)
						enterSectors[y][x] = nullptr;
				}

				if (diffY > 0)
				{
					if (y >= diffY)
						leaveSectors[y][x] = nullptr;

					if (y <= diffY)
						enterSectors[y][x] = nullptr;
				}
				else if (diffY < 0)
				{
					//�Ʒ����� ���� �̵����� ���̽�
					int count = diffY * -1;		//1

					//�� ���� �����ؾ� ��
					if (y <= count)
						leaveSectors[y][x] = nullptr;

					//�Ʒ� ���� �����ؾ� ��
					if (y >= count)
						enterSectors[y][x] = nullptr;
				}
			}
		}

		//���� leaveSectors���� ���� �޽����� ������ �Ǵ� ���͵��� ���Ұ�
		//enterSectors���� ���� �޽����� ������ �Ǵ� ���͵��� ����
		std::vector<Sector*> leaves;
		std::vector<Sector*> enters;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				Sector* leave = leaveSectors[y][x];
				if (leave != nullptr)
					leaves.push_back(leave);

				Sector* enter = enterSectors[y][x];
				if (enter != nullptr)
					enters.push_back(enter);
			}
		}		

		player->OnSectorChanged(leaves, enters);
	}	
}

void World::ChangeSectorAndNotifyMessageToPlayer(Player* player, float x, float y)
{
	BROADCAST_REMOVE_OTHER_PLAYER(*this,
								  player->GetID(),
								  player);

	//2. ���͸� ü�����ϰ�
	ChangeSector(player, x, y);

	//3. ���ο� ���Ϳ� �޽��� ������
	SendPlayerInfoContainedInSector(player);

	BROADCAST_CREATE_OTHER_PLAYER(*this,
								  player->GetID(),
								  player->GetDirection(),
								  x,
								  y,
								  player);
}

void World::AddMonster(Monster* monster)
{
	//���Ϳ� �ְ�
	//���� ���� ��ε�ĳ����

	int sectorX = (int)(monster->GetPosition().x / mSectorSize);
	int sectorY = (int)(monster->GetPosition().y / mSectorSize);

	mSectors[sectorY][sectorX].monsters.push_back(monster);
	monster->SetSectorPosition(sectorX, sectorY);	
}

void World::DeadMonster(Monster* monster)
{
	//���Ϳ����� �����ؾ� ��...
	Sector* sector = GetSector(monster);
	sector->monsters.remove(monster);

	mMonsterManager.DeadMonster(monster);	
}

void World::ChangeSector(Player* player, float x, float y)
{	
	Position2DInt oldPos = player->GetSectorPosition();
	Position2DInt currentPos = {
		(int)(x / mSectorSize),
		(int)(y / mSectorSize)
	};

	if (oldPos == currentPos)
	{
		return;
	}

	//���� ������Ʈ!
	mSectors[oldPos.y][oldPos.x].players.remove(player);
	mSectors[currentPos.y][currentPos.x].players.push_back(player);
	player->SetSectorPosition(currentPos.x, currentPos.y);	
}

void World::SendPlayerInfoContainedInSector(Player* player)
{
	std::vector<Sector*> aroundSectors;
	GetAroundSector(player, &aroundSectors);

	for (auto iter = aroundSectors.begin();
		 iter != aroundSectors.end();
		 ++iter)
	{
		Sector* sector = *iter;
		//auto players = sector->players;
		std::list<Player*>& players = sector->players;

		for (auto iter = players.begin();
			 iter != players.end();
			 ++iter)
		{
			Player* otherPlayer = *iter;

			if (player->GetID() == otherPlayer->GetID())
			{
				continue;
			}

			int id = otherPlayer->GetID();
			BYTE dir = otherPlayer->GetDirection();
			Position2D playerPos = otherPlayer->GetPosition();

			SEND_CREATE_OTHER_PLAYER(*player->GetClientInfo(),
									 id,
									 dir,
									 playerPos.x,
									 playerPos.y);

			/*
			 * ������ Ŭ�� �̵����̾��ٸ�
			 * �̵����̶�� ���� �˷��� ��
			 */
			if (otherPlayer->IsMove())
			{
				SEND_PLAYER_MOVE_START(*player->GetClientInfo(),
									  id,
									  dir,
									  playerPos.x,
									  playerPos.y);
			}
		}
	}
}

void World::SendMonsterInfoContainedInSector(Player* player)
{
	std::vector<Sector*> aroundSectors;
	GetAroundSector(player, &aroundSectors);

	for (auto iter = aroundSectors.begin();
		iter != aroundSectors.end();
		++iter)
	{
		Sector* sector = *iter;		
		std::list<Monster*>& monsters = sector->monsters;

		for (auto iter = monsters.begin();
			 iter != monsters.end();
			 ++iter)
		{
			Monster* monster = *iter;

			SEND_CREATE_MONSTER(*player->GetClientInfo(), 
								monster->GetID(),
								monster->GetDirection(),								
								monster->GetPosition().x,
								monster->GetPosition().y);

			/*
			 * �� ���Ͱ� �̵����̾��ٸ�
			 * �̵����̶�� ���� �˷��� ��
			 */
			/*if (monster->IsMove())
			{
				SEND_PLAYER_MOVE_START(*player->GetClientInfo(),
					id,
					dir,
					playerPos.x,
					playerPos.y);
			}*/
		}
	}
}
