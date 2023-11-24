/*
*
*/

#pragma once

#include "math.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include <array>
#include <iomanip>
#include <iostream>

template <std::floating_point _Ty = double>
class BasicMatrix3
{
public:
	using value_type = _Ty;
	using data_type = std::array<_Ty, 9>;
	using row_type = BasicVector3<_Ty>;

	union /* anonymous */
	{
		data_type data;
		struct { row_type a0; row_type a1; row_type a2; };
	};

	NODISCARD static consteval BasicMatrix3 identity( ) noexcept
	{
		return {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};
	}

	NODISCARD static consteval BasicMatrix3 null( ) noexcept
	{
		return { };
	}

	NODISCARD static constexpr BasicMatrix3 makeProjection(const BasicVector2<value_type>& _s) noexcept
	{
		return {
			2 / _s.x, 0, 0,
			0, 2 / _s.y, 0,
			0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix3 makeProjection(const BasicVector2<value_type>& _lb, const BasicVector2<value_type>& _rt) noexcept
	{
		value_type dx = _rt.x - _lb.x,
			dy = _rt.y - _lb.y;

		return {
			2 / dx, 0, 0,
			0, 2 / dy, 0,
			-(_rt.x + _lb.x) / dx, -(_rt.y + _lb.y) / dy, 1
		};
	}

	NODISCARD static _CONSTEXPRG BasicMatrix3 makeReflection(const BasicVector2<value_type>& _n) noexcept
	{
		_EXPECTS(_n.is_norm( ), "vector should be normalized");

		return {
			_n.x * _n.x - _n.y * _n.y, 2 * _n.x * _n.y, 0,
			2 * _n.x * _n.y, _n.y * _n.y - _n.x * _n.x, 0,
			0, 0, 1
		};
	}

	NODISCARD static BasicMatrix3 makeRotation(value_type _theta) noexcept
	{
		value_type s = sin(_theta), c = cos(_theta);

		return {
			c, -s, 0,
			s, c, 0,
			0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix3 makeScale(const BasicVector2<value_type>& _s) noexcept
	{
		return {
			_s.x, 0, 0,
			0, _s.y, 0,
			0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix3 makeShearingX(value_type _xS) noexcept
	{
		return {
			1, _xS, 0,
			0, 1, 0,
			0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix3 makeShearingY(value_type _yS) noexcept
	{
		return {
			1, 0, 0,
			_yS, 1, 0,
			0, 0, 1
		};
	}

	NODISCARD static constexpr BasicMatrix3 makeTranslation(const BasicVector2<value_type>& _t) noexcept
	{
		return {
			1, 0, 0,
			0, 1, 0,
			_t.x, _t.y, 1
		};
	}

	constexpr BasicMatrix3& set(data_type& _val) const noexcept
	{
		data = _val;

		return *this;
	}

	NODISCARD constexpr BasicMatrix3 transposed( ) const noexcept
	{
		return {
			data[0], data[3], data[6],
			data[1], data[4], data[7],
			data[2], data[5], data[8]
		};
	}

	NODISCARD constexpr BasicMatrix3& transpose( ) noexcept
	{
		value_type aux;
		aux = data[1];
		data[1] = data[3];
		data[3] = aux;
		aux = data[2];
		data[2] = data[6];
		data[6] = aux;
		aux = data[5];
		data[5] = data[7];
		data[7] = aux;

		return *this;
	}

	NODISCARD constexpr value_type& operator[](size_t _pos) noexcept
	{
		_EXPECTS(_pos >= 0 && _pos < 3, "accessing invalid matrix row");
		if (_pos == 0)
			return a0;
		if (_pos == 1)
			return a1;
		if (_pos == 2)
			return a2;
	}

	NODISCARD constexpr const value_type& operator[](size_t _pos) const noexcept
	{
		_EXPECTS(_pos >= 0 && _pos < 3, "accessing invalid matrix row");
		if (_pos == 0)
			return a0;
		if (_pos == 1)
			return a1;
		if (_pos == 2)
			return a2;
	}

	NODISCARD constexpr BasicMatrix3 operator+(const BasicMatrix3& _rhs) const noexcept
	{
		return {
			data[0] + _rhs.data[0], data[1] + _rhs.data[1], data[2] + _rhs.data[2],
			data[3] + _rhs.data[3], data[4] + _rhs.data[4], data[5] + _rhs.data[5],
			data[6] + _rhs.data[6], data[7] + _rhs.data[7], data[8] + _rhs.data[8],
		};
	}

	constexpr BasicMatrix3& operator+=(const BasicMatrix3& _rhs) noexcept
	{
		data[0] += _rhs.data[0];
		data[1] += _rhs.data[1];
		data[1] += _rhs.data[1];
		data[2] += _rhs.data[2];
		data[3] += _rhs.data[3];
		data[4] += _rhs.data[4];
		data[5] += _rhs.data[5];
		data[6] += _rhs.data[6];
		data[7] += _rhs.data[7];
		data[8] += _rhs.data[8];

		return *this;
	}

	NODISCARD constexpr BasicMatrix3 operator-(const BasicMatrix3& _rhs) const noexcept
	{
		return {
			data[0] - _rhs.data[0], data[1] - _rhs.data[1], data[2] - _rhs.data[2],
			data[3] - _rhs.data[3], data[4] - _rhs.data[4], data[5] - _rhs.data[5],
			data[6] - _rhs.data[6], data[7] - _rhs.data[7], data[8] - _rhs.data[8],
		};
	}

	constexpr BasicMatrix3& operator-=(const BasicMatrix3& _rhs) noexcept
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

		return *this;
	}

	NODISCARD constexpr BasicMatrix3 operator*(const BasicMatrix3& _rhs) const noexcept
	{
		return {
			data[0] * _rhs.data[0] + data[1] * _rhs.data[3] + data[2] * _rhs.data[6],
			data[0] * _rhs.data[1] + data[1] * _rhs.data[4] + data[2] * _rhs.data[7],
			data[0] * _rhs.data[2] + data[1] * _rhs.data[5] + data[2] * _rhs.data[8],
			data[3] * _rhs.data[0] + data[4] * _rhs.data[4] + data[5] * _rhs.data[6],
			data[3] * _rhs.data[1] + data[4] * _rhs.data[5] + data[5] * _rhs.data[7],
			data[3] * _rhs.data[2] + data[4] * _rhs.data[6] + data[5] * _rhs.data[8],
			data[6] * _rhs.data[0] + data[7] * _rhs.data[4] + data[8] * _rhs.data[6],
			data[6] * _rhs.data[1] + data[7] * _rhs.data[5] + data[8] * _rhs.data[7],
			data[6] * _rhs.data[2] + data[7] * _rhs.data[6] + data[8] * _rhs.data[8]
		};
	}

	constexpr BasicMatrix3& operator*=(const BasicMatrix3& _rhs) noexcept
	{
		return *this = *this * _rhs;
	}

	NODISCARD constexpr BasicMatrix3 operator*(value_type _rhs) const noexcept
	{
		return {
			data[0] * _rhs, data[1] * _rhs, data[2] * _rhs,
			data[3] * _rhs, data[4] * _rhs, data[5] * _rhs,
			data[6] * _rhs, data[7] * _rhs, data[8] * _rhs
		};
	}

	constexpr BasicMatrix3& operator*=(value_type _rhs) noexcept
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

		return *this;
	}

	NODISCARD constexpr bool operator==(const BasicMatrix3& _rhs) const noexcept
	{
		return data == _rhs.data;
	}

	NODISCARD constexpr bool operator!=(const BasicMatrix3& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
};

template <std::integral _Ty>
BasicMatrix3(std::array<_Ty, 9>) -> BasicMatrix3<double>;

using Matrix3 = BasicMatrix3<>;
using FMatrix3 = BasicMatrix3<float>;

namespace std_utils
{

template <typename _Ty>
std::ostream& operator<<(std::ostream& _os, const BasicMatrix3<_Ty>& _rhs) noexcept
{
	constexpr std::streamsize w = 16;
	_os << _rhs[0] << std::setw(w) << _rhs[1] << std::setw(w) << _rhs[2] << '\n'
		<< _rhs[3] << std::setw(w) << _rhs[4] << std::setw(w) << _rhs[5] << '\n'
		<< _rhs[6] << std::setw(w) << _rhs[7] << std::setw(w) << _rhs[8];

	return _os;
}

}

template <typename _Ty>
BasicMatrix3<_Ty> concatMatrix(BasicMatrix3<_Ty>& _lhs, const BasicMatrix3<_Ty>& _rhs) noexcept
{
	return _lhs * _rhs;
}

template <typename _Ty>
NODISCARD constexpr BasicVector2<_Ty> transformVector(const BasicVector2<_Ty>& _v, const BasicMatrix3<_Ty>& _m) noexcept
{
	return BasicVector2(
		_m[0] * _v.x + _m[3] * _v.y + _m[6],
		_m[1] * _v.x + _m[4] * _v.y + _m[7]
	);
}
