#include "./Network/NetIoEvent.h"

namespace garam
{
	namespace net
	{
		NetIoEvent::NetIoEvent(eIoEventType type)
			: mIoType(type)
		{
			Initialize();
		}

		void NetIoEvent::Initialize()
		{
			OVERLAPPED::hEvent = 0;
			OVERLAPPED::Internal = 0;
			OVERLAPPED::InternalHigh = 0;
			OVERLAPPED::Offset = 0;
			OVERLAPPED::OffsetHigh = 0;
			OVERLAPPED::Pointer = 0;
		}

		eIoEventType NetIoEvent::GetType()
		{
			return mIoType;
		}
	}
}
