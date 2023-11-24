/*
*
*/

#pragma once

#include "math.hpp"

template <std::floating_point _Ty = double>
class BasicVector2
{
public:
	using value_type = _Ty;
	union /* anonymous */
	{
		struct { value_type x; value_type y; };
		// NOTE: meant to class internal use only
		std::array<_Ty, 2> data;
	};

	NODISCARD static consteval BasicVector2 i( ) noexcept
	{
		return BasicVector2(1, 0);
	}

	NODISCARD static consteval BasicVector2 j( ) noexcept
	{
		return BasicVector2(0, 1);
	}

	NODISCARD static consteval BasicVector2 zero( ) noexcept
	{
		return BasicVector2(0);
	}

	NODISCARD static consteval BasicVector2 one( ) noexcept
	{
		return BasicVector2(1);
	}

#pragma warning(disable: WARN_VARINST_)
	constexpr BasicVector2( ) noexcept
		: x{ },
		y{ }
	{ }

	constexpr explicit BasicVector2(value_type _c) noexcept
		: x{ _c },
		y{ _c }
	{ }

	constexpr BasicVector2(value_type _x, value_type _y) noexcept
		: x{ _x },
		y{ _y }
	{ }
#pragma warning(default: WARN_VARINST_)

	NODISCARD bool is_inf( ) const
	{
		return std::isinf(x) || std::isinf(y);
	}

	NODISCARD bool is_nan( ) const
	{
		return std::isnan(x) || std::isnan(y);
	}

	NODISCARD bool is_norm( ) const noexcept
	{
		return almost_equal(mag2( ), static_cast<value_type>(1));
	}

	NODISCARD bool is_null( ) const noexcept
	{
		return almost_equal(x, static_cast<value_type>(0))
			&& almost_equal(y, static_cast<value_type>(0));
	}

	NODISCARD bool is_unit( ) const noexcept
	{
		return is_norm( );
	}

	NODISCARD bool is_unit(uint32_t _ulp) const noexcept
	{
		return almost_equal(mag2( ), static_cast<_Ty>(1), _ulp);
	}

	NODISCARD bool is_zero( ) const noexcept
	{
		return almost_equal(x, static_cast<_Ty>(0))
			&& almost_equal(y, static_cast<_Ty>(0));
	}

	NODISCARD bool is_zero(uint32_t _ulp) const noexcept
	{
		return almost_equal(x, static_cast<_Ty>(0), _ulp)
			&& almost_equal(y, static_cast<_Ty>(0), _ulp);
	}

	NODISCARD constexpr value_type mag2( ) const noexcept
	{
		return x * x + y * y;
	}

	NODISCARD value_type mag( ) const noexcept
	{
		return sqrt(mag2( ));
	}

	BasicVector2& normalize( ) noexcept
	{
		if (!is_null( ))
		{
			value_type m = mag( );
			x /= m;
			y /= m;
		}

		return *this;
	}

	NODISCARD constexpr BasicVector2 orth( ) const noexcept
	{
		return BasicVector2(-y, x);
	}

	constexpr BasicVector2& set(value_type _x, value_type _y) noexcept
	{
		x = _x;
		y = _y;

		return *this;
	}

	NODISCARD value_type theta( ) const noexcept
	{
		return std::acos(x);
	}

	NODISCARD constexpr value_type& operator[ ](size_t _pos) noexcept
	{
		_EXPECTS(_pos < 2, "accessing position out-of-bounds");

		return data[_pos];
	}

	NODISCARD constexpr const value_type& operator[ ](size_t _pos) const noexcept
	{
		_EXPECTS(_pos < 2, "accessing position out-of-bounds");

		return data[_pos];
	}

	NODISCARD constexpr BasicVector2 operator-( ) const noexcept
	{
		return BasicVector2(-x, -y);
	}

	NODISCARD constexpr BasicVector2 operator+(const BasicVector2& _rhs) const noexcept
	{
		return BasicVector2(x + _rhs.x, y + _rhs.y);
	}

	constexpr BasicVector2& operator+=(const BasicVector2& _rhs) noexcept
	{
		x += _rhs.x;
		y += _rhs.y;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator+(value_type _rhs) const noexcept
	{
		return BasicVector2(x + _rhs, y + _rhs);
	}

	constexpr BasicVector2& operator+=(value_type _rhs) noexcept
	{
		x += _rhs;
		y += _rhs;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator-(const BasicVector2& _rhs) const noexcept
	{
		return BasicVector2(x - _rhs.x, y - _rhs.y);
	}

	constexpr BasicVector2& operator-=(const BasicVector2& _rhs) noexcept
	{
		x -= _rhs.x;
		y -= _rhs.y;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator-(value_type _rhs) const noexcept
	{
		return BasicVector2(x - _rhs, y - _rhs);
	}

	constexpr BasicVector2& operator-=(value_type _rhs) noexcept
	{
		x -= _rhs;
		y -= _rhs;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator*(const BasicVector2& _rhs) const noexcept
	{
		return BasicVector2(x * _rhs.x, y * _rhs.y);
	}

	constexpr BasicVector2& operator*=(const BasicVector2& _rhs) noexcept
	{
		x *= _rhs.x;
		y *= _rhs.y;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator*(value_type _rhs) const noexcept
	{
		return BasicVector2(x * _rhs, y * _rhs);
	}

	constexpr BasicVector2& operator*=(value_type _rhs) noexcept
	{
		x *= _rhs;
		y *= _rhs;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator/(const BasicVector2& _rhs) const noexcept
	{
		_EXPECTS(!_rhs.is_null( ), "can't divide by data null vector");

		return BasicVector2(x / _rhs.x, y / _rhs.y);
	}

	constexpr BasicVector2& operator/=(const BasicVector2& _rhs) noexcept
	{
		_EXPECTS(!_rhs.is_null( ), "can't divide by data null vector");
		x /= _rhs.x;
		y /= _rhs.y;

		return *this;
	}

	NODISCARD constexpr BasicVector2 operator/(value_type _rhs) const noexcept
	{
		_EXPECTS(_rhs != 0, "can't divide by zero");

		return BasicVector2(x / _rhs, y / _rhs);
	}

	constexpr BasicVector2& operator/=(value_type _rhs) noexcept
	{
		_EXPECTS(_rhs != 0, "can't divide by zero");
		x /= _rhs;
		y /= _rhs;

		return *this;
	}

	NODISCARD bool operator==(const BasicVector2& _rhs) const noexcept
	{
		return x == _rhs.x && y == _rhs.y;
	}

	NODISCARD bool operator!=(const BasicVector2& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
};

template <std::integral _Ty>
BasicVector2(_Ty, _Ty) -> BasicVector2<double>;

using Vector2 = BasicVector2<>;
using FVector2 = BasicVector2<float>;

namespace std_utils
{

template <typename _Ty>
std::ostream& operator<<(std::ostream& _os, const BasicVector2<_Ty>& _rhs) noexcept
{
	_os << '(' << _rhs.x << ", "
		<< _rhs.y << ')';

	return _os;
}

}

template <typename _Ty>
NODISCARD constexpr _Ty cross(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	return _lhs.x * _rhs.y - _lhs.y * _rhs.x;
}

template <typename _Ty>
NODISCARD constexpr _Ty dist2(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	return BasicVector2(_lhs.x - _rhs.x, _lhs.y - _rhs.y).mag2( );
}

template <typename _Ty>
NODISCARD _Ty dist(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	return sqrt(dist2(_lhs, _rhs));
}

template <typename _Ty>
NODISCARD constexpr _Ty dot(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y;
}

template <typename _Ty>
NODISCARD constexpr bool equal(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	return almost_equal(_lhs.x, _rhs.x)
		&& almost_equal(_lhs.y, _rhs.y);
}

template <typename _Ty>
NODISCARD constexpr bool equal(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs, uint32_t _ulp) noexcept
{
	return almost_equal(_lhs.x, _rhs.x, _ulp)
		&& almost_equal(_lhs.y, _rhs.y, _ulp);
}

template <typename _Ty>
NODISCARD constexpr BasicVector2<_Ty> lerp(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs, _Ty _t) noexcept
{
	if (_t == static_cast<_Ty>(0))
		return _lhs;
	else if (_t == static_cast<_Ty>(1))
		return _rhs;

	return BasicVector2(_lhs + (_rhs - _lhs) * _t);
}

template <typename _Ty>
NODISCARD constexpr BasicVector2<_Ty> proj(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	_EXPECTS(!_rhs.is_null( ) && _rhs.is_norm( ), "right hand side vector should be non-null and normalized");

	return _rhs * dot(_lhs, _rhs);
}

template <typename _Ty>
NODISCARD _Ty theta(const BasicVector2<_Ty>& _lhs, const BasicVector2<_Ty>& _rhs) noexcept
{
	_EXPECTS(_lhs.is_norm( ) && _rhs.is_norm( ), "given vectors should be normalized");

	return std::acos(dot(_lhs, _rhs));
}
