#pragma once
#include "../Common/CommonLib.h"
#include "./Contents/Entity.h"

namespace garam
{
	namespace net
	{
		class Sector;		
		class Entity;
		//class World;
		class ClientInfo;

		class BasePlayer : public Entity
		{
		public:
			BasePlayer(bool isMoving, ClientInfo* client);
			~BasePlayer();

			bool IsMove();
			ClientInfo* GetClientInfo();
			void OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter);
			/*World* GetWorld();*/
			virtual void OnAppendToWorld(std::vector<BasePlayer*>& otherPlayers, std::vector<Entity*>& otherMonsters) = 0;

		protected:
			virtual void OnOtherPlayerLeaveSectorRange(BasePlayer* otherPlayer) = 0;
			virtual void OnOtherPlayerEnterSectorRange(BasePlayer* otherPlayer) = 0;
			virtual void OnOtherMonsterLeaveSectorRange(Entity* otherMonster) = 0;
			virtual void OnOtherMonsterEnterSectorRange(Entity* otherMonster) = 0;

			ClientInfo* mClient;
			bool mIsMoving;

		private:
			void ProcessLeaveSector(std::vector<Sector*>& leaveSectors);
			void ProcessNewEnterSector(std::vector<Sector*>& enterSectors);
		};
	}
}