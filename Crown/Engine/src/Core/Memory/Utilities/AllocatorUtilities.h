#pragma once
#include "Core/Utilities/Defines.h"
namespace Crown
{
	static std::size_t CalculatePadding(const std::size_t a_Address, const std::size_t a_Alignment)
	{
		CROWN_PROFILE_FUNCTION();
		const auto multiplier = (a_Address / a_Alignment) + 1;
		const auto alignedAddress = multiplier * a_Alignment;
		const auto padding = alignedAddress - a_Address;
		return padding;
	}
	static std::size_t CalculatePaddingWithHeader(const std::size_t a_Address, const std::size_t a_Alignment, const std::size_t a_HeaderSize)
	{
		CROWN_PROFILE_FUNCTION();
		std::size_t padding = CalculatePadding(a_Address, a_Alignment);
		std::size_t neededSpace = a_HeaderSize;

		if (padding < neededSpace)
		{
			// Header does not fit - Calculate next aligned address that header fits
			neededSpace -= padding;

			// How many alignments I need to fit the header
			if (neededSpace % a_Alignment > 0)
			{
				padding += a_Alignment * (1 + (neededSpace / a_Alignment));
			}
			else
			{
				padding += a_Alignment * (neededSpace / a_Alignment);
			}
		}
		return padding;
	}
	//static std::size_t AlignForward(const void* const a_Address, std::size_t a_Alignment)
	//{
	//	CROWN_PROFILE_FUNCTION();
	//	ASSERT_ENGINE(!(a_Alignment % 2), "Alignment is not a power of 2");
	//	//a_Address = (a_Address + (a_Alignment - 1)) & -a_Alignment;
	//	const std::size_t adjustment = a_Alignment - (reinterpret_cast<uintptr_t>(a_Address)& static_cast<uintptr_t>(a_Alignment - 1));
	//	if (adjustment == a_Alignment) return 0;

	//	return adjustment;
	//}
	//static void* AlignForwardAddress(void* a_Address, std::size_t a_Alignment)
	//{
	//	CROWN_PROFILE_FUNCTION();
	//	ASSERT_ENGINE(!(a_Alignment % 2), "Alignment is not a power of 2");
	//	//a_Address = (a_Address + (a_Alignment - 1)) & -a_Alignment;
	//	const std::size_t adjustment = a_Alignment - (reinterpret_cast<uintptr_t>(a_Address)& static_cast<uintptr_t>(a_Alignment - 1));
	//	if (adjustment == a_Alignment) return a_Address;

	//	const auto returnAddress = reinterpret_cast<void*>(reinterpret_cast<char*>(a_Address) + adjustment);
	//	return returnAddress;
	//}
}
