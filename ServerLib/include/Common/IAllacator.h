#pragma once

namespace garam
{
	namespace net
	{
		template <typename T>
		class IAllocator
		{
		public:
			virtual T Alloc() = 0;
			virtual void Free(T obj) = 0;
		};
	}
}
