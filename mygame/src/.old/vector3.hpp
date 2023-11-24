/*
*
*/

#pragma once

#include "math.hpp"
#include <array>

template <std::floating_point _Ty = double>
class BasicVector3
{
public:
	using value_type = _Ty;
	union /* anonymous */
	{
		struct { value_type x; value_type y; value_type z; };
		// NOTE: meant to class internal use only
		std::array<_Ty, 3> data;
	};

	NODISCARD static consteval BasicVector3 i( ) noexcept
	{
		return BasicVector3(1, 0, 0);
	}

	NODISCARD static consteval BasicVector3 j( ) noexcept
	{
		return BasicVector3(0, 1, 0);
	}

	NODISCARD static consteval BasicVector3 k( ) noexcept
	{
		return BasicVector3(0, 0, 1);
	}

	NODISCARD static consteval BasicVector3 zero( ) noexcept
	{
		return BasicVector3(0);
	}

	NODISCARD static consteval BasicVector3 one( ) noexcept
	{
		return BasicVector3(1);
	}

#pragma warning(disable: WARN_VARINST_)
	constexpr BasicVector3( ) noexcept
		: x{ },
		y{ },
		z{ }
	{ }

	constexpr explicit BasicVector3(value_type _c) noexcept
		: x{_c},
		y{_c},
		z{_c}
	{ }

	constexpr BasicVector3(value_type _x, value_type _y, value_type _z) noexcept
		: x{_x},
		y{_y},
		z{_z}
	{ }
#pragma warning(default: WARN_VARINST_)

	NODISCARD bool is_inf( ) const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z);
	}

	NODISCARD bool is_nan( ) const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	NODISCARD bool is_norm( ) const noexcept
	{
		return almost_equal(mag2( ), static_cast<value_type>(1));
	}

	NODISCARD bool is_null( ) const noexcept
	{
		return x == 0 && y == 0 && z == 0;
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
			&& almost_equal(y, static_cast<_Ty>(0))
			&& almost_equal(z, static_cast<_Ty>(0));
	}

	NODISCARD bool is_zero(uint32_t _ulp) const noexcept
	{
		return almost_equal(x, static_cast<_Ty>(0), _ulp)
			&& almost_equal(y, static_cast<_Ty>(0), _ulp)
			&& almost_equal(z, static_cast<_Ty>(0), _ulp);
	}

	NODISCARD constexpr value_type mag2( ) const noexcept
	{
		return x * x + y * y + z * z;
	}

	NODISCARD value_type mag( ) const noexcept
	{
		return sqrt(mag2( ));
	}

	BasicVector3& normalize( ) noexcept
	{
		if (!is_null( ))
		{
			value_type m = mag( );
			x /= m;
			y /= m;
			z /= m;
		}

		return *this;
	}

	constexpr BasicVector3& set(value_type _x, value_type _y, value_type _z) noexcept
	{
		x = _x;
		y = _y;
		z = _z;

		return *this;
	}

	NODISCARD value_type thetaI( ) const noexcept
	{
		_EXPECTS(is_norm( ), "vector should be normalized");

		return std::acos(x);
	}

	NODISCARD value_type thetaJ( ) const noexcept
	{
		_EXPECTS(is_norm( ), "vector should be normalized");

		return std::acos(y);
	}

	NODISCARD value_type thetaK( ) const noexcept
	{
		_EXPECTS(is_norm( ), "vector should be normalized");

		return std::acos(z);
	}

	NODISCARD constexpr value_type& operator[ ](size_t _pos) noexcept
	{
		_EXPECTS(_pos < 3, "accessing position out-of-bounds");

		return data[_pos];
	}

	NODISCARD constexpr const value_type& operator[ ](size_t _pos) const noexcept
	{
		_EXPECTS(_pos < 3, "accessing position out-of-bounds");

		return data[_pos];
	}

	NODISCARD constexpr BasicVector3 operator-( ) const noexcept
	{
		return BasicVector3(-x, -y, -z);
	}

	NODISCARD constexpr BasicVector3 operator+(const BasicVector3& _rhs) const noexcept
	{
		return BasicVector3(x + _rhs.x, y + _rhs.y, z + _rhs.z);
	}

	constexpr BasicVector3& operator+=(const BasicVector3& _rhs) noexcept
	{
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator+(value_type _rhs) const noexcept
	{
		return BasicVector3(x + _rhs, y + _rhs, z + _rhs);
	}

	constexpr BasicVector3& operator+=(value_type _rhs) noexcept
	{
		x += _rhs;
		y += _rhs;
		z += _rhs;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator-(const BasicVector3& _rhs) const noexcept
	{
		return BasicVector3(x - _rhs.x, y - _rhs.y, z - _rhs.z);
	}

	constexpr BasicVector3& operator-=(const BasicVector3& _rhs) noexcept
	{
		x -= _rhs.x;
		y -= _rhs.y;
		z -= _rhs.z;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator-(value_type _rhs) const noexcept
	{
		return BasicVector3(x - _rhs, y - _rhs, z - _rhs);
	}

	constexpr BasicVector3& operator-=(value_type _rhs) noexcept
	{
		x -= _rhs;
		y -= _rhs;
		z -= _rhs;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator*(const BasicVector3& _rhs) const noexcept
	{
		return BasicVector3(x * _rhs.x, y * _rhs.y, z * _rhs.z);
	}

	constexpr BasicVector3& operator*=(const BasicVector3& _rhs) noexcept
	{
		x *= _rhs.x;
		y *= _rhs.y;
		z *= _rhs.z;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator*(value_type _rhs) const noexcept
	{
		return BasicVector3(x * _rhs, y * _rhs, z * _rhs);
	}

	constexpr BasicVector3& operator*=(value_type _rhs) noexcept
	{
		x *= _rhs;
		y *= _rhs;
		z *= _rhs;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator/(const BasicVector3& _rhs) const noexcept
	{
		_EXPECTS(!_rhs.is_null( ), "can't divide by data null vector");

		return BasicVector3(x / _rhs.x, y / _rhs.y, z / _rhs.z);
	}

	constexpr BasicVector3& operator/=(const BasicVector3& _rhs) noexcept
	{
		_EXPECTS(!_rhs.is_null( ), "can't divide by data null vector");
		x /= _rhs.x;
		y /= _rhs.y;
		z /= _rhs.z;

		return *this;
	}

	NODISCARD constexpr BasicVector3 operator/(value_type _rhs) const noexcept
	{
		_EXPECTS(_rhs != 0, "can't divide by zero");

		return BasicVector3(x / _rhs, y / _rhs, z / _rhs);
	}

	constexpr BasicVector3& operator/=(value_type _rhs) noexcept
	{
		_EXPECTS(_rhs != 0, "can't divide by zero");
		x /= _rhs;
		y /= _rhs;
		z /= _rhs;

		return *this;
	}

	NODISCARD bool operator==(const BasicVector3& _rhs) const noexcept
	{
		return x == _rhs.x && y == _rhs.y && z == _rhs.z;
	}

	NODISCARD bool operator!=(const BasicVector3& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
};

template <std::integral _Ty>
BasicVector3(_Ty, _Ty, _Ty) -> BasicVector3<double>;

using Vector3 = BasicVector3<>;
using FVector3 = BasicVector3<float>;

namespace std_utils
{

template <typename _Ty>
std::ostream& operator<<(std::ostream& _os, const BasicVector3<_Ty>& _rhs) noexcept
{
	_os << '(' << _rhs.x << ", "
		<< _rhs.y << ", "
		<< _rhs.z << ')';

	return _os;
}

}

template <typename _Ty>
NODISCARD constexpr BasicVector3<_Ty> cross(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	return BasicVector3(_lhs.y * _rhs.z - _lhs.z * _rhs.y,
		_lhs.z * _rhs.x - _lhs.x * _rhs.z,
		_lhs.x * _rhs.y - _lhs.y * _rhs.x
	);
}

template <typename _Ty>
NODISCARD constexpr _Ty dist2(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	return BasicVector3(_lhs.x - _rhs.x, _lhs.y - _rhs.y, _lhs.z - _rhs.z).mag2( );
}

template <typename _Ty>
NODISCARD _Ty dist(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	return sqrt(dist2(_lhs, _rhs));
}

template <typename _Ty>
NODISCARD constexpr _Ty dot(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z;
}

template <typename _Ty>
NODISCARD constexpr bool equal(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	return almost_equal(_lhs.x, _rhs.x)
		&& almost_equal(_lhs.y, _rhs.y)
		&& almost_equal(_lhs.z, _rhs.z);
}

template <typename _Ty>
NODISCARD constexpr bool equal(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs, uint32_t _ulp) noexcept
{
	return almost_equal(_lhs.x, _rhs.x, _ulp)
		&& almost_equal(_lhs.y, _rhs.y, _ulp)
		&& almost_equal(_lhs.z, _rhs.z, _ulp);
}

template <typename _Ty>
NODISCARD constexpr BasicVector3<_Ty> lerp(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs, _Ty _t) noexcept
{
	if (_t == static_cast<_Ty>(0))
		return _lhs;
	else if (_t == static_cast<_Ty>(1))
		return _rhs;

	return BasicVector3(_lhs + (_rhs - _lhs) * _t);
}

template <typename _Ty>
NODISCARD constexpr BasicVector3<_Ty> proj(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	_EXPECTS(!_rhs.is_null( ) && _rhs.is_norm( ), "right hand side vector should be non-null and normalized");

	return _rhs * dot(_lhs, _rhs);
}

template <typename _Ty>
NODISCARD _Ty theta(const BasicVector3<_Ty>& _lhs, const BasicVector3<_Ty>& _rhs) noexcept
{
	_EXPECTS(_lhs.is_norm( ) && _rhs.is_norm( ), "given vectors should be normalized");

	return std::acos(dot(_lhs, _rhs));
}
