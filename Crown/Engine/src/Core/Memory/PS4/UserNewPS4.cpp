#ifdef __ORBIS__
#include "Core/Memory/MemoryCustom.h"
#ifndef NO_CUSTOM_ALLOCATOR
#include <new>
#include <cstdio>//used for size_t (same name for PS4 and Windows)
 
//#include "Core/Memory/MemoryCustom.h"
 
void* operator new(std::size_t a_Size)
{
	std::printf("global operator new(size_t) called, size = %zu\n", a_Size);
	return Crown::g_Allocator.allocate(a_Size, sizeof(a_Size));
}
 
void* operator new(std::size_t a_Size, const std::nothrow_t&) noexcept
{
	std::printf("global operator new(size_t,nothrow) called, size = %zu\n", a_Size);
	return Crown::g_Allocator.allocate(a_Size, sizeof(a_Size));
}
 
void* operator new[](std::size_t a_Size)
{
	std::printf("global operator new[](size_t) called, size = %zu\n", a_Size);
	return Crown::g_Allocator.allocate(a_Size, sizeof(a_Size));
}
 
void* operator new[](std::size_t a_Size, const std::nothrow_t&) noexcept
{
	std::printf("global operator new(size_t,nothrow) called, size = %zu\n", a_Size);
	return Crown::g_Allocator.allocate(a_Size, sizeof(a_Size));
}
 
void operator delete(void* a_Ptr) noexcept
{
	std::printf("global operator delete(void*) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, 0/*, _Size*/);
}
 
void operator delete(void* a_Ptr, const std::nothrow_t&) noexcept
{
	std::printf("global operator delete(void*,nothrow) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, 0/*, _Size*/);
}
 
void operator delete[](void* a_Ptr) noexcept
{
	std::printf("global operator delete[](void*) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, 0/*, _Size*/);
}
 
void operator delete[](void* a_Ptr, const std::nothrow_t&) noexcept
{
	std::printf("global operator delete[](void*,nothrow) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, 0/*, _Size*/);
}
 
void operator delete(void* a_Ptr, std::size_t a_Size) noexcept
{
	std::printf("global operator delete(void*,size_t) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, a_Size);
}
void operator delete(void* a_Ptr, std::size_t a_Size, const std::nothrow_t&) noexcept
{
	std::printf("global operator delete(void*,size_t,nothrow) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, a_Size);
}
void operator delete[](void* a_Ptr, std::size_t a_Size) noexcept
{
	std::printf("global operator delete[](void*,size_t) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, a_Size);
}
void operator delete[](void* a_Ptr, std::size_t a_Size, const std::nothrow_t&) noexcept
{
	std::printf("global operator delete[](void*,size_t,nothrow) called");/*, size = %zu\n", _Size);*/
	Crown::g_Allocator.deallocate(a_Ptr, a_Size);
}

#endif //No_CUSTOM_ALLOCATOR
#endif //Orbis