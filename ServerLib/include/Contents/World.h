#pragma once
#include "../Common/CommonLib.h"

#define MAP_SIZE_X 2000
#define MAP_SIZE_Y 2000

namespace garam
{
	namespace net
	{
		class BasePlayer;
		class Entity;
		class NetPacket;

		class Sector
		{
		public:
			Sector()
			{
				x = 0;
				y = 0;
				players.clear();
			}

			bool operator==(Sector& other)
			{
				return x == other.x && y == other.y;
			}

			bool operator!=(Sector& other)
			{
				return !(*this == other);
			}

			//TODO : 나중에 하나로 합친다.
			std::list<BasePlayer*> players;
			std::list<Entity*> monsters;

			int x;
			int y;
		};

		class World
		{
		public:
			World();
			~World();

			void Create(int sectorCountX, int sectorCountY, int sectorSize);
			void AddPlayer(BasePlayer* player);
			void RemovePlayer(BasePlayer* player);
			void Broadcast(garam::net::NetPacket* packet, BasePlayer* sender, bool exceptSender = true);
			Sector* GetSector(Entity* entity);
			void GetAroundSector(Entity* entity, std::vector<Sector*>* outAroundSectors);
			virtual void OnUpdate(float deltaTime);

		protected:
			//TODO : Entity로 하나로 합쳐야 한다.
			void GetPlayerInfoContainedInSector(BasePlayer* player, std::vector<BasePlayer*>& otherPlayers);
			void GetMonsterInfoContainedInSector(BasePlayer* player, std::vector<Entity*>& otherMonsters);
			void ChangeSector(BasePlayer* player, float x, float y);

			Sector** mSectors;
			std::vector<BasePlayer*> mPlayers;
			int mSectorSize;
			int mSectorCountX;
			int mSectorCountY;

		private:			
		};
	}	
}
