#pragma once
#include "../Common/CommonLib.h"
#include "IAllacator.h"
#include "MemoryPool.h"
#include "NetPacket.h"

namespace garam
{
	namespace net
	{		
		class NetPacket;

		class NetPacketAllocator : public memory::IAllocator<NetPacket*>
		{
		public:
			NetPacketAllocator();
			NetPacket* Alloc();
			void Free(NetPacket* packet);

		private:			
			memory::MemoryPool<NetPacket> mPool;
		};
	}
}