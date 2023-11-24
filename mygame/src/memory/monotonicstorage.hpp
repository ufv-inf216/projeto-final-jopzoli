/*
*
*/

#pragma once

#include "memory.hpp"

#define MONOTONIC_STACK_DEF_SIZE	1024

#define DECL_MONOTONIC_STACK_STORGE(x, sz)			\
byte_t buffer_##x##_##sz[sz];						\
MonotonicSegregatedStorage x(buffer_##x##_##sz, sz)

#define DECL_MONOTONIC_DOWNSTREAM_STORGE(x)	\
MonotonicSegregatedStorage x()

class MonotonicSegregatedStorage
	: public detail::identity_equal_resource
{
public:
	MonotonicSegregatedStorage(std::pmr::memory_resource* const _resource
		= std::pmr::get_default_resource( )) noexcept;

	MonotonicSegregatedStorage(size_t _initSize,
		std::pmr::memory_resource* const _resource = std::pmr::get_default_resource( ));

	MonotonicSegregatedStorage(void* const _buffer, size_t _size) noexcept;

	MonotonicSegregatedStorage(const MonotonicSegregatedStorage&) = delete;
	MonotonicSegregatedStorage& operator=(const MonotonicSegregatedStorage&) = delete;

	MonotonicSegregatedStorage(MonotonicSegregatedStorage&& _other) noexcept;
	MonotonicSegregatedStorage& operator=(MonotonicSegregatedStorage&& _other) noexcept;

	~MonotonicSegregatedStorage( ) override
	{
		release( );
	}

	void release( );

	std::pmr::memory_resource* upstreamResource( ) const noexcept
	{
		return m_resource;
	}

protected:
	void* do_allocate(size_t _size, size_t _align) override
	{
		if (!std::align(_align, _size, m_currrentBlock, m_remainingSpace))
			_grow(_size, _align);

		if (m_currrentBlock)
		{
			void* const ptr = m_currrentBlock;
			m_currrentBlock = static_cast<byte_t*>(m_currrentBlock) + _size;
			m_remainingSpace -= _size;

			return ptr;
		}

		return nullptr;
	}

	void do_deallocate(void* const, size_t, size_t) override
	{ }

private:
	struct chunck
		: detail::single_link<>
	{
		size_t size;
		size_t align;

		chunck(size_t _size, size_t _align) noexcept
			: size{ _size },
			align{ _align }
		{ }

		void* const getBaseAddress( ) const noexcept
		{
			return const_cast<char* const>(reinterpret_cast<const char*>(this + 1) - size);
		}
	};

	static constexpr size_t _Min_buffer_size
		= adjustOffset(4 * sizeof(chunck), Default_new_align);
	static constexpr size_t _Max_buffer_size = 0 - alignof(chunck);

	void* m_currrentBlock;
	size_t m_remainingSpace;
	std::pmr::memory_resource* m_resource;
	detail::linked_stack<chunck> m_chunckList;

	void _grow(size_t _size, size_t _align);
	size_t _roundSize(size_t _size) const noexcept;
	size_t _scaleSize(size_t _size) const noexcept;
};
