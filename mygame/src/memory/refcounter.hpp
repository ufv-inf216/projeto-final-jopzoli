/*
*
*/

#pragma once

#include "memory.hpp"

namespace detail
{
struct NOVTABLE ref_counter_base
{
	NODISCARDRAWPTR static void* operator new(std::size_t _count)
	{
		return singleton_memory::getAssynchronousPool( )->allocate(_count);
	}

	NODISCARDRAWPTR static void* operator new(std::size_t _count, size_t _align)
	{
		return singleton_memory::getAssynchronousPool( )->allocate(_count, _align);
	}

	static void operator delete(void* _block, size_t _count)
	{
		return singleton_memory::getAssynchronousPool( )->deallocate(_block, _count);
	}

	static void operator delete(void* _block, size_t _count, size_t _align)
	{
		return singleton_memory::getAssynchronousPool( )->deallocate(_block, _count, _align);
	}

	std::atomic_int32_t count{ 1 };

	virtual void destroy( ) noexcept = 0;

	void dec( ) noexcept
	{
		if (count.fetch_sub(1, std::memory_order_acq_rel) == 1)
			destroy( );
	}

	void inc( ) noexcept
	{
		count.fetch_add(1, std::memory_order_relaxed);
	}

	NODISCARD bool isOne( ) const noexcept
	{
		return count.load(std::memory_order_acquire) == 1;
	}

	NODISCARD bool isZero( ) const noexcept
	{
		return count.load(std::memory_order_acquire) == 0;
	}

	int32_t useCount( ) const noexcept
	{
		return count.load(std::memory_order_acquire);
	}

	int32_t useCountRelaxed( ) const noexcept
	{
		return count.load(std::memory_order_relaxed);
	}
};

template <typename _Ty>
struct ref_counter
	: ref_counter_base
{
public:
	using value_type = _Ty;
	using base_type = ref_counter_base;

	explicit ref_counter(value_type* _ptr) noexcept
		: base_type{ },
		ptr{ _ptr }
	{ }

protected:
	value_type* ptr{ nullptr };

	void destroy( ) noexcept override
	{
		delete ptr;
		delete this;
	}
};
}

template <typename _Ty>
struct ptr_rc_base
{
	template <typename>
	friend struct object_ptr;
public:
	using value_type = _Ty;

	// TODO: make private
	value_type* ptr{ nullptr };
	detail::ref_counter_base* refCounter{ nullptr };

	NODISCARD int32_t use_count( ) const noexcept
	{
		return refCounter ? refCounter->useCount( ) : 0;
	}

	template <typename _OtherTy>
	NODISCARD bool ownerBefore(const ptr_rc_base<_OtherTy>& _other) const noexcept
	{
		return refCounter < _other.refCounter;
	}

	ptr_rc_base(const ptr_rc_base&) = delete;
	ptr_rc_base& operator=(const ptr_rc_base&) = delete;

protected:
	constexpr ptr_rc_base( ) = default;
	~ptr_rc_base( ) = default;

	NODISCARD value_type* get( ) const noexcept
	{
		return ptr;
	}

	template <typename _OtherTy>
	void moveConstructFrom(ptr_rc_base<_OtherTy>&& _rhs) noexcept
	{
		ptr = _rhs.ptr;
		refCounter = _rhs.refCounter;
		_rhs.ptr = nullptr;
		_rhs.refCounter = nullptr;
	}

	template <typename _OtherTy>
	void copyConstructFrom(const ptr_rc_base<_OtherTy>& _rhs) noexcept
	{
		_rhs.refCounter->inc( );
		ptr = _rhs.ptr;
		refCounter = _rhs.refCounter;
	}

	void incRef( ) const noexcept
	{
		if (refCounter)
			refCounter->inc( );
	}
	
	void decRef( ) const noexcept
	{
		if (refCounter)
			refCounter->dec( );
	}

	void swap(ptr_rc_base& _rhs) noexcept
	{
		std::swap(ptr, _rhs.ptr);
		std::swap(refCounter, _rhs.refCounter);
	}
};
