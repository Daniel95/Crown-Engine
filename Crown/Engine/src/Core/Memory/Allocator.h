#pragma once
//#include <typeinfo>
#include <cstdlib>

#include "Core/Memory/AllocationStrategy.h"

namespace Crown
{
	template <class T>
	class Allocator {
	public:
		using _From_primary = Allocator;
 
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
 
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal = std::true_type;
 
		constexpr Allocator() noexcept = default;
		constexpr Allocator(const Allocator&) noexcept = default;
		template <class Other>
		explicit constexpr Allocator(const Allocator<Other>&) noexcept {}
	private:
		AllocationStrategy* m_AllocationStrategy{ nullptr };
	public:
		Allocator(AllocationStrategy* a_Strategy) :m_AllocationStrategy{ a_Strategy } {}; //explicit breaks initializer list. e.g. std::vector<int, Allocator<int>> monotonicAllocator{&monotonic};

		Allocator(AllocationStrategy& a_Strategy) :m_AllocationStrategy{ &a_Strategy } {};
 
		T* allocate(const std::size_t a_Count)//lower case required for template code in std:: container classes
		{
			CROWN_PROFILE_FUNCTION();

			if (m_AllocationStrategy != nullptr)
			{
				return reinterpret_cast<T*>(m_AllocationStrategy->allocate(sizeof(T) * a_Count, alignof(T)));
			}
			else
			{
#ifdef _DEBUG_MEM_TEXT
				auto location = static_cast<T*>(std::malloc(sizeof(T) * a_Count));
				///
				assert(location && "Failed to malloc new memory");
				const auto locationString = static_cast<const void*>(location);
				std::printf("Warning default: Allocate %zu bytes at %p\n", a_Count * sizeof(T), locationString);
				//std::cout << "Warning default: Allocate " << a_Count * sizeof(T)
				//	<< " bytes at " << locationString << " (type "
				//	<< _TYPEID(location).name() << ")." << std::endl;
				///
				return location;
#else
				
				return reinterpret_cast<T*>(std::malloc(sizeof(T) * a_Count));
#endif
			}
		}
 
		T* allocate(const std::size_t a_Count, const void*)//lower case required for template code in std:: container classes
		{
			CROWN_PROFILE_FUNCTION();
			if (m_AllocationStrategy != nullptr)
			{
				return reinterpret_cast<T*>(m_AllocationStrategy->allocate(sizeof(T) * a_Count, alignof(T)));
				//return static_cast<T*>(m_AllocationStrategy->allocate(sizeof(T) * a_Count, alignof(T)));
			}
			else
			{
#ifdef _DEBUG_MEM_TEXT
				auto location = reinterpret_cast<T*>(std::malloc(sizeof(T) * a_Count));
				///
				assert(location && "Failed to malloc new memory");
				const auto locationString = reinterpret_cast<const void*>(location);
				std::printf("Warning default: Allocate %zu bytes at %p\n", a_Count * sizeof(T), locationString);
				//std::cout << "Warning default: Allocate " << a_Count * sizeof(T)
				//	<< " bytes at " << locationString << " (type "
				//	<< _TYPEID(location).name() << ")." << std::endl;
				///
				return location;
#else
				return reinterpret_cast<T*>(std::malloc(sizeof(T) * a_Count));
				//return static_cast<T*>(std::malloc(sizeof(T) * a_Count));
#endif
			}
		}
 
		void deallocate(T* const a_Ptr, const std::size_t a_Count)//lower case required for template code in std:: container classes
		{
			CROWN_PROFILE_FUNCTION();
			
			if (m_AllocationStrategy != nullptr)
			{
				m_AllocationStrategy->deallocate(a_Ptr, sizeof(T) * a_Count);
			}
			else
			{
#ifdef _DEBUG_MEM_TEXT
				///
				std::printf("Warning default: Deallocate %zu bytes at %p\n", a_Count * sizeof(T), a_Ptr);
				//std::cout << "Warning default: Deallocate " << a_Count * sizeof(T)
				//	<< " bytes at " << a_Ptr << " (type "
				//	<< _TYPEID(T).name() << ")." << std::endl;
				///
#endif
				std::free(a_Ptr);
			}
		}
	};
	
}//Crown