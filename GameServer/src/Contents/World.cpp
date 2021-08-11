#include "./Contents/World.h"
#include "Contents/Player.h"

World::World()
{
	for (int y = 0; y < SECTOR_COUNT_Y; y++)
	{
		for (int x = 0; x < SECTOR_COUNT_X; x++)
		{
			mSectors[y][x].x = x;
			mSectors[y][x].y = y;

			mSectors[y][x].players.clear();
		}
	}
}

World::~World()
{
}

void World::AddPlayer(Player* player)
{
	int sectorX = (int)(player->GetPosition().x / SECTOR_SIZE);
	int sectorY = (int)(player->GetPosition().y / SECTOR_SIZE);

	mSectors[sectorY][sectorX].players.push_back(player);
	player->SetSectorPosition(sectorX, sectorY);
	mPlayers.push_back(player);
}

void World::RemovePlayer(Player* player)
{
	int sectorX = (int)(player->GetPosition().x / SECTOR_SIZE);
	int sectorY = (int)(player->GetPosition().y / SECTOR_SIZE);

	mSectors[sectorY][sectorX].players.remove(player);
	
	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		if ((*iter)->GetID() == player->GetID())
		{
			mPlayers.erase(iter);
			break;
		}
	}
}

void World::Broadcast(garam::net::NetPacket* packet, Player* player)
{
	int sectorX = player->GetSectorPosition().x;
	int sectorY = player->GetSectorPosition().y;

	Sector* aroundSectors[9] = { nullptr, };
	GridLocation offset[] = {
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

		if (x < 0 || x >= 64 || y < 0 || y >= 64)
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
			if (otherPlayer->GetID() == player->GetID())
				continue;
			
			otherPlayer->GetClientInfo()->SendPacket(packet);			
		}
	}	
}

void World::GetAroundSector(Player* player, std::vector<Sector*>* outAroundSectors)
{
	GridLocation grid = player->GetSectorPosition();

	GridLocation offset[] = {
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

		if (x < 0 || x > 64 || y < 0 || y > 64)
			continue;
		
		outAroundSectors->push_back(&mSectors[y][x]);
	}
}

void World::Update()
{
	for (auto iter = mPlayers.begin(); iter != mPlayers.end(); ++iter)
	{
		Player* player = *iter;

		if (!player->IsMove())
			continue;

		GridLocation oldPos = player->GetSectorPosition();
		GridLocation currentPos = {
			(int)(player->GetPosition().x / SECTOR_SIZE),
			(int)(player->GetPosition().y / SECTOR_SIZE)
		};

		if (oldPos == currentPos)
			continue;

		//���� ������Ʈ!
		mSectors[oldPos.y][oldPos.x].players.remove(player);
		mSectors[currentPos.y][currentPos.x].players.push_back(player);
		player->SetSectorPosition(currentPos.x, currentPos.y);

		//�¿��� ��� �̵��ߴٸ� diff�� ����� ��������
		//�쿡�� �·� �̵��ߴٸ� diff�� ������ ����
		GridLocation diff = currentPos - oldPos;

		//���� ����ó���� ������...
		//���� �ֺ� 9�� ���� ��������
		Sector* leaveSectors[3][3] = { nullptr, };
		Sector* enterSectors[3][3] = { nullptr, };
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				//������ �ִ� �۾�			
				if (oldPos.x + x < 0 || oldPos.x + x >= 64 ||
					oldPos.y + y < 0 || oldPos.y + y >= 64)
					continue;

				leaveSectors[y + 1][x + 1] = &mSectors[oldPos.y + y][oldPos.x + x];
			}
		}

		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				if (currentPos.x + x < 0 || currentPos.x + x >= 64 ||
					currentPos.y + y < 0 || currentPos.y + y >= 64)
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

		//���� oldSectors���� ���� �޽����� ������ �Ǵ� ���͵��� ���Ұ�
		//updateSectors���� ���� �޽����� ������ �Ǵ� ���͵��� ����
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
