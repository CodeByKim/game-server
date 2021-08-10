#include "./Contents/SectorManager.h"
#include "Contents/Player.h"

SectorManager::SectorManager()
{
	for (int y = 0; y < SECTOR_COUNT_Y; y++)
	{
		for (int x = 0; x < SECTOR_COUNT_X; x++)
		{
			mSectors[y][x].x = x;
			mSectors[y][x].y = y;

			mSectors[y][x].mPlayers.clear();
		}
	}
}

SectorManager::~SectorManager()
{
}

void SectorManager::AddPlayer(Player* player)
{
	int sectorX = player->GetPosition().x / SECTOR_SIZE;
	int sectorY = player->GetPosition().y / SECTOR_SIZE;

	mSectors[sectorY][sectorX].mPlayers.push_back(player);
	player->SetSectorPosition(sectorX, sectorY);
}

void SectorManager::RemovePlayer(Player* player)
{
	int sectorX = player->GetPosition().x / SECTOR_SIZE;
	int sectorY = player->GetPosition().y / SECTOR_SIZE;

	mSectors[sectorY][sectorX].mPlayers.remove(player);
}

//���⼭ 9���� ���͸� ���Բ� �ؾ��Ѵ�.
void SectorManager::PerformAroundPlayers(Player* player, std::function<void(Player*)> func)
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
		for (auto iter = aroundSectors[i]->mPlayers.begin();
			iter != aroundSectors[i]->mPlayers.end(); ++iter)
		{
			Player* otherPlayer = *iter;
			if (otherPlayer->GetID() == player->GetID())
				continue;

			func(otherPlayer);
		}
	}
}

void SectorManager::Broadcast(garam::net::NetPacket* packet, Player* player)
{
}

void SectorManager::GetAroundSector(int x, int y, Sector** arr)
{
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

	Sector sector = mSectors[y][x];

	for (int i = 0; i < 9; i++)
	{
		int x = sector.x + offset[i].x;
		int y = sector.y + offset[i].y;

		if (x < 0 || x > 64 || y < 0 || y > 64)
			continue;

		arr[i] = &mSectors[y][x];
	}
}

void SectorManager::UpdateSector(Player* player, std::function<void(Player*)> old, std::function<void(Player*)> update)
{
	GridLocation oldPos = player->GetSectorPosition();
	GridLocation currentPos = {
		player->GetPosition().x / SECTOR_SIZE,
		player->GetPosition().y / SECTOR_SIZE
	};

	if (oldPos == currentPos)
		return;

	//���� ������Ʈ!
	mSectors[oldPos.y][oldPos.x].mPlayers.remove(player);
	mSectors[currentPos.y][currentPos.x].mPlayers.push_back(player);
	player->SetSectorPosition(currentPos.x, currentPos.y);

	//�¿��� ��� �̵��ߴٸ� diff�� ����� ��������
	//�쿡�� �·� �̵��ߴٸ� diff�� ������ ����
	GridLocation diff = currentPos - oldPos;

	//���� ����ó���� ������...
	//���� �ֺ� 9�� ���� ��������
	Sector* oldSectors[3][3] = { nullptr, };
	Sector* updateSectors[3][3] = { nullptr, };
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			//������ �ִ� �۾�			
			if (oldPos.x + x < 0 || oldPos.x + x >= 64 ||
				oldPos.y + y < 0 || oldPos.y + y >= 64)
				continue;

			oldSectors[y + 1][x + 1] = &mSectors[oldPos.y + y][oldPos.x + x];
		}
	}

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (currentPos.x + x < 0 || currentPos.x + x >= 64 ||
				currentPos.y + y < 0 || currentPos.y + y >= 64)
				continue;

			updateSectors[y + 1][x + 1] = &mSectors[currentPos.y + y][currentPos.x + x];
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
					oldSectors[y][x] = nullptr;

				if (x <= diffX)
					updateSectors[y][x] = nullptr;
			}
			else if (diffX < 0)		//�����ʿ��� �������� �̵��� ���̽�
			{
				//���... x�� 0,1�� ���;���
				int count = diffX * -1;		//1
				//���� ���� �����ؾ� ��
				if (x <= count)
					oldSectors[y][x] = nullptr;

				//�ڿ� ���� �����ؾ� ��
				if (x >= count)
					updateSectors[y][x] = nullptr;
			}

			if (diffY > 0)
			{
				if (y >= diffY)
					oldSectors[y][x] = nullptr;

				if (y <= diffY)
					updateSectors[y][x] = nullptr;
			}
			else if (diffY < 0)
			{
				//�Ʒ����� ���� �̵����� ���̽�
				int count = diffY * -1;		//1

				//�� ���� �����ؾ� ��
				if (y <= count)
					oldSectors[y][x] = nullptr;

				//�Ʒ� ���� �����ؾ� ��
				if (y >= count)
					updateSectors[y][x] = nullptr;
			}
		}
	}

	//���� oldSectors���� ���� �޽����� ������ �Ǵ� ���͵��� ���Ұ�
	//updateSectors���� ���� �޽����� ������ �Ǵ� ���͵��� ����		
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			Sector* oldSector = oldSectors[y][x];
			if (oldSector == nullptr)
				continue;

			for (auto iter = oldSector->mPlayers.begin();
				iter != oldSector->mPlayers.end(); ++iter)
			{
				Player* otherPlayer = *iter;
				if (otherPlayer->GetID() == player->GetID())
					continue;

				old(otherPlayer);				
			}
		}
	}
	
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			Sector* updateSector = updateSectors[y][x];
			if (updateSector == nullptr)
				continue;

			for (auto iter = updateSector->mPlayers.begin();
				iter != updateSector->mPlayers.end(); ++iter)
			{
				Player* otherPlayer = *iter;
				if (otherPlayer->GetID() == player->GetID())
					continue;

				update(otherPlayer);				
			}
		}
	}	
}