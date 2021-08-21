#include "./Contents/BasePlayer.h"
#include "Contents/World.h"
#include "Common/Protocol.h"

BasePlayer::BasePlayer(bool isMoving, garam::net::ClientInfo* client)
	: mIsMoving(isMoving)
	, mClient(client)
{
}

BasePlayer::~BasePlayer()
{
}

bool BasePlayer::IsMove()
{
	return mIsMoving;
}

garam::net::ClientInfo* BasePlayer::GetClientInfo()
{
	return mClient;
}

void BasePlayer::OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter)
{
	// ���� ������ Leave Sector�� ���� ó��
	ProcessLeaveSector(leave);

	// ���� ������ Enter Sector�� ���� ó��
	ProcessNewEnterSector(enter);
}

void BasePlayer::ProcessLeaveSector(std::vector<Sector*>& leaveSectors)
{
	for (int i = 0; i < leaveSectors.size(); i++)
	{
		Sector* leaveSector = leaveSectors[i];
		std::list<BasePlayer*>& players = leaveSector->players;

		for (auto iter = players.begin();
			iter != players.end();
			++iter)
		{
			BasePlayer* otherPlayer = *iter;

			if (otherPlayer->GetID() == GetID())
				continue;


			// otherPlayer���� player�� ������ٰ� �����ض�						
			SEND_REMOVE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), GetID());

			//player���� otherPlayer�� �����Ǿ��ٰ� �����ض�						
			SEND_REMOVE_OTHER_PLAYER(*GetClientInfo(), otherPlayer->GetID());

		}

		std::list<Monster*>& monsters = leaveSector->monsters;

		for (auto iter = monsters.begin();
			iter != monsters.end();
			++iter)
		{
			Monster* monster = *iter;

			SEND_REMOVE_MONSTER(*GetClientInfo(), monster->GetID());
		}
	}
}

void BasePlayer::ProcessNewEnterSector(std::vector<Sector*>& enterSectors)
{
	for (int i = 0; i < enterSectors.size(); i++)
	{
		Sector* enterSector = enterSectors[i];
		std::list<BasePlayer*>& players = enterSector->players;

		for (auto iter = players.begin();
			iter != players.end();
			++iter)
		{
			BasePlayer* otherPlayer = *iter;

			if (otherPlayer->GetID() == GetID())
				continue;

			// otherPlayer���� player�� ���� �����Ǿ��ٰ� �����ض�						
			SEND_CREATE_OTHER_PLAYER(*otherPlayer->GetClientInfo(),
				GetID(),
				GetDirection(),
				GetPosition().x,
				GetPosition().y);

			// player���� otherPlayer�� ���� �����Ǿ��ٰ� �����ض�						
			SEND_CREATE_OTHER_PLAYER(*GetClientInfo(),
				otherPlayer->GetID(),
				otherPlayer->GetDirection(),
				otherPlayer->GetPosition().x,
				otherPlayer->GetPosition().y);

			if (IsMove())
			{
				/*
				 * ���� ���� �̵� ���̶��
				 * ���� ������ ������ �ٸ� �÷��̾��
				 * ���� �̵����̶�� ���� �˷���
				 */
				SEND_PLAYER_MOVE_START(*otherPlayer->GetClientInfo(),
					GetID(),
					GetDirection(),
					GetPosition().x,
					GetPosition().y);
			}

			if (otherPlayer->IsMove())
			{
				/*
				 * ���� ������ ������ �ٸ� �÷��̾ �̵� ���̶��
				 * ������ �� �÷��̾��� ������ ������
				 */
				SEND_PLAYER_MOVE_START(*GetClientInfo(),
					otherPlayer->GetID(),
					otherPlayer->GetDirection(),
					otherPlayer->GetPosition().x,
					otherPlayer->GetPosition().y);
			}
		}

		std::list<Monster*>& monsters = enterSector->monsters;

		for (auto iter = monsters.begin();
			iter != monsters.end();
			++iter)
		{
			Monster* monster = *iter;

			SEND_CREATE_MONSTER(*GetClientInfo(),
				monster->GetID(),
				monster->GetDirection(),
				monster->GetPosition().x,
				monster->GetPosition().y);
		}
	}
}