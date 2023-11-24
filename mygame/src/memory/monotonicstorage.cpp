#include "monotonicstorage.hpp"

MonotonicSegregatedStorage::MonotonicSegregatedStorage(std::pmr::memory_resource* const _resource) noexcept
	: m_currrentBlock{ nullptr },
	m_remainingSpace{ 0 },
	m_resource{ _resource },
	m_chunckList{ }
{ }

MonotonicSegregatedStorage::MonotonicSegregatedStorage(size_t _initSize,
	std::pmr::memory_resource* const _resource)
	: m_resource{ _resource }
{
	_grow(_initSize, Default_new_align);
	if (!m_currrentBlock)
		xBadAlloc( );
}

MonotonicSegregatedStorage::MonotonicSegregatedStorage(void* const _buffer, size_t _size) noexcept
	: m_currrentBlock{ _buffer },
	m_resource{ nullptr }
{
	_EXPECTS(isAligned(_buffer, Default_new_align),
		"given parameter _buffer should be aligned to Default_new_align");
	m_remainingSpace = _size - sizeof(chunck);
	m_chunckList.push(::new (static_cast<byte_t*>(_buffer) + m_remainingSpace)
		chunck{ _size, Default_new_align });
}

MonotonicSegregatedStorage::MonotonicSegregatedStorage(MonotonicSegregatedStorage&& _other) noexcept
	: m_currrentBlock{ std::exchange(_other.m_currrentBlock, nullptr) },
	m_remainingSpace{ std::exchange(_other.m_remainingSpace, 0) },
	m_resource{ std::exchange(_other.m_resource, nullptr) },
	m_chunckList{ std::exchange(_other.m_chunckList, { }) }
{ }

MonotonicSegregatedStorage& MonotonicSegregatedStorage::operator=(MonotonicSegregatedStorage&& _other) noexcept
{
	if (&_other != this)
	{
		release( );
		m_currrentBlock = std::exchange(_other.m_currrentBlock, nullptr);
		m_remainingSpace = std::exchange(_other.m_remainingSpace, 0);
		m_resource = std::exchange(_other.m_resource, nullptr);
		m_chunckList = std::exchange(_other.m_chunckList, { });
	}

	return *this;
}

void MonotonicSegregatedStorage::release( )
{
	if (m_resource)
	{
		m_currrentBlock = nullptr;
		m_remainingSpace = 0;

		chunck* nxt;
		while (!m_chunckList.isEmpty( ))
		{
			nxt = m_chunckList.pop( );
			m_resource->deallocate(nxt->getBaseAddress( ), nxt->size, nxt->align);
		}
	}
}

void MonotonicSegregatedStorage::_grow(size_t _size, size_t _align)
{
	if (m_resource)
	{
		if (_size >= _Max_buffer_size - sizeof(chunck))
			xBadAlloc( );

		_size += sizeof(chunck);
		size_t nxtSize = m_chunckList.isEmpty( ) ? _Min_buffer_size : _scaleSize(m_chunckList.top( )->size);
		if (nxtSize < _size)
			nxtSize = adjustOffset(_size, alignof(chunck));

		_align = std::max(_align, Default_new_align);
		void* const buffer = m_resource->allocate(nxtSize, _align);
		if (buffer)
		{
			_ASSERTION(isAligned(buffer, _align),
				"memory resource allocation didn't respect the required alignment");
			m_currrentBlock = buffer;
			m_remainingSpace = nxtSize - sizeof(chunck);
			m_chunckList.push(::new (static_cast<byte_t*>(buffer) + m_remainingSpace)
				chunck{ nxtSize, _align });
			return;
		}
	}

	m_currrentBlock = nullptr;
	// TODO: better warn
	_WARNING(m_currrentBlock, "couldn't allocate more memory from upstream resource");
}

size_t MonotonicSegregatedStorage::_roundSize(size_t _size) const noexcept
{
	if (_size < _Min_buffer_size)
		return _Min_buffer_size;
	if (_size >= _Max_buffer_size)
		return _Max_buffer_size;

	return adjustOffset(_size, alignof(chunck));
}

size_t MonotonicSegregatedStorage::_scaleSize(size_t _size) const noexcept
{
	constexpr size_t maxSize = (_Max_buffer_size - alignof(chunck) + 1) / 3 * 2;
	if (_size >= maxSize)
		return _Max_buffer_size;

	return adjustOffset(_size + (_size + 1) / 2, alignof(chunck));
}
