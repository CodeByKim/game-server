#include "Common/NetPacketAllocator.h"

namespace garam
{
	namespace net
	{
		NetPacket* NetPacketAllocator::Alloc()
		{
			return mPool.Alloc();
		}

		void NetPacketAllocator::Free(NetPacket* packet)
		{
			if (InterlockedDecrement((UINT*)&packet->mRefCount) == 0)
			{
				mPool.Free(packet);
			}
		}

		NetPacketAllocator::NetPacketAllocator()
		{
		}
	}
}