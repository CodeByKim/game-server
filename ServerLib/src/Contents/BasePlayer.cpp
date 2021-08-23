#include "./Contents/BasePlayer.h"
#include "Contents/World.h"
#include "Server/Connection.h"

namespace garam
{
	namespace net
	{
		BasePlayer::BasePlayer(int hp, int id, BYTE dir, Position2D position, ClientInfo* client, bool isMoving)
			: Entity(hp, id, dir, position)
			, mClient(client)
			, mIsMoving(isMoving)			
		{
		}

		BasePlayer::~BasePlayer()
		{
		}

		bool BasePlayer::IsMove()
		{
			return mIsMoving;
		}

		garam::net::ClientInfo* BasePlayer::GetClientInfo()
		{
			return mClient;
		}
	}
}