#pragma once

#include "Core/Memory/AllocationStrategy.h"
#include "Core/Memory/FreelistAllocator/SinglyLinkedList.h"

namespace Crown
{
	class FreeList final : public AllocationStrategy
	{
	private:
		struct FreeHeader
		{
			std::size_t m_BlockSize;
			//bool m_Used{};//1 byte
			//char padding[7]{};//7 bytes to align m_Used, this struct can become much smaller than 40 bytes
			//std::size_t m_Size{};//8 bytes
			//char* m_Next{};//8 bytes
			//char* m_Previous{};//8 bytes
			//char* m_Data{};//8 bytes
		};
		struct AllocationHeader
		{
			std::size_t m_BlockSize;
			char m_Padding;
		};
		typedef SinglyLinkedList<FreeHeader>::Node Node;//Smaller to type
	
	public:
		explicit FreeList(char* const a_Buffer, std::size_t a_Max/*, MemoryType a_MemoryType = MemoryType::Cpu*/);
		
	private:
		void* DoAllocate(const std::size_t a_Size, const std::size_t a_Alignment = 8) override;
		void DoDeallocate(void* const a_Ptr, const std::size_t/* a_Count*/) override;
		void DoRelease() override;

		void Coalescence(Node* a_PreviousNode, Node* a_FreeNode);
		void Find(const std::size_t a_Size, const std::size_t a_Alignment, std::size_t& a_Padding, Node*& a_PreviousNode, Node*& a_FoundNode) const;
		void FindBest(const std::size_t a_Size, const std::size_t a_Alignment, std::size_t& a_Padding, Node*& a_PreviousNode, Node*& a_FoundNode) const;
		void FindFirst(const std::size_t a_Size, const std::size_t a_Alignment, std::size_t& a_Padding, Node*& a_PreviousNode, Node*& a_FoundNode) const;

		
		char* const m_Buffer{ nullptr };
		const std::size_t UNUSED(m_Max) { 0 };
		SinglyLinkedList<FreeHeader> m_FreeList{};
		//MemoryType m_MemoryType{ MemoryType::Cpu };
		
		//void* FirstFit(std::size_t a_Size) const;
		//void FirstFit(std::size_t a_Size, const std::size_t a_Alignment, std::size_t& padding, Header*& a_PreviousHeader, Header*& a_FoundHeader) const;
		//void* NextFit();
		//void* BestFit();
		//void* SegregatedFit();

		//static void HeaderSplit(Header*& a_Header, const std::size_t a_TotalSize, const std::size_t a_Remainder);
		//void headerCoalesce();
		//Header* m_Firstheader{};
		//Header* m_Lastheader{};
		//size_t m_SplittingSize{};
	};
}//Crown


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

	//FreeList freelist{g_GlobalHeap, 2 * g_Gb};
	////const auto ptr = freelist.allocate(0, 0);
	//const auto ptr = freelist.allocate(8);
	//freelist.deallocate(ptr, 8);
	//const auto ptrN = freelist.allocate(9);
	//const auto ptr1 = freelist.allocate(82);// , 8);

	//const auto ptr2 = freelist.allocate(13);
	//const auto ptr3 = freelist.allocate(25);
	//freelist.deallocate(ptr2, 13);
	//freelist.deallocate(ptr1, 82);
	//const auto ptr4 = freelist.allocate(8);
 //   freelist.deallocate(ptr3,0);
 //   freelist.deallocate(ptr4, 0);
 //   freelist.deallocate(ptrN, 0);

	////freelist.deallocate(ptr2, 16);


	//freelist.deallocate(reinterpret_cast<char*>(ptr1) - 1, 0);
	//freelist.deallocate(ptr1, 1);
	



	//FreeList freelist{g_GlobalHeap, 2 * g_Gb};
	////const auto ptr = freelist.allocate(0, 0);
	//const auto ptr = freelist.allocate(8);
	//freelist.deallocate(ptr, 8);
	//const auto ptrN = freelist.allocate(9);
	//const auto ptr1 = freelist.allocate(82);// , 8);

	//const auto ptr2 = freelist.allocate(13);
	//const auto ptr3 = freelist.allocate(25);
	//freelist.deallocate(ptr2, 24);
	//freelist.deallocate(ptr1, 80);
	//const auto ptr4 = freelist.allocate(8);
	//freelist.deallocate(ptr2, 16);