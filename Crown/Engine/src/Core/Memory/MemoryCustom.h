#pragma once
#include <cstdlib>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>//used for size_t (same name for PS4 and Windows)



//Defines to switch memory allocation strategies
#define CUSTOM_ALLOCATOR
#define NO_CUSTOM_ALLOCATOR
	#define FREELIST_ALLOCATOR
	//#define STACK_ALLOCATOR
	//#define MONOTONIC_ALLOCATOR

#ifdef NO_CUSTOM_ALLOCATOR
#undef CUSTOM_ALLOCATOR
#endif

#ifdef CUSTOM_ALLOCATOR
	#ifdef STACK_ALLOCATOR
	#include "Core/Memory/StackAllocator/StackAllocator.h"
	#elif defined MONOTONIC_ALLOCATOR
	#include "Core/Memory/MonotonicAllocator/MonotonicAllocator.h"
	#elif defined FREELIST_ALLOCATOR
	#include "Core/Memory/FreelistAllocator/FreelistAllocator.h"
	#else
	static_assert(false, "No allocation strategy selected for CUSTOM_ALLOCATOR");
	#endif
#endif

namespace Crown
{
	//Platform specific implementation in MemoryWindows.cpp and MemoryPS4.cpp
	size_t InitializeMemory(size_t a_Size, size_t a_Alignment, size_t a_Start = 0, size_t a_MemType = 0/*SceKernelMemoryType*/);
	void* MaskMemory(size_t a_Size, size_t a_Protection, size_t a_Flags, size_t a_PhysicalAddress, size_t a_Alignment);

	extern size_t g_PhysicalCpu;
	extern size_t g_PhysicalGpu;
	extern char* g_CpuHeap;
	extern char& g_GlobalHeap;
	extern char* g_GpuHeap;

	//Easy size usage
	const size_t g_Kb = 1024;
	const size_t g_Mb = 1024 * 1024;
	const size_t g_Gb = 1024 * 1024 * 1024;
	
	//Memory sizes for all custom memory strategies
	const size_t g_MemorySizeWindows = 2 * g_Gb;
	//const size_t g_MemorySizePS4 = 1 * g_Gb; //Hardcoded at 1 * g_Gb for now

	struct MemoryBuffer
	{
		MemoryBuffer();
		~MemoryBuffer()
		{
			std::free(m_Buffer);
		};
		char* m_Buffer;
	};



	
#ifdef CUSTOM_ALLOCATOR
#ifdef STACK_ALLOCATOR
	extern Stack g_Allocator;
#elif defined MONOTONIC_ALLOCATOR
	extern Monotonic g_Allocator;
#elif defined FREELIST_ALLOCATOR
	extern FreeList g_Allocator;
#else
	static_assert(false, "No allocation strategy selected for CUSTOM_ALLOCATOR");
#endif
#endif
}//Crown
