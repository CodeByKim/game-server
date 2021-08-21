#include "./Contents/Entity.h"

namespace garam
{
	namespace net
	{
		Entity::Entity()
		{
		}

		Entity::~Entity()
		{
		}

		int Entity::GetID()
		{
			return mID;
		}

		int Entity::GetHP()
		{
			return mHP;
		}

		bool Entity::IsDead()
		{
			return mHP <= 0;
		}

		BYTE Entity::GetDirection()
		{
			return mCurrentDir;
		}

		Position2DInt& Entity::GetSectorPosition()
		{
			return mSectorPosition;
		}

		Position2D& Entity::GetPosition()
		{
			return mPosition;
		}

		void Entity::SetSectorPosition(int x, int y)
		{
			mSectorPosition.x = x;
			mSectorPosition.y = y;
		}

		void Entity::SetPosition(float x, float y)
		{
			mPosition.x = x;
			mPosition.y = y;
		}
	}
}