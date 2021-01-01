#pragma once

#ifdef _DEBUG_MEM_TEXT
#include <iostream>
#endif

#include "Core/Memory/AllocationStrategy.h"

/*//////Example stack allocator implementation allocate /deallocate
//#include "Core/Memory/StackAllocator.h"
//Stack stack{ g_GlobalHeap };
//const auto ptr1 = stack.allocate(8, 0);
//const auto ptr2 = stack.allocate(17, 0);
//const auto ptr3 = stack.allocate(12, 0);
//stack.deallocate(ptr2, 17);
//stack.deallocate(ptr3, 12);
//stack.deallocate(ptr1, 8);
////stack.deallocate(reinterpret_cast<char*>(ptr1) - 1, 0); //assert outside of buffer
////stack.deallocate(ptr1, 1);//assert deallocate wrong size (outside of buffer)
*/

/*////Example stack allocator implementation allocate /release /deallocate
//#include "Core/Memory/StackAllocator.h"
//Stack stack{ g_GlobalHeap };
//const auto ptr1 = stack.allocate(8, 0);
//auto ptr2 = stack.allocate(17, 0);
//const auto ptr3 = stack.allocate(12, 0);
//stack.release();
//stack.deallocate(ptr2, 17);
//ptr2 = stack.allocate( 7,0);
//stack.deallocate(ptr3, 12);
//stack.deallocate(ptr1, 8);
////stack.deallocate(reinterpret_cast<char*>(ptr1) - 1, 0); //assert outside of buffer
////stack.deallocate(ptr1, 1);//assert deallocate wrong size (outside of buffer)
*/

namespace Crown
{
	class Stack final : public AllocationStrategy
	{
	public:
		explicit Stack(char* const a_Buffer) : m_Buffer{ a_Buffer } {}
	private:
		void* DoAllocate(const std::size_t a_Count, const std::size_t/*a_Alignment*/) override
		{
			ASSERT_ENGINE(!(m_Buffer == nullptr), "stack buffer not initialized");
			ASSERT_ENGINE(!(a_Count < 0), "Allocating less than 0 in stack buffer");
			
			m_Free = m_Buffer + m_Head;
#ifdef _DEBUG_MEM_TEXT
			std::printf("Stack Allocate %zu bytes at %p \n", a_Count, m_Free);
#endif
			m_Head += a_Count;
			return static_cast<void*>(m_Free);
		}

		void DoDeallocate(void* const a_Ptr, const std::size_t a_Size) override
		{
			//no possibility to check previous allocation without using e.g. a linked list to store this information
			ASSERT_ENGINE(!(m_Buffer == nullptr), "stack buffer not initialized");

			ASSERT_ENGINE(!(a_Ptr == nullptr), "Deallocating nullptr in stack buffer");
			ASSERT_ENGINE(!(a_Size < 0), "Deallocating less than 0 in stack buffer");

			ASSERT_ENGINE(!(a_Ptr < m_Buffer), "Deallocating outside stack buffer");

			if (a_Ptr <= m_Free)
			{
				m_Free = static_cast<char*>(a_Ptr);
				m_Head = m_Free - m_Buffer;
#ifdef _DEBUG_MEM_TEXT
				std::printf("Stack Deallocate %zu bytes at %p \n", a_Size, a_Ptr);
			}
			else
			{
				std::printf("Warning: Stack Deallocate call on already de-allocated memory \n");
				std::printf("%zu bytes at %p \n", a_Size, a_Ptr);
				std::printf("%zu bytes off current pointer at %p \n", reinterpret_cast<char*>(a_Ptr) - m_Free, m_Free);
			}
#else
			}
#endif
		}

		void DoRelease() override
		{
#ifdef _DEBUG_MEM_TEXT
			std::printf("Stack Release %zu bytes from %p to %p \n", m_Head, m_Free, m_Buffer);
#endif
			m_Free = m_Buffer;
			m_Head = 0;
		}

		char* const m_Buffer{ nullptr };
		char* m_Free{ nullptr };
		std::size_t m_Head{ 0 };
	};
	
}//Crown
