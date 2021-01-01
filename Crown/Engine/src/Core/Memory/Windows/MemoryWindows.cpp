#include "CrownPCH.h"
#pragma warning(push, 0)//#pragma init_seg(compiler) is normally reserved for compiler specific or std code, this warning gets treated as an error.
#pragma init_seg (compiler)//forcing early static initialize (malloc) of memory buffers before initialization of std::cout (This all happens before int main())
#pragma warning(pop)//See 

#include "Core/Memory/MemoryCustom.h"

namespace Crown
{
	size_t InitializeMemory(size_t a_Size, size_t/* a_Alignment*/, size_t/* a_Start*/, size_t/* a_MemType*/)
	{
		return reinterpret_cast<size_t>(std::malloc(a_Size));
	}
	
	void* MaskMemory(size_t a_Size, size_t/* a_Protection*/, size_t/* a_Flags*/, size_t/* a_PhysicalAddress*/, size_t/* a_Alignment*/)
	{
		return std::malloc(a_Size);
	}

	
	//char* g_CpuHeap = static_cast<char*>(MaskMemory(1 * g_Gb, 0x02 /*SCE_KERNEL_PROT_CPU_RW*/, 0, g_PhysicalCpu, 16 * g_Kb));
	//char* g_GlobalHeap = static_cast<char*>(MaskMemory(1 * g_Gb, 0x02 | 0x30 /*SCE_KERNEL_PROT_CPU_RW | SCE_KERNEL_PROT_GPU_RW*/, 0, g_PhysicalCpu + 1 * g_Gb, 16 * g_Kb));
	//char* g_GpuHeap = static_cast<char*>(MaskMemory(1 * g_Gb, 0x30 /*SCE_KERNEL_PROT_GPU_RW*/, 0, g_PhysicalGpu, 16 * g_Kb));

	//Defines to switch memory allocation strategies
#ifdef NO_CUSTOM_ALLOCATOR
#undef CUSTOM_ALLOCATOR
#endif
#ifdef CUSTOM_ALLOCATOR
	MemoryBuffer::MemoryBuffer() :m_Buffer(reinterpret_cast<char*>(MaskMemory(g_MemorySizeWindows, 0, 0, 0, 0))){}
	MemoryBuffer g_MemoryBuffer;
	char& g_GlobalHeap = *g_MemoryBuffer.m_Buffer;
	//char* g_GlobalHeap = reinterpret_cast<char*>(MaskMemory(g_MemorySizeWindows, 0, 0, 0, 0));
#ifdef STACK_ALLOCATOR
	Stack g_Allocator{ &g_GlobalHeap , g_MemorySizeWindows };
#elif defined MONOTONIC_ALLOCATOR
	Monotonic g_Allocator{ &g_GlobalHeap , g_MemorySizeWindows };
#elif defined FREELIST_ALLOCATOR
	FreeList g_Allocator{ &g_GlobalHeap , g_MemorySizeWindows };
#else
	static_assert(false, "No allocation strategy selected for CUSTOM_ALLOCATOR");
#endif
#endif
	
}//Crown
 