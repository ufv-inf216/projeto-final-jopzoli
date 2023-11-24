/*
*
*/

#pragma once

#include "utils/define.hpp"

namespace detail
{
struct NOVTABLE ref_counter_base
{
	mutable int32_t count{ 1 };

	virtual void destroy( ) noexcept = 0;

	inline int32_t dec( ) noexcept
	{
		if (--count == 0)
			destroy( );

		return count;
	}

	inline int32_t inc( ) noexcept
	{
		return ++count;
	}

	NODISCARD inline bool isOne( ) const noexcept
	{
		return count == 1;
	}

	NODISCARD inline bool isZero( ) const noexcept
	{
		return count == 0;
	}

	inline int32_t useCount( ) const noexcept
	{
		return count;
	}
};

template <typename _Ty>
struct ptr_rc
{
public:
	using value_type = _Ty;

	NODISCARD inline useCount( ) const noexcept
	{
		return m_counter ? m_counter->useCount( ) : 0;
	}

	ptr_rc(ptr_rc&) = delete;
	ptr_rc& operator=(ptr_rc&) = delete;

private:
	value_type* m_ptr{ nullptr };
	ref_counter_base* m_counter{ nullptr };
};
}
