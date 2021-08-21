#pragma once
#include "../Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		class Entity
		{
		public:
			Entity();
			~Entity();

			int GetID();
			int GetHP();
			bool IsDead();
			BYTE GetDirection();
			Position2DInt& GetSectorPosition();
			Position2D& GetPosition();
			void SetSectorPosition(int x, int y);
			void SetPosition(float x, float y);

			virtual void OnUpdate(float deltaTime) = 0;
			virtual void OnHit(int damage) = 0;

		protected:
			int mHP;
			int mID;
			BYTE mCurrentDir;
			Position2D mPosition;
			Position2DInt mSectorPosition;
		};
	}
}