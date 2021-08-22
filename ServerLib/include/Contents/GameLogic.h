#pragma once
#include "../Common/CommonLib.h"

//TODO : message handler에는 이제 update 필요 없겠지?

namespace garam
{
	namespace net
	{
		class World;

		class GameLogic
		{
		public:
			GameLogic();
			~GameLogic();

			void RegisterWorld(World* world);

			virtual void OnUpdate(float deltaTime);

		private:
			World* mWorld;
		};
	}
}