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
	// 이전 섹터인 Leave Sector에 관한 처리
	ProcessLeaveSector(leave);

	// 새로 진입한 Enter Sector에 관한 처리
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


			// otherPlayer에게 player가 삭제됬다고 전달해라						
			SEND_REMOVE_OTHER_PLAYER(*otherPlayer->GetClientInfo(), GetID());

			//player에게 otherPlayer가 삭제되었다고 전달해라						
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

			// otherPlayer에게 player가 새로 생성되었다고 전달해라						
			SEND_CREATE_OTHER_PLAYER(*otherPlayer->GetClientInfo(),
				GetID(),
				GetDirection(),
				GetPosition().x,
				GetPosition().y);

			// player에게 otherPlayer가 새로 생성되었다고 전달해라						
			SEND_CREATE_OTHER_PLAYER(*GetClientInfo(),
				otherPlayer->GetID(),
				otherPlayer->GetDirection(),
				otherPlayer->GetPosition().x,
				otherPlayer->GetPosition().y);

			if (IsMove())
			{
				/*
				 * 내가 지금 이동 중이라면
				 * 새로 진입한 섹터의 다른 플레이어에게
				 * 내가 이동중이라는 것을 알려라
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
				 * 새로 진입한 섹터의 다른 플레이어가 이동 중이라면
				 * 나한테 그 플레이어의 정보를 보내라
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