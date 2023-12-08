#include "object.hpp"
#include "utils/logger.hpp"

std::pmr::synchronized_pool_resource singleton_object_memory::synchronousPool{ };

void* Object::operator new(std::size_t _count)
{
	return _allocate(_count);
}

void* Object::operator new(std::size_t _count, size_t _align)
{
	return _allocate(_count, _align);
}

void Object::operator delete(void* _block, size_t _count)
{
	_deallocate(_block, _count);
}

void Object::operator delete(void* _block, size_t _count, size_t _align)
{
	_deallocate(_block, _count, _align);
}

void* Object::_allocate(size_t _count)
{
	return singleton_object_memory::getSynchronousPool( )->allocate(
		_count);
}

void* Object::_allocate(size_t _count, size_t _align)
{
	return singleton_object_memory::getSynchronousPool( )->allocate(
		_count,
		_align);
}

void Object::_deallocate(void* _block, size_t _count)
{
	singleton_object_memory::getSynchronousPool( )->deallocate(
		_block,
		_count);
}

void Object::_deallocate(void* _block, size_t _count, size_t _align)
{
	singleton_object_memory::getSynchronousPool( )->deallocate(
		_block,
		_count,
		_align);
}
