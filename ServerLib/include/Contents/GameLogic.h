#pragma once
#include "../Common/CommonLib.h"

//TODO : message handler���� ���� update �ʿ� ������?

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