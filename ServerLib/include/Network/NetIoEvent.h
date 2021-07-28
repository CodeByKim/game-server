#pragma once
#include "../Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		enum class eIoEventType
		{
			Receive,
			Send
		};

		class NetIoEvent : public OVERLAPPED
		{	
		public:
			NetIoEvent(eIoEventType type);
			
			void Initialize();
			eIoEventType GetType();
			
		private:
			eIoEventType mIoType;
		};
	}
}


