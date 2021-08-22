#include "./Contents/BasePlayer.h"
#include "Contents/World.h"
#include "Server/Connection.h"

namespace garam
{
	namespace net
	{
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

		/*World* BasePlayer::GetWorld()
		{
			return mClient->GetWorld();
		}*/

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


					OnOtherPlayerLeaveSectorRange(otherPlayer);
				}

				std::list<Entity*>& monsters = leaveSector->monsters;

				for (auto iter = monsters.begin();
					iter != monsters.end();
					++iter)
				{
					Entity* monster = *iter;

					OnOtherMonsterLeaveSectorRange(monster);
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

					OnOtherPlayerEnterSectorRange(otherPlayer);
				}

				std::list<Entity*>& monsters = enterSector->monsters;

				for (auto iter = monsters.begin();
					iter != monsters.end();
					++iter)
				{
					Entity* monster = *iter;

					OnOtherMonsterEnterSectorRange(monster);
				}
			}
		}
	}
}