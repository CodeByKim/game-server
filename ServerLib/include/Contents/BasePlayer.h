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
			BasePlayer(bool isMoving, ClientInfo* client);
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