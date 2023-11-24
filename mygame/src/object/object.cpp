#include "object.hpp"
#include "utils/logger.hpp"

Object::Object( ) noexcept
{ }

void* Object::operator new(std::size_t _count)
{
	return singleton_memory::getPool( )->allocate(_count);
}

void* Object::operator new(std::size_t _count, size_t _align)
{
	return singleton_memory::getPool( )->allocate(_count, _align);
}

void Object::operator delete(void* _block, size_t _count)
{
	singleton_memory::getPool( )->deallocate(_block, _count);
}

void Object::operator delete(void* _block, size_t _count, size_t _align)
{
	singleton_memory::getPool( )->deallocate(_block, _count, _align);
}
