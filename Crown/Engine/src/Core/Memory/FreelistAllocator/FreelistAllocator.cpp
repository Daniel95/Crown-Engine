#include "CrownPCH.h"

#include "Core/Memory/FreelistAllocator/FreelistAllocator.h"
#include "Core/Memory/Utilities/AllocatorUtilities.h"

namespace Crown
{
	FreeList::FreeList(char* const a_Buffer, std::size_t a_Max): m_Buffer(a_Buffer), m_Max(a_Max)
	{
		CROWN_PROFILE_FUNCTION();
		DoRelease();
	}

	void* FreeList::DoAllocate(const std::size_t a_Size, const std::size_t a_Alignment)
	{
		CROWN_PROFILE_FUNCTION();

		const auto allocationHeaderSize = sizeof(AllocationHeader);
		//const auto freeHeaderSize = sizeof(FreeHeader);
		//ASSERT_ENGINE(a_Size >= sizeof(Node), "Allocation size is too small for Freelist");
		ASSERT_ENGINE(a_Alignment >= 8, "Alignment for Freelist must be at least 8 bytes");
		ASSERT_ENGINE(a_Size > 0, "Allocation to small in Freelist allocator");

		// Search through the free list for a free block that has enough space to allocate our data
		std::size_t padding;
		Node* affectedNode;
		Node* previousNode;
		this->Find(a_Size, a_Alignment, padding, previousNode, affectedNode);
		ASSERT_ENGINE(affectedNode != nullptr, "Freelist buffer overflow");
		ASSERT_ENGINE(affectedNode != NULL, "Freelist buffer overflow");

		const auto alignmentPadding = padding - allocationHeaderSize;
		const auto requiredSize = a_Size + padding;

		const auto rest = affectedNode->m_Data.m_BlockSize - requiredSize;

		if (rest > allocationHeaderSize)
		{
			// We have to split the block into the data block and a free block of size 'rest'
			const auto newFreeNode = reinterpret_cast<Node*>(reinterpret_cast<size_t>(affectedNode) + requiredSize);
			newFreeNode->m_Data.m_BlockSize = rest;
			m_FreeList.Insert(affectedNode, newFreeNode);
		}
		m_FreeList.Remove(previousNode, affectedNode);

		// Setup data block
		const auto headerAddress = reinterpret_cast<std::size_t>(affectedNode) + alignmentPadding;
		const auto dataAddress = headerAddress + allocationHeaderSize;
		
		reinterpret_cast<AllocationHeader*>(headerAddress)->m_BlockSize = requiredSize;//TODO: -alignmentPadding;
		reinterpret_cast<AllocationHeader*>(headerAddress)->m_Padding = static_cast<char>(alignmentPadding);

		return reinterpret_cast<void*>(dataAddress);
	}

	void FreeList::Find(const std::size_t a_Size, const std::size_t a_Alignment, std::size_t& a_Padding, Node*& a_PreviousNode, Node*& a_FoundNode) const
	{
		CROWN_PROFILE_FUNCTION();
		//Currently only the Find First algorithm works correctly
		FindFirst(a_Size, a_Alignment, a_Padding, a_PreviousNode, a_FoundNode);
		//FindBest(a_Size, a_Alignment, a_Padding, a_PreviousNode, a_FoundNode); //TODO: fix bug in... padding?
	}

	void FreeList::FindFirst(const std::size_t a_Size, const std::size_t a_Alignment, std::size_t& a_Padding, Node*& a_PreviousNode, Node*& a_FoundNode) const
	{
		CROWN_PROFILE_FUNCTION();
		//Iterate list and return the first free block with a size >= than given size
		auto it = m_FreeList.m_Head;
		Node* itPrev = nullptr;

		while (it != nullptr)
		{
			//a_Padding = CalculatePaddingWithHeader(reinterpret_cast<std::size_t>(it) + a_Size, a_Alignment, sizeof(AllocationHeader));
			a_Padding = CalculatePaddingWithHeader(reinterpret_cast<std::size_t>(it) + a_Size, a_Alignment, sizeof(AllocationHeader));
			const auto requiredSpace = a_Size + a_Padding;
			if (it->m_Data.m_BlockSize >= requiredSpace)
			{
				//Found fitting block
				break;
			}
			itPrev = it;
			it = it->m_Next;
		}
		a_PreviousNode = itPrev;
		a_FoundNode = it;
	}

	void FreeList::FindBest(const std::size_t a_Size, const std::size_t a_Alignment, std::size_t& a_Padding, Node*& a_PreviousNode, Node*& a_FoundNode) const
	{
		CROWN_PROFILE_FUNCTION();
		// Iterate WHOLE list keeping a pointer to the best fit
		auto smallestDifference = std::numeric_limits<std::size_t>::max();
		Node* bestBlock = nullptr;
		auto it = m_FreeList.m_Head;
		Node* itPrev = nullptr;
		Node* bestPrev = nullptr;
		
		while (it != nullptr)
		{
			a_Padding = CalculatePaddingWithHeader(reinterpret_cast<std::size_t>(it) + a_Size, a_Alignment, sizeof(AllocationHeader));
			const auto requiredSpace = a_Size + a_Padding;
			if (it->m_Data.m_BlockSize >= requiredSpace)
			{
				//Found fitting block
				if (it->m_Data.m_BlockSize - requiredSpace < smallestDifference)
				{
					//Found better fit
					bestPrev = itPrev;
					bestBlock = it;
					smallestDifference = it->m_Data.m_BlockSize - requiredSpace;
				}
			}
			itPrev = it;
			it = it->m_Next;
		}
		a_PreviousNode = bestPrev;
		a_FoundNode = bestBlock;
	}

	void FreeList::DoDeallocate(void* const a_Ptr, const std::size_t)
	{
		CROWN_PROFILE_FUNCTION();
		// Insert it in a sorted position by the address number
		const auto currentAddress = reinterpret_cast<std::size_t>(a_Ptr);
		const auto headerAddress = currentAddress - sizeof(AllocationHeader);
		const AllocationHeader* allocationHeader{ reinterpret_cast<AllocationHeader*>(headerAddress) };

		const auto freeNode = reinterpret_cast<Node*>(headerAddress);
		freeNode->m_Data.m_BlockSize = allocationHeader->m_BlockSize;// +allocationHeader->m_Padding;
		freeNode->m_Next = nullptr;

		auto it = m_FreeList.m_Head;
		Node* itPrev = nullptr;
		while (it != nullptr)
		{
			if (reinterpret_cast<Node*>(a_Ptr) < it)
			{
				m_FreeList.Insert(itPrev, freeNode);
				break;
			}
			itPrev = it;
			it = it->m_Next;
		}

		// Merge contiguous nodes
		Coalescence(itPrev, freeNode);
	}

	void FreeList::Coalescence(Node* a_PreviousNode, Node* a_FreeNode)
	{
		CROWN_PROFILE_FUNCTION();
		if (a_FreeNode->m_Next != nullptr)
		{
			if (reinterpret_cast<std::size_t>(a_FreeNode) + a_FreeNode->m_Data.m_BlockSize == reinterpret_cast<std::size_t>(a_FreeNode->m_Next))
			{
				a_FreeNode->m_Data.m_BlockSize += a_FreeNode->m_Next->m_Data.m_BlockSize;
				m_FreeList.Remove(a_FreeNode, a_FreeNode->m_Next);
			}
		}

		if (a_PreviousNode != nullptr)
		{
			if (reinterpret_cast<std::size_t>(a_PreviousNode) + a_PreviousNode->m_Data.m_BlockSize == reinterpret_cast<std::size_t>(a_FreeNode))
			{
				a_PreviousNode->m_Data.m_BlockSize += a_FreeNode->m_Data.m_BlockSize;
				m_FreeList.Remove(a_PreviousNode, a_FreeNode);
			}
		}
	}

	void FreeList::DoRelease()
	{
		CROWN_PROFILE_FUNCTION();
		const auto firstNode = reinterpret_cast<Node*>(m_Buffer);
		firstNode->m_Data.m_BlockSize = m_Max;
		firstNode->m_Next = nullptr;
		m_FreeList.m_Head = nullptr;
		m_FreeList.Insert(nullptr, firstNode);
	}

}











//namespace Crown
//{
//
//
//	FreeList::FreeList(char* const a_Buffer, std::size_t a_Max) : m_Buffer{ a_Buffer }, m_Max{ a_Max }, m_SplittingSize(sizeof(Header) * 2)//, m_MemoryType(a_MemoryType), m_Max{ a_Max } {};
//	{
//		m_Firstheader = reinterpret_cast<Header*>(a_Buffer);
//		m_Firstheader->m_Used = false;
//		m_Firstheader->padding[0] = char(171);//memory debug help 0xABABABABABABAB01 / 0xABABABABABABAB00 for firstHeader.m_Used
//		m_Firstheader->padding[1] = char(171);
//		m_Firstheader->padding[2] = char(171);
//		m_Firstheader->padding[3] = char(171);
//		m_Firstheader->padding[4] = char(171);
//		m_Firstheader->padding[5] = char(171);
//		m_Firstheader->padding[6] = char(171);
//		m_Firstheader->m_Size = a_Max - sizeof(Header);
//		m_Firstheader->m_Next = nullptr;
//		m_Firstheader->m_Previous = nullptr;
//		m_Firstheader->m_Data = m_Buffer + sizeof(Header);
//	}
//
//	void* FreeList::DoAllocate(const std::size_t a_Count, const std::size_t)
//	{
//		CROWN_PROFILE_FUNCTION();
//		ASSERT_ENGINE(a_Count != 0, "Allocating 0 bytes in Freelist allocator");
//		
//		const std::size_t allocationHeaderSize = sizeof(Header);
//		//const std::size_t freeHeaderSize = sizeof(Header);
//
//		/*
//		 * Find empty memory that is big enough for header + data + padding
//		 * ... empty memory has header, so find empty memory (header) big enough for data + padding
//		 * <------------------------------------------------>
//		 * First 40 bytes are already header
//		 * <****|------------------------------------------->
//		 * Next bytes are data
//		 * <****|###########|------------------------------->
//		 * Last bytes are padding
//		 * <****|###########|000---------------------------->
//		 * Check to split memory
//		 * <****|###########|000><-------------------------->
//		 * Enough free space left so create new header
//		 * First 40 bytes are header
//		 * <****|###########|000><****|---------------------->
//		 * Check for empty memory
//		 * <****|###########|000><****|---------------------->
//		 * First 40 bytes are already header
//		 * <****|###########|000><****|---------------------->
//		 * Next bytes are data
//		 * <****|###########|000><****|###########|---------->
//		 * Last bytes are padding
//		 * <****|###########|000><****|###########|000------->
//		 * Check to split memory
//		 * <****|###########|000><****|###########|000><----->
//		 * Not enough free space left
//		 */
//
//		std::size_t padding;
//		Header* previousHeader;
//		Header* foundHeader;
//		FirstFit(a_Count, alignof(std::size_t), padding, previousHeader, foundHeader);
//		ASSERT_ENGINE(foundHeader != nullptr, "Freelist buffer out of memory");
//
//
//
//
//
//		
//
//		//const std::size_t alignmentPadding = padding - allocationHeaderSize;
//		const std::size_t requiredSize = a_Count + padding;
//
//		const std::size_t remainder = foundHeader->m_Size - requiredSize - allocationHeaderSize;
//		foundHeader->m_Used = true;
//		if (remainder > 0)
//		{
//			//check to split memory block
//			HeaderSplit(foundHeader, requiredSize, remainder);
//			//Header* newFreeHeader = reinterpret_cast<Header*>(reinterpret_cast<char*>(foundHeader) + requiredSize);
//			//newFreeHeader->m_Size = remainder;
//		}
//		const std::size_t headerAddress = reinterpret_cast<std::size_t>(foundHeader);// +alignmentPadding;
//		const std::size_t dataAddress = headerAddress + allocationHeaderSize;
//		//reinterpret_cast<Header*>(headerAddress)->m_Size = requiredSize;
//		foundHeader->m_Data = reinterpret_cast<char*>(dataAddress);
//		//foundHeader->m_Data = reinterpret_cast<char*>(foundHeader) + allocationHeaderSize + alignmentPadding;
//		*foundHeader->m_Data = 101;
//		return reinterpret_cast<void*>(foundHeader->m_Data);
//
//
//		//return FirstFit(a_Count);
//		//return NextFit(a_Count);
//		//return BestFit(a_Count);
//		//return SegregatedFit(a_Count);
//	}
//
//	void FreeList::DoDeallocate(void* const a_Ptr, const std::size_t)
//	{
//		CROWN_PROFILE_FUNCTION();
//		const auto header = reinterpret_cast<Header*>(reinterpret_cast<char*>(a_Ptr) - sizeof(Header));
//		ASSERT_ENGINE(header, "Deallocating invalid pointer in Freelist allocator");
//		header->m_Used = false; //freed current header
//
//		Header* previous = reinterpret_cast<Header*>(header->m_Previous);
//		Header* next = reinterpret_cast<Header*>(header->m_Next);
//		Header* empty = header;
//
//		while (previous != nullptr)
//		{
//			if (previous->m_Used == false) //previous header grows over current header
//			{
//				previous->m_Next = empty->m_Next;
//				previous->m_Size += empty->m_Size;
//				if (empty->m_Next != nullptr)
//				{
//					reinterpret_cast<Header*>(empty->m_Next)->m_Previous = empty->m_Previous;
//				}
//				empty = previous;
//				previous = reinterpret_cast<Header*>(previous->m_Previous);
//				continue;
//			}
//			break;
//		}
//		while (next != nullptr)
//		{
//			if (next->m_Used == false) //current header grows over next header
//			{
//				empty->m_Next = next->m_Next;
//				empty->m_Size += next->m_Size;
//				if (next->m_Next != nullptr)
//				{
//					reinterpret_cast<Header*>(next->m_Next)->m_Previous = next->m_Previous;
//				}
//				next = reinterpret_cast<Header*>(next->m_Next);
//				continue;
//			}
//			break;
//		}
//
//		ASSERT_ENGINE(header->m_Next != header->m_Previous, "wrong deallocate");
//		//Header* previous = reinterpret_cast<Header*>(header->m_Previous);
//		//Header* previousValidated = header;
//		//Header* next = reinterpret_cast<Header*>(header->m_Next);
//		//Header* nextValidated = header; */
//		//
//		//if (previous == nullptr)
//		//{
//		//	previousValidated = nullptr;
//		//}
//		//if (next == nullptr)
//		//{
//		//	nextValidated = nullptr;
//		//}
//		//while (previous != nullptr)
//		//{
//		//	if (previous->m_Used == false)
//		//	{
//		//		previous->m_Size += previousValidated->m_Size;
//		//		previousValidated->m_Previous = nullptr;
//		//		previousValidated->m_Data = nullptr;
//		//		previousValidated = previous;
//		//		previous = reinterpret_cast<Header*>(previous->m_Previous);
//		//	}
//		//	else
//		//	{
//		//		previousValidated = previous;
//		//		break;
//		//	}
//		//}
//		//while (next != nullptr)
//		//{
//		//	if (next->m_Used == false)
//		//	{
//		//		nextValidated->m_Size += next->m_Size;
//		//		next->m_Size = 0;
//		//		next->m_Previous = nullptr;
//		//		nextValidated->m_Next = next->m_Next;
//		//		next->m_Next = nullptr;
//		//		next->m_Data = nullptr;
//		//		nextValidated->m_Data = nullptr;
//		//		next = reinterpret_cast<Header*>(nextValidated->m_Next);
//		//	}
//		//	else
//		//	{
//		//		nextValidated = next;
//		//		break;
//		//	}
//		//}
//		//header->m_Previous = nullptr;
//		//header->m_Next = nullptr;
//		//header->m_Data = nullptr;
//		//if (previousValidated != nullptr)
//		//{
//		//	previousValidated->m_Next = reinterpret_cast<char*>(nextValidated); //skipping all empty blocks in between
//		//}
//		//if (nextValidated != nullptr)
//		//{
//		//	nextValidated->m_Previous = reinterpret_cast<char*>(previousValidated); //freeing all adjacent empty memory blocks to form one block
//		//}
//	}
//
//	void FreeList::DoRelease()
//	{
//		// impl
//	}
//
//	void FreeList::FirstFit(std::size_t a_Size, const std::size_t a_Alignment, std::size_t& padding, Header*& a_PreviousHeader, Header*& a_FoundHeader) const
//	{
//		CROWN_PROFILE_FUNCTION();
//		Header* header = m_Firstheader;
//		Header* previousHeader = nullptr;
//
//		while (header != nullptr)
//		{
//			if (header->m_Used == false)
//			{
//				padding = alignForward(reinterpret_cast<const void* const>((reinterpret_cast<char*>(header) + a_Size)), a_Alignment);
//				//padding = CalculatePadding(reinterpret_cast<size_t>(header), a_Alignment);
//				//padding = CalculatePaddingWithHeader(reinterpret_cast<size_t>(header), a_Alignment, sizeof(Header));
//				const std::size_t requiredSpace = a_Size + padding;
//				if (header->m_Size >= requiredSpace)
//				{
//					break;
//				}
//			}
//			previousHeader = header;
//			header = reinterpret_cast<Header*>(header->m_Next);
//		}
//		a_PreviousHeader = previousHeader;
//		a_FoundHeader = header;
//	}
//
//	void FreeList::HeaderSplit(Header*& a_Header, const std::size_t a_RequiredSize, const std::size_t a_Remainder)
//	{
//		CROWN_PROFILE_FUNCTION();
//		const auto oldNextHeader = reinterpret_cast<Header*>(a_Header->m_Next);
//		const auto newHeader = reinterpret_cast<Header*>(reinterpret_cast<char*>(a_Header) + a_RequiredSize);
//
//		a_Header->m_Size = a_RequiredSize;
//		a_Header->m_Next = reinterpret_cast<char*>(newHeader);
//
//		newHeader->m_Size = a_Remainder - sizeof(Header);
//		newHeader->m_Next = reinterpret_cast<char*>(oldNextHeader);
//		newHeader->m_Previous = reinterpret_cast<char*>(a_Header);
//		newHeader->m_Used = false;
//		
//		if (oldNextHeader != nullptr)
//		{
//			oldNextHeader->m_Previous = reinterpret_cast<char*>(newHeader);
//		}
//	}
//
//
//
//
//
//	void* FreeList::FirstFit(std::size_t a_Size) const
//		{
//		CROWN_PROFILE_FUNCTION();
//		/*
//		 * on allocate, look for free space
//		 * start at beginning of array (first header)
//		 * on first fit approach
//		 * start with first header
//		 * check if space is available
//		 *		if space not available, go to next header
//		 *				if no next header
//		 *						return out of memory error
//		 *		if space available, check splitting size
//		 *				if splitting size, check next header
//		 *						if no next header
//		 *								create new header after data (current header + size of data + size of header)
//		 *								set current header's next header to new header
//		 *								set new header's previous header to current header
//		 *						if next header
//		 *								create new header after data (current header + size of data + size of header)
//		 *								set new header's previous header to current header
//		 *								set new header's next header to next header
//		 *								set current header's next header to new header
//		 *								set next header's previous header to new header
//		 *						set size of current header from previous size to data size
//		 *						set size of new header to previous size - data
//		 *						set current header's used to true
//		 *						return data address of current header data
//		 *				if not splitting size
//		 *						set current header's used to true
//		 *						return data address of current header data
//		 */
//		Header* header = m_Firstheader; //start with first header
//		while (header != nullptr) //loop over headers to find an empty one that fits
//		{
//			if (header->m_Used == false) //empty
//			{
//				const auto padding = alignForward(header, 8);
//				const auto requiredSize = a_Size + padding + sizeof(Header);
//				if (header->m_Size >= requiredSize) //fits
//				{
//					if (header->m_Size - requiredSize >= m_SplittingSize) //big enough to split
//					{
//						if (header->m_Next == nullptr) // last header
//						{
//							header->m_Next = reinterpret_cast<char*>(header) + requiredSize;
//							const auto headerNext = reinterpret_cast<Header*>(header->m_Next);
//							headerNext->m_Size = header->m_Size - requiredSize;
//							header->m_Size = requiredSize;
//							headerNext->m_Used = false;
//							header->m_Used = true;
//							headerNext->m_Next = nullptr;
//							headerNext->m_Previous = reinterpret_cast<char*>(header);
//							headerNext->m_Data = header->m_Next + sizeof(Header);
//							*header->m_Data = 101;
//							return header->m_Data;
//						}
//						//else // header exists after new header
//						{
//							const auto oldNext = reinterpret_cast<Header*>(header->m_Next);
//							header->m_Next = reinterpret_cast<char*>(header) + requiredSize;
//							const auto headerNext = reinterpret_cast<Header*>(header->m_Next);
//							headerNext->m_Size = header->m_Size - requiredSize;
//							header->m_Size = requiredSize;
//							headerNext->m_Used = false;
//							header->m_Used = true;
//							headerNext->m_Next = reinterpret_cast<char*>(oldNext);
//							headerNext->m_Previous = reinterpret_cast<char*>(header);
//							headerNext->m_Data = header->m_Next + sizeof(Header);
//							oldNext->m_Previous = header->m_Next;
//							*header->m_Data = 102;
//							return header->m_Data;
//						}
//					}
//					//not big enough to split
//					{
//						header->m_Used = true;
//						*header->m_Data = 103;
//						return header->m_Data;
//					}
//					
//				}
//			}
//			header = reinterpret_cast<Header*>(header->m_Next);
//		}
//		ASSERT_ENGINE(header != nullptr, "Freelist buffer overflow");
//		return nullptr; // out of memory
//	}
//}
