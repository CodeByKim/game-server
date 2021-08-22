#include "./Contents/GameLogic.h"
#include "./Contents/World.h"

namespace garam
{
	namespace net
	{
		GameLogic::GameLogic()
			: mWorld(nullptr)
		{
		}

		GameLogic::~GameLogic()
		{
		}

		void GameLogic::RegisterWorld(World* world)
		{
			mWorld = world;
		}

		void GameLogic::OnUpdate(float deltaTime)
		{
			if (mWorld != nullptr)
			{
				mWorld->OnUpdate(deltaTime);
			}
		}
	}
}
