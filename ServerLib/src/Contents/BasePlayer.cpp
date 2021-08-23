#include "./Contents/BasePlayer.h"
#include "Contents/World.h"
#include "Server/Connection.h"

namespace garam
{
	namespace net
	{
		BasePlayer::BasePlayer(bool isMoving, garam::net::ClientInfo* client)
			: mIsMoving(isMoving)
			, mClient(client)
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