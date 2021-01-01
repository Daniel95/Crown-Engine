// ReSharper disable CppInconsistentNaming
//namespace Crown won't work with global new overload
//Changing argument names doesn't match with global new overload
#include "CrownPCH.h"
#include "Core/Memory/MemoryCustom.h"
//#ifdef Windows
#ifndef NO_CUSTOM_ALLOCATOR
#include <new>
#ifdef _DEBUG_MEM_TEXT
#include <iostream>
#endif
//#include "Core/Memory/MemoryCustom.h"

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
		void* __CRTDECL operator new(std::size_t _Size)
	{
#ifdef _DEBUG_MEM_TEXT
		std::printf("global operator new(size_t) called, size = %zu\n", _Size);
#endif
		return Crown::g_Allocator.allocate(_Size, sizeof(_Size));
	}

	_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR
		void* __CRTDECL operator new[](std::size_t _Size)
	{
#ifdef _DEBUG_MEM_TEXT
		std::printf("global operator new[](size_t) called, size = %zu\n", _Size);
#endif
		return Crown::g_Allocator.allocate(_Size, sizeof(_Size));
	}

		void operator delete(void* _Block) noexcept
	{
#ifdef _DEBUG_MEM_TEXT
		std::printf("global operator delete(void*) called");/*, size = %zu\n", _Size);*/
#endif
		Crown::g_Allocator.deallocate(_Block, 0/*, _Size*/);
	}

	void operator delete[](void* _Block) noexcept
	{
#ifdef _DEBUG_MEM_TEXT
		std::printf("global operator delete[](void*) called");/*, size = %zu\n", _Size);*/
#endif
		Crown::g_Allocator.deallocate(_Block, 0/*, _Size*/);
	}

		void operator delete(void* _Block, std::size_t _Size) noexcept
	{
#ifdef _DEBUG_MEM_TEXT
		std::printf("global operator delete(void*,size_t) called, size = %zu\n", _Size);
#endif
		Crown::g_Allocator.deallocate(_Block, _Size);
	}

	void operator delete[](void* _Block, std::size_t _Size) noexcept
	{
#ifdef _DEBUG_MEM_TEXT
		std::printf("global operator delete[](void*,size_t) called, size = %zu\n", _Size);
#endif
		Crown::g_Allocator.deallocate(_Block, _Size);
	}
#endif //NO_CUSTOM_ALLOCATOR
//#endif //Windows
