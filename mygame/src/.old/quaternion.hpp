/*
*
*/

#pragma once

#include "math.hpp"
#include "matrix4.hpp"
#include "vector3.hpp"

template <std::floating_point _Ty = double>
class BasicQuaternion
{
public:
	using value_type = _Ty;
	union /* anonymous */
	{
		struct { value_type x; value_type y; value_type z; value_type w; };
		// NOTE: meant to class internal use only
		std::array<_Ty, 3> data;
	};

	NODISCARD static consteval BasicQuaternion i( ) noexcept
	{
		return BasicQuaternion(0, 0, 0, 1);
	}

	NODISCARD static consteval BasicQuaternion zero( ) noexcept
	{
		return BasicQuaternion( );
	}

	NODISCARD BasicQuaternion fromEuler(value_type _r, value_type _p, value_type _y) noexcept
	{
		_r /= 2;
		_p /= 2;
		_y /= 2;

		_Ty cr = cos(_r),
			cp = cos(_p),
			cy = cos(_y),
			sr = cos(_r),
			sp = sin(_p),
			sy = sin(_y);

		return BasicQuaternion(
			BasicVector3<value_type>(
				cr * cp * sy - sr * sp * cy,
				cr * sp * cy + sr * cp * sy,
				sr * cp * cy - cr * sp * sy
			),
			cr * cp * cy + sr * sp * sy
		);
	}

	NODISCARD static _CONSTEXPRG BasicQuaternion makeReflection(const BasicVector3<value_type>& _n) noexcept
	{
		_EXPECTS(_n.is_norm( ), "vector should be normalized");

		return BasicQuaternion(_n, 0);
	}

	NODISCARD static BasicQuaternion makeRotation(value_type _theta, const BasicVector3<value_type>& _n) noexcept
	{
		_EXPECTS(_n.is_norm( ), "vector should be normalized");

		return BasicQuaternion(_n * sin(_theta / 2), cos(_theta / 2));
	}

#pragma warning(disable: WARN_VARINST_)
	constexpr BasicQuaternion( ) noexcept
		: x{ },
		y{ },
		z{ },
		w{ 1 }
	{ }

	constexpr explicit BasicQuaternion(value_type _c) noexcept
		: x{ _c },
		y{ _c },
		z{ _c },
		w{ _c }
	{ }

	constexpr BasicQuaternion(const BasicVector3<value_type>& _n) noexcept
		: x{ _n.x },
		y{ _n.y },
		z{ _n.z },
		w{ 0 }
	{ }

	constexpr BasicQuaternion(const BasicVector3<value_type>& _n, value_type _w) noexcept
		: x{ _n.x },
		y{ _n.y },
		z{ _n.z },
		w{ _w }
	{ }
#pragma warning(default: WARN_VARINST_)

	NODISCARD BasicVector3<value_type> axiz( ) const noexcept
	{
		_EXPECTS(is_norm( ), "vector should be normalized");

		return imaginary( ) / std::sqrt(1 - w * w);
	}

	NODISCARD constexpr BasicQuaternion conjugate( ) const noexcept
	{
		return BasicQuaternion(-imaginary( ), w);
	}

	NODISCARD constexpr BasicVector3<value_type> imaginary( ) const noexcept
	{
		return BasicVector3(x, y, z);
	}

	NODISCARD constexpr BasicQuaternion inverse( ) const noexcept
	{
		return conjugate( ) / mag2( );
	}

	NODISCARD bool is_inf( ) const
	{
		return std::isinf(x) || std::isinf(y) || std::isinf(z) || std::isinf(w);
	}

	NODISCARD bool is_nan( ) const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w);
	}

	NODISCARD bool is_norm( ) const noexcept
	{
		return almost_equal(mag2( ), static_cast<value_type>(1));
	}

	NODISCARD constexpr bool is_null( ) const noexcept
	{
		return x == 0 && y == 0 && z == 0 && w == 0;
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
			&& almost_equal(z, static_cast<_Ty>(0))
			&& almost_equal(w, static_cast<_Ty>(0));
	}

	NODISCARD bool is_zero(uint32_t _ulp) const noexcept
	{
		return almost_equal(x, static_cast<_Ty>(0), _ulp)
			&& almost_equal(y, static_cast<_Ty>(0), _ulp)
			&& almost_equal(z, static_cast<_Ty>(0), _ulp)
			&& almost_equal(w, static_cast<_Ty>(0), _ulp);
	}

	NODISCARD constexpr value_type mag2( ) const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	NODISCARD value_type mag( ) const noexcept
	{
		return sqrt(mag2( ));
	}

	BasicQuaternion& normalize( ) noexcept
	{
		value_type m = mag( );
		if (m != 0)
		{
			x /= m;
			y /= m;
			z /= m;
			w /= m;
		} else
			*this = i( );

		return *this;
	}

	NODISCARD constexpr value_type real( ) const noexcept
	{
		return w;
	}

	constexpr BasicQuaternion& set(value_type _w) noexcept
	{
		w = _w;

		return *this;
	}

	constexpr BasicQuaternion& set(value_type _x, value_type _y, value_type _z) noexcept
	{
		x = _x;
		y = _y;
		z = _z;

		return *this;
	}

	constexpr BasicQuaternion& set(const BasicVector3<value_type>& _v, value_type _w) noexcept
	{
		x = _v._x;
		y = _v._y;
		z = _v._z;
		w = _w;

		return *this;
	}

	NODISCARD value_type theta( ) const noexcept
	{
		_EXPECTS(is_norm( ), "vector should be normalized");

		return 2 * std::acos(w);
	}

	NODISCARD BasicVector3<value_type> toEuler( ) const noexcept
	{
		value_type p = 2 * (w * y - x * z);

		return BasicVector3(
			std::atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)),
			2 * std::atan2(std::sqrt(1 + p), std::sqrt(1 - p)) - PiO2,
			std::atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z))
		);
	}

	NODISCARD constexpr BasicMatrix4<value_type> toMatrix( ) const noexcept
	{
		return BasicMatrix4({
			w, -z, y, x,
			z, w, -x, y,
			-y, x, w, z,
			-x, -y, -z, w
			});
	}

	NODISCARD constexpr BasicMatrix4<value_type> toRotationMatrix( ) const noexcept
	{
		return BasicMatrix4({
			1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w,
			2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w,
			2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y
			});
	}

	NODISCARD constexpr value_type& operator[ ](size_t _pos) noexcept
	{
		_EXPECTS(_pos < 4, "accessing position out-of-bounds");

		return data[_pos];
	}

	NODISCARD constexpr const value_type& operator[ ](size_t _pos) const noexcept
	{
		_EXPECTS(_pos < 4, "accessing position out-of-bounds");

		return data[_pos];
	}

	NODISCARD constexpr BasicQuaternion operator-( ) const noexcept
	{
		return BasicQuaternion(-imaginary( ), -w);
	}

	NODISCARD constexpr BasicQuaternion operator*(const BasicQuaternion& _rhs) const noexcept
	{
		return BasicQuaternion(
			BasicVector3<value_type>(
				x * _rhs.w + w * _rhs.x + y * _rhs.z - z * _rhs.y,
				y * _rhs.w + w * _rhs.y + z * _rhs.x - x * _rhs.z,
				z * _rhs.w + w * _rhs.z + x * _rhs.y - y * _rhs.x
			),
			w * _rhs.w - x * _rhs.x - y * _rhs.y - z * _rhs.z
		);
	}

	NODISCARD constexpr BasicQuaternion operator/(value_type _rhs) const noexcept
	{
		_EXPECTS(_rhs != 0, "can't divide by zero");

		return BasicQuaternion(imaginary( ) / _rhs, w / _rhs);
	}

	constexpr BasicQuaternion& operator*=(const BasicQuaternion& _rhs) noexcept
	{
		return *this = *this * _rhs;
	}

	NODISCARD bool operator==(const BasicQuaternion& _rhs) const noexcept
	{
		return x == _rhs.x && y == _rhs.y && z == _rhs.z && w == _rhs.w;
	}

	NODISCARD bool operator!=(const BasicQuaternion& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
};

template <std::integral _Ty>
BasicQuaternion(_Ty, _Ty, _Ty, _Ty) -> BasicQuaternion<double>;

using Quaternion = BasicQuaternion<>;
using FQuaternion = BasicQuaternion<float>;

namespace std_utils
{

template <typename _Ty>
std::ostream& operator<<(std::ostream& _os, const BasicQuaternion<_Ty>& _rhs) noexcept
{
	_os << '(' << _rhs.x << ", "
		<< _rhs.y << ", "
		<< _rhs.z << ", "
		<< _rhs.w << ')';

	return _os;
}

}

template <typename _Ty>
NODISCARD constexpr _Ty dot(const BasicQuaternion<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs) noexcept
{
	return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z + _lhs.w * _rhs.w;
}

template <typename _Ty>
NODISCARD constexpr bool equal(const BasicQuaternion<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs) noexcept
{
	return almost_equal(_lhs.x, _rhs.x)
		&& almost_equal(_lhs.y, _rhs.y)
		&& almost_equal(_lhs.z, _rhs.z)
		&& almost_equal(_lhs.w, _rhs.w);
}

template <typename _Ty>
NODISCARD constexpr bool equal(const BasicQuaternion<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs, uint32_t _ulp) noexcept
{
	return almost_equal(_lhs.x, _rhs.x, _ulp)
		&& almost_equal(_lhs.y, _rhs.y, _ulp)
		&& almost_equal(_lhs.z, _rhs.z, _ulp)
		&& almost_equal(_lhs.w, _rhs.w, _ulp);
}

template <typename _Ty>
NODISCARD constexpr BasicQuaternion<_Ty> lerp(const BasicQuaternion<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs, _Ty _t) noexcept
{
	if (_t == static_cast<_Ty>(0))
		return _lhs;
	else if (_t == static_cast<_Ty>(1))
		return _rhs;

	return BasicQuaternion(
		lerp(_lhs.x, _rhs.x, _t),
		lerp(_lhs.y, _rhs.y, _t),
		lerp(_lhs.z, _rhs.z, _t),
		lerp(_lhs.w, _rhs.w, _t)
	);
}

template <typename _Ty>
NODISCARD _Ty halfTheta(const BasicQuaternion<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs) noexcept
{
	_EXPECTS(_lhs.is_norm( ) && _rhs.is_norm( ), "given vectors should be normalized");

	return std::acos(dot(_lhs, _rhs));
}

template <typename _Ty>
NODISCARD BasicQuaternion<_Ty> slerp(const BasicQuaternion<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs, _Ty _t) noexcept
{
	if (_t == 0)
		return _lhs;
	else if (_t == 1)
		return _rhs;

	_t /= 2;
	_Ty cHalf = dot(_lhs, _rhs),
		theta = acos(cHalf);

	if (theta < 0)
		theta -= theta;
	_Ty s = sin(theta);
	_Ty p = sin((1 - _t) * theta) / s,
		q = sin(_t * theta) / s;

	return BasicQuaternion(
		p * _lhs.x + q * _rhs.x,
		p * _lhs.y + q * _rhs.y,
		p * _lhs.z + q * _rhs.z,
		p * _lhs.w + q * _rhs.w
	);
}

template <typename _Ty>
NODISCARD BasicVector3<_Ty> transformVector(const BasicVector3<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs) noexcept
{
	/*_Ty ix = _q.w * _v.x + _q.y * _v.z - _q.z * _v.y,
		iy = _q.w * _v.y + _q.z * _v.x - _q.x * _v.z,
		iz = _q.w * _v.z + _q.x * _v.y - _q.y * _v.x,
		iw = -(_q.x * _v.x + _q.y * _v.y + _q.z * _v.z);

	return BasicVector3(
		ix * _q.w - iw * _q.x - iy * _q.z + iz * _q.y,
		iy * _q.w - iw * _q.y - iz * _q.x + ix * _q.z,
		iz * _q.w - iw * _q.z - ix * _q.y + iy * _q.x
	);*/

	_EXPECTS(_rhs.is_norm( ), "quartenion should be normalized");
	BasicVector3 t = cross(_rhs.imaginary( ), _lhs) * 2;

	return _lhs + t * _rhs.w + cross(_rhs.imaginary( ), t);
}

// NOTE: review
template <typename _Ty>
NODISCARD BasicVector3<_Ty> reflectVector(const BasicVector3<_Ty>& _lhs, const BasicQuaternion<_Ty>& _rhs) noexcept
{
	BasicQuaternion r = _rhs * _rhs * _lhs;

	return r.imaginary( );
}
