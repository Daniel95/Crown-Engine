#pragma once

//#include <cstdio>//used for size_t (same name for PS4 and Windows)
//#define _DEBUG_MEM_TEXT//Enables debug text for custom memory allocators, a lot of text during runtime.

#include "Core/Utilities/Defines.h"

namespace Crown
{
	class AllocationStrategy
	{
	public:
		virtual ~AllocationStrategy() = default;
		AllocationStrategy() = default;

		void* allocate(const std::size_t a_Size, std::size_t a_Alignment = 8)//lower case required for template code in std:: container classes
		{
			CROWN_PROFILE_FUNCTION();

			return this->DoAllocate(a_Size, a_Alignment);
		}

		void deallocate(void* const a_Ptr, const std::size_t a_Size)//lower case required for template code in std:: container classes
		{
			CROWN_PROFILE_FUNCTION();

			this->DoDeallocate(a_Ptr, a_Size);
		}

		void release()//lower case required for template code in std:: container classes
		{
			CROWN_PROFILE_FUNCTION();

			this->DoRelease();
		}

		virtual void* DoAllocate(const std::size_t a_Size, const std::size_t a_Alignment = 8) = 0;
		virtual void DoDeallocate(void* const a_Ptr, const std::size_t a_Size) = 0;
		virtual void DoRelease() = 0;

	private:
		//
	};

	enum class MemoryType
	{
		Cpu,
		Gpu
	};

}//Crown
