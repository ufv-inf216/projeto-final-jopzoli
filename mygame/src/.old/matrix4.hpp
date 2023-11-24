/*
*
*/

#pragma once

#include "math.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include <array>
#include <iomanip>
#include <iostream>

template <std::floating_point _Ty = double>
class BasicMatrix4
{
public:
	using value_type = _Ty;
	using data_type = std::array<_Ty, 16>;
	using row_type = BasicVector4<_Ty>;

	union /* anonymous */
	{
		data_type data;
		struct { row_type a0; row_type a1; row_type a2; row_type a3; };
	};

	NODISCARD static consteval BasicMatrix4 identity( ) noexcept
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static consteval BasicMatrix4 null( ) noexcept
	{
		return { };
	}

	NODISCARD static BasicMatrix4 makeLookAt(const BasicVector3<value_type>& _eye, const BasicVector3<value_type>& _target, const BasicVector3<value_type>& _up) noexcept
	{
		auto f = _target - _eye;
		f.normalize( );
		auto s = cross(f, _up);
		s.normalize( );
		auto u = cross(s, f);

		return {
			s.x, u.x, f.x, 0,
			s.y, u.y, f.y, 0,
			s.z, u.z, f.z, 0,
			dot(s, -_eye), dot(u, -_eye), dot(f, -_eye), 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeOrthographicProj(const BasicVector2<value_type>& _p, value_type _near, value_type _far) noexcept
	{
		value_type k = _near - _far;

		return {
			2 / _p.x, 0, 0, 0,
			0, 2 / _p.y, 0, 0,
			0, 0, 2 / k, 0,
			0, 0, (_near + _far) / k, 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeOrthographicProj(const BasicVector2<value_type>& _lb, const BasicVector2<value_type>& _rt, value_type _near, value_type _far) noexcept
	{
		value_type dx = _rt.x - _lb.x,
			dy = _rt.y - _rt.y,
			k = _near - _far;

		return {
			2 / dx, 0, 0, 0,
			0, 2 / dy, 0, 0,
			0, 0, 2 / k, 0,
			-(_rt.x + _lb.x) / dx, -(_rt.y + _lb.y) / dy, (_near + _far) / k, 1
		};
	}

	// NOTE: infinite far
	NODISCARD static constexpr BasicMatrix4 makePerspectiveProj(const BasicVector2<value_type>& _p, value_type _near) noexcept
	{
		return {
			2 * _near / _p.x, 0, 0, 0,
			0, 2 * _near / _p.y, 0, 0,
			0, 0, -1, -1,
			0, 0, -2 * _near, 0
		};
	}

	NODISCARD static constexpr BasicMatrix4 makePerspectiveProj(const BasicVector2<value_type>& _p, value_type _near, value_type _far) noexcept
	{
		value_type k = _near - _far;

		return {
			2 * _near / _p.x, 0, 0, 0,
			0, 2 * _near / _p.y, 0, 0,
			0, 0, (_near + _far) / k, -1,
			0, 0, 2 * _near * _far / k, 0
		};
	}

	// NOTE: infinite far
	NODISCARD static BasicMatrix4 makePerspectiveProj(value_type _fov, value_type _aspectRatio, value_type _near) noexcept
	{
		value_type t = 1 / tan(_fov / 2);

		return {
			t, 0, 0, 0,
			0, _aspectRatio * t, 0, 0,
			0, 0, -1, -1,
			0, 0, -2 * _near, 0
		};
	}

	NODISCARD static constexpr BasicMatrix4 makePerspectiveProj(value_type _fov, value_type _aspectRatio, value_type _near, value_type _far) noexcept
	{
		value_type t = 1 / tan(_fov / 2),
			k = _near - _far;

		return {
			t, 0, 0, 0,
			0, _aspectRatio * t, 0, 0,
			0, 0, (_near + _far) / k, -1,
			0, 0, 2 * _near * _far / k, 0
		};
	}

	// NOTE: infinite far
	NODISCARD static constexpr BasicMatrix4 makePerspectiveProj(const BasicVector2<value_type>& _lb, const BasicVector2<value_type>& _rt, value_type _near) noexcept
	{
		value_type dx = _rt.x - _lb.x,
			dy = _rt.y - _rt.y;

		return {
			2 * _near / dx, 0, 0, 0,
			0, 2 * _near / dy, 0, 0,
			(_rt.x + _lb.x) / dx, (_rt.y + _lb.y) / dy, -1, -1,
			0, 0, -2 * _near, 0
		};
	}

	NODISCARD static constexpr BasicMatrix4 makePerspectiveProj(const BasicVector2<value_type>& _lb, const BasicVector2<value_type>& _rt, value_type _near, value_type _far) noexcept
	{
		value_type dx = _rt.x - _lb.x,
			dy = _rt.y - _rt.y,
			k = _near - _far;

		return {
			2 * _near / dx, 0, 0, 0,
			0, 2 * _near / dy, 0, 0,
			(_rt.x + _lb.x) / dx, (_rt.y + _lb.y) / dy, (_near + _far) / k, -1,
			0, 0, 2 * _near * _far / k, 0
		};
	}

	NODISCARD static _CONSTEXPRG BasicMatrix4 makeReflection(const BasicVector3<value_type>& _n) noexcept
	{
		_EXPECTS(_n.is_norm( ), "vector should be normalized");

		return {
			1 - 2 * _n.x * _n.x, -2 * _n.x * _n.y, -2 * _n.x * _n.z, 0,
			-2 * _n.x * _n.y, 1 - 2 * _n.y * _n.y, -2 * _n.y * _n.z, 0,
			-2 * _n.x * _n.z, -2 * _n.y * _n.z, 1 - 2 * _n.z * _n.z, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static BasicMatrix4 makeRotationI(value_type _theta) noexcept
	{
		value_type s = sin(_theta), c = cos(_theta);

		return {
			1, 0, 0, 0,
			0, c, s, 0,
			0, -s, c, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static BasicMatrix4 makeRotationJ(value_type _theta) noexcept
	{
		value_type s = sin(_theta), c = cos(_theta);

		return {
			c, 0, -s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static BasicMatrix4 makeRotationK(value_type _theta) noexcept
	{
		value_type s = sin(_theta), c = cos(_theta);

		return {
			c, s, 0, 0,
			-s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeScale(value_type _xS, value_type _yS, value_type _zS) noexcept
	{
		return {
			_xS, 0, 0, 0,
			0, _yS, 0, 0,
			0, 0, _zS, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeShearingXY(const BasicVector2<value_type>& _s) noexcept
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			_s.x, _s.y, 1, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeShearingXZ(const BasicVector2<value_type>& _s) noexcept
	{
		return {
			1, 0, 0, 0,
			_s.x, 1, _s.y, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeShearingYZ(const BasicVector2<value_type>& _s) noexcept
	{
		return {
			1, _s.x, _s.y, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix4 makeTranslation(value_type _xT, value_type _yT, value_type _zT) noexcept
	{
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			_xT, _yT, _zT, 1
		};
	}

#pragma warning(disable: WARN_VARINST_)
	constexpr BasicMatrix4(const data_type& _data) noexcept
		: data{ _data }
	{ }

	constexpr BasicMatrix4(const row_type& _a0, const row_type& _a1, const row_type& _a2, const row_type& _a3) noexcept
		: a0{ _a0 },
		a1{ _a1 },
		a2{ _a2 },
		a3{ _a3 }
	{ }
#pragma warning(default: WARN_VARINST_)

	constexpr BasicMatrix4& set(const data_type& _val) const noexcept
	{
		data = _val;

		return *this;
	}

	NODISCARD constexpr BasicMatrix4 transposed( ) const noexcept
	{
		return {
			data[0], data[4], data[8], data[12],
			data[1], data[5], data[9], data[13],
			data[2], data[6], data[10], data[14],
			data[3], data[7], data[11], data[15]
		};
	}

	NODISCARD constexpr BasicMatrix4& transpose( ) noexcept
	{
		value_type aux;
		aux = data[1];
		data[1] = data[4];
		data[4] = aux;
		aux = data[2];
		data[2] = data[8];
		data[8] = aux;
		aux = data[6];
		data[6] = data[9];
		data[9] = aux;
		aux = data[3];
		data[3] = data[12];
		data[12] = aux;
		aux = data[7];
		data[7] = data[13];
		data[13] = aux;
		aux = data[11];
		data[11] = data[14];
		data[14] = aux;

		return *this;
	}

	NODISCARD constexpr row_type& operator[](size_t _pos) noexcept
	{
		_EXPECTS(_pos >= 0 && _pos < 4, "accessing invalid matrix line");
		if (_pos == 0)
			return a0;
		if (_pos == 1)
			return a1;
		if (_pos == 2)
			return a2;
		if (_pos == 3)
			return a3;
	}

	NODISCARD constexpr const value_type& operator[](size_t _pos) const noexcept
	{
		_EXPECTS(_pos >= 0 && _pos < 4, "accessing invalid matrix line");
		if (_pos == 0)
			return a0;
		if (_pos == 1)
			return a1;
		if (_pos == 2)
			return a2;
		if (_pos == 3)
			return a3;
	}

	NODISCARD constexpr BasicMatrix4 operator+(const BasicMatrix4& _rhs) const noexcept
	{
		return {
			data[0] + _rhs.data[0], data[1] + _rhs.data[1], data[2] + _rhs.data[2], data[3] + _rhs.data[3],
			data[4] + _rhs.data[4], data[5] + _rhs.data[5], data[6] + _rhs.data[6], data[7] + _rhs.data[7],
			data[8] + _rhs.data[8], data[9] + _rhs.data[9], data[10] + _rhs.data[10], data[11] + _rhs.data[11],
			data[12] + _rhs.data[12], data[13] + _rhs.data[13], data[14] + _rhs.data[14], data[15] + _rhs.data[15]
		};
	}

	constexpr BasicMatrix4& operator+=(const BasicMatrix4& _rhs) noexcept
	{
		data[0] += _rhs.data[0];
		data[1] += _rhs.data[1];
		data[2] += _rhs.data[2];
		data[3] += _rhs.data[3];
		data[4] += _rhs.data[4];
		data[5] += _rhs.data[5];
		data[6] += _rhs.data[6];
		data[7] += _rhs.data[7];
		data[8] += _rhs.data[8];
		data[9] += _rhs.data[9];
		data[10] += _rhs.data[10];
		data[11] += _rhs.data[11];
		data[12] += _rhs.data[12];
		data[13] += _rhs.data[13];
		data[14] += _rhs.data[14];
		data[15] += _rhs.data[15];

		return *this;
	}

	NODISCARD constexpr BasicMatrix4 operator-(const BasicMatrix4& _rhs) const noexcept
	{
		return {
			data[0] - _rhs.data[0], data[1] - _rhs.data[1], data[2] - _rhs.data[2], data[3] - _rhs.data[3],
			data[4] - _rhs.data[4], data[5] - _rhs.data[5], data[6] - _rhs.data[6], data[7] - _rhs.data[7],
			data[8] - _rhs.data[8], data[9] - _rhs.data[9], data[10] - _rhs.data[10], data[11] - _rhs.data[11],
			data[12] - _rhs.data[12], data[13] - _rhs.data[13], data[14] - _rhs.data[14], data[15] - _rhs.data[15]
		};
	}

	constexpr BasicMatrix4& operator-=(const BasicMatrix4& _rhs) noexcept
	{
		data[0] -= _rhs.data[0];
		data[1] -= _rhs.data[1];
		data[2] -= _rhs.data[2];
		data[3] -= _rhs.data[3];
		data[4] -= _rhs.data[4];
		data[5] -= _rhs.data[5];
		data[6] -= _rhs.data[6];
		data[7] -= _rhs.data[7];
		data[8] -= _rhs.data[8];
		data[9] -= _rhs.data[9];
		data[10] -= _rhs.data[10];
		data[11] -= _rhs.data[11];
		data[12] -= _rhs.data[12];
		data[13] -= _rhs.data[13];
		data[14] -= _rhs.data[14];
		data[15] -= _rhs.data[15];

		return *this;
	}

	NODISCARD constexpr BasicMatrix4 operator*(const BasicMatrix4& _rhs) const noexcept
	{
		return {
			data[0] * _rhs.data[0] + data[1] * _rhs.data[4] + data[2] * _rhs.data[8] + data[3] * _rhs.data[12],
			data[0] * _rhs.data[1] + data[1] * _rhs.data[5] + data[2] * _rhs.data[9] + data[3] * _rhs.data[13],
			data[0] * _rhs.data[2] + data[1] * _rhs.data[6] + data[2] * _rhs.data[10] + data[3] * _rhs.data[14],
			data[0] * _rhs.data[3] + data[1] * _rhs.data[7] + data[2] * _rhs.data[11] + data[3] * _rhs.data[15],
			data[4] * _rhs.data[0] + data[5] * _rhs.data[4] + data[6] * _rhs.data[8] + data[7] * _rhs.data[12],
			data[4] * _rhs.data[1] + data[5] * _rhs.data[5] + data[6] * _rhs.data[9] + data[7] * _rhs.data[13],
			data[4] * _rhs.data[2] + data[5] * _rhs.data[6] + data[6] * _rhs.data[10] + data[7] * _rhs.data[14],
			data[4] * _rhs.data[3] + data[5] * _rhs.data[7] + data[6] * _rhs.data[11] + data[7] * _rhs.data[15],
			data[8] * _rhs.data[0] + data[9] * _rhs.data[4] + data[10] * _rhs.data[8] + data[11] * _rhs.data[12],
			data[8] * _rhs.data[1] + data[9] * _rhs.data[5] + data[10] * _rhs.data[9] + data[11] * _rhs.data[13],
			data[8] * _rhs.data[2] + data[9] * _rhs.data[6] + data[10] * _rhs.data[10] + data[11] * _rhs.data[14],
			data[8] * _rhs.data[3] + data[9] * _rhs.data[7] + data[10] * _rhs.data[11] + data[11] * _rhs.data[15],
			data[12] * _rhs.data[0] + data[13] * _rhs.data[4] + data[14] * _rhs.data[8] + data[15] * _rhs.data[12],
			data[12] * _rhs.data[1] + data[13] * _rhs.data[5] + data[14] * _rhs.data[9] + data[15] * _rhs.data[13],
			data[12] * _rhs.data[2] + data[13] * _rhs.data[6] + data[14] * _rhs.data[10] + data[15] * _rhs.data[14],
			data[12] * _rhs.data[3] + data[13] * _rhs.data[7] + data[14] * _rhs.data[11] + data[15] * _rhs.data[15]
		};
	}

	constexpr BasicMatrix4& operator*=(const BasicMatrix4& _rhs) noexcept
	{
		return *this = *this * _rhs;
	}

	NODISCARD constexpr BasicMatrix4 operator*(value_type _rhs) const noexcept
	{
		return {
			data[0] * _rhs, data[1] * _rhs, data[2] * _rhs, data[3] * _rhs,
			data[4] * _rhs, data[5] * _rhs, data[6] * _rhs, data[7] * _rhs,
			data[8] * _rhs, data[9] * _rhs, data[10] * _rhs, data[11] * _rhs,
			data[12] * _rhs, data[13] * _rhs, data[14] * _rhs, data[15] * _rhs
		};
	}

	constexpr BasicMatrix4& operator*=(value_type _rhs) noexcept
	{
		data[0] *= _rhs;
		data[1] *= _rhs;
		data[2] *= _rhs;
		data[3] *= _rhs;
		data[4] *= _rhs;
		data[5] *= _rhs;
		data[6] *= _rhs;
		data[7] *= _rhs;
		data[8] *= _rhs;
		data[9] *= _rhs;
		data[10] *= _rhs;
		data[11] *= _rhs;
		data[12] *= _rhs;
		data[13] *= _rhs;
		data[14] *= _rhs;
		data[15] *= _rhs;

		return *this;
	}

	NODISCARD constexpr bool operator==(const BasicMatrix4& _rhs) const noexcept
	{
		return data == _rhs.data;
	}

	NODISCARD constexpr bool operator!=(const BasicMatrix4& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
};

template <std::integral _Ty>
BasicMatrix4(const std::array<_Ty, 16>&) -> BasicMatrix4<double>;

using Matrix4 = BasicMatrix4<>;
using FMatrix4 = BasicMatrix4<float>;

namespace std_utils
{

template <typename _Ty>
std::ostream& operator<<(std::ostream& _os, const BasicMatrix4<_Ty>& _rhs) noexcept
{
	constexpr std::streamsize w = 16;
	_os << _rhs[0] << std::setw(w) << _rhs[1] << std::setw(w) << _rhs[2] << std::setw(w) << _rhs[3] << '\n'
		<< _rhs[4] << std::setw(w) << _rhs[5] << std::setw(w) << _rhs[6] << std::setw(w) << _rhs[7] << '\n'
		<< _rhs[8] << std::setw(w) << _rhs[9] << std::setw(w) << _rhs[10] << std::setw(w) << _rhs[11] << '\n'
		<< _rhs[12] << std::setw(w) << _rhs[13] << std::setw(w) << _rhs[14] << std::setw(w) << _rhs[15];

	return _os;
}

}

template <typename _Ty>
BasicMatrix4<_Ty> concatMatrix(BasicMatrix4<_Ty>& _lhs, const BasicMatrix4<_Ty>& _rhs) noexcept
{
	return _lhs * _rhs;
}

template <typename _Ty>
NODISCARD constexpr BasicVector3<_Ty> transformVector(const BasicVector3<_Ty>& _v, const BasicMatrix4<_Ty>& _m) noexcept
{
	return BasicVector3(
		_m[0] * _v.x + _m[4] * _v.y + _m[8] * _v.z + _m[12],
		_m[1] * _v.x + _m[5] * _v.y + _m[9] * _v.z + _m[13],
		_m[2] * _v.x + _m[6] * _v.y + _m[10] * _v.z + _m[14]
	);
}
