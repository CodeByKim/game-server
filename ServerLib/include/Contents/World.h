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
			
			std::list<BasePlayer*> players;
			//std::list<Entity*> monsters;
			std::list<Entity*> entities;

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
			void GetEntityAroundSector(BasePlayer* player, std::vector<Entity*>& otherEntities);			
			
			virtual void OnPlayerJoin(BasePlayer* player, std::vector<Entity*>& otherEntities) = 0;
			virtual void OnPlayerLeave(BasePlayer* leavePlayer) = 0;
			virtual void ProcessLeaveSector(BasePlayer* player, std::vector<Sector*>& leaveSectors) = 0;
			virtual void ProcessNewEnterSector(BasePlayer* player, std::vector<Sector*>& enterSectors) = 0;

			Sector** mSectors;
			std::vector<BasePlayer*> mPlayers;
			int mSectorSize;
			int mSectorCountX;
			int mSectorCountY;

		private:
			void SectorUpdate(BasePlayer* player, std::vector<Sector*>& leave, std::vector<Sector*>& enter);
		};
	}	
}
