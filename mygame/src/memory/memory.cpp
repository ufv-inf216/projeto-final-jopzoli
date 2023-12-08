#include "memory.hpp"

std::pmr::unsynchronized_pool_resource singleton_memory::assynchronousPool = { };

std::pmr::synchronized_pool_resource singleton_memory::synchronousPool = { };

bool isAligned(void* const _ptr, size_t _align) noexcept
{
	return (reinterpret_cast<uintptr_t>(_ptr) & (_align - 1)) == 0;
}
