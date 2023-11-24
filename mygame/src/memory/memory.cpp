#include "memory.hpp"

bool isAligned(void* const _ptr, size_t _align) noexcept
{
	return (reinterpret_cast<uintptr_t>(_ptr) & (_align - 1)) == 0;
}
