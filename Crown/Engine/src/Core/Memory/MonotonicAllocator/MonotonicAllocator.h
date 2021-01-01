#pragma once

#include "Core/Memory/AllocationStrategy.h"

#ifdef _DEBUG_MEM_TEXT
#include <iostream>
#endif
 
namespace Crown
{
	class Monotonic final : public AllocationStrategy {
	public:
		explicit Monotonic(char* const a_Buffer, std::size_t a_Max/*, MemoryType a_MemoryType = MemoryType::Cpu*/) :m_Buffer{ a_Buffer }, m_Max{a_Max} {}//, m_MemoryType(a_MemoryType), m_Max{ a_Max } {};
	private:
		void* DoAllocate(const std::size_t a_Count, const std::size_t/* a_Alignment*/) override
		{
			CROWN_PROFILE_FUNCTION();
			ASSERT_ENGINE(!(m_Head + a_Count > m_Max), "Monotonic buffer overflow");
			ASSERT_ENGINE(!(m_Head + a_Count < m_Head), "Monotonic allocate failed");
			const auto ptr{ m_Buffer + m_Head };
			///
#ifdef _DEBUG_MEM_TEXT
			std::printf("Monotonic Allocate %zu bytes at %p \n", a_Count, ptr);
#endif
			///
			m_Head += a_Count;
			return reinterpret_cast<void*>(ptr);
		}

#ifdef _DEBUG_MEM_TEXT 
		void DoDeallocate(void* const a_Ptr, const std::size_t a_Count) override
		{
			std::printf("Monotonic Deallocate %zu bytes at %p \n", a_Count, a_Ptr);
			// impl
		}
#else
		void DoDeallocate(void* const/* a_Ptr*/, const std::size_t/* a_Count*/) override
		{
			// impl
		}
#endif
 
		void DoRelease() override
		{
#ifdef _DEBUG_MEM_TEXT
			std::printf("Monotonic Release %zu bytes\n", m_Head);
#endif
			m_Head = 0;
		}
 
		char* const m_Buffer{ nullptr };
		//MemoryType m_MemoryType{ MemoryType::Cpu };
		const std::size_t UNUSED(m_Max) { 0 };
		std::size_t m_Head{ 0 };
	};
 
}//Crown