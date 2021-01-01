#ifdef __ORBIS__
#include <cstdio>
#include <kernel/memory.h>
#include <sys/_defines/_sce_ok.h>

#include "Core/Memory/MemoryCustom.h"
 
namespace Crown
{
	size_t InitializeMemory(std::size_t a_Size, std::size_t a_Alignment, std::size_t a_Start, std::size_t a_MemType/*SceKernelMemoryType*/)
	{
		off_t physicalAddress{};
 
		const auto ret = sceKernelAllocateDirectMemory
		(
			a_Start,
			SCE_KERNEL_MAIN_DMEM_SIZE,
			a_Size,
			a_Alignment,
			a_MemType,
			&physicalAddress
		);
		if (ret != SCE_OK)
		{
			printf("sceKernelAllocateDirectMemory failed: 0x%08X\n", ret);
			return 0;
		}
		return physicalAddress;
	}
 
	void* MaskMemory(std::size_t a_Size, std::size_t a_Protection, std::size_t a_Flags, std::size_t a_PhysicalAddress, std::size_t a_Alignment)
	{
		void* address = nullptr;
		const auto ret = sceKernelMapDirectMemory
		(
			&address,
			a_Size,
			a_Protection,
			a_Flags,
			a_PhysicalAddress,
			a_Alignment
		);
		if (ret != SCE_OK)
		{
			printf("sceKernelMapDirectMemory failed: 0x%08X\n", ret);
			return nullptr;
		}
		return address;
	}
 
	//Defines to switch memory allocation strategies
#ifdef NO_CUSTOM_ALLOCATOR
#undef CUSTOM_ALLOCATOR
#endif
#ifdef CUSTOM_ALLOCATOR
	size_t g_PhysicalCpu = InitializeMemory(2 * g_Gb, 16 * g_Kb, 0, SCE_KERNEL_WB_ONION);
	size_t g_PhysicalGpu = InitializeMemory(1 * g_Gb, 16 * g_Kb, 0, SCE_KERNEL_WC_GARLIC);

	MemoryBuffer::MemoryBuffer() :m_Buffer(reinterpret_cast<char*>(MaskMemory(1 * g_Gb, SCE_KERNEL_PROT_CPU_RW | SCE_KERNEL_PROT_GPU_RW, 0, g_PhysicalCpu + 1 * g_Gb, 16 * g_Kb))) {}
	MemoryBuffer g_MemoryBuffer;
	char& g_GlobalHeap = *g_MemoryBuffer.m_Buffer;
	
	char* g_CpuHeap = static_cast<char*>(MaskMemory(1 * g_Gb, SCE_KERNEL_PROT_CPU_RW, 0, g_PhysicalCpu, 16 * g_Kb));
	//char* g_GlobalHeap = static_cast<char*>(MaskMemory(1 * g_Gb, SCE_KERNEL_PROT_CPU_RW | SCE_KERNEL_PROT_GPU_RW, 0, g_PhysicalCpu + 1 * g_Gb, 16 * g_Kb));
	char* g_GpuHeap = static_cast<char*>(MaskMemory(1 * g_Gb, SCE_KERNEL_PROT_GPU_RW, 0, g_PhysicalGpu, 16 * g_Kb));
	
	
	
#ifdef STACK_ALLOCATOR
	Stack g_Allocator{ &g_GlobalHeap , 1 * g_Gb };
#elif defined MONOTONIC_ALLOCATOR
	Monotonic g_Allocator{ &g_GlobalHeap , 1 * g_Gb };
#elif defined FREELIST_ALLOCATOR
	FreeList g_Allocator{ &g_GlobalHeap , 1 * g_Gb };
#else
	static_assert(false, "No allocation strategy selected for CUSTOM_ALLOCATOR");
#endif
#endif
	
}//Crown
#endif//ORBIS
 