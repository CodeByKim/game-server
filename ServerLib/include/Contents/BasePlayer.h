#pragma once
#include "../Common/CommonLib.h"
#include "./Contents/Entity.h"

namespace garam
{
	namespace net
	{
		class Sector;		
		class Entity;		
		class ClientInfo;

		class BasePlayer : public Entity
		{
		public:
			BasePlayer(int hp, int id, BYTE dir, Position2D position, ClientInfo* client, bool isMoving);
			~BasePlayer();

			bool IsMove();
			ClientInfo* GetClientInfo();			

		protected:
			ClientInfo* mClient;
			bool mIsMoving;

		private:
		};
	}
}