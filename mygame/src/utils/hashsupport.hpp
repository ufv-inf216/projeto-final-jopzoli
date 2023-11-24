/*
*
*/

#pragma once

#include "type.hpp"

#define _HASH_FNV_OFFSET_BASIS_	14695981039346656037ULL
#define _HASH_FNV_PRIME_		1099511628211ULL

typedef uint64_t hash_t;

template <typename> NODISCARD consteval hash_t static_hash( ) noexcept { }
template <typename _Ty> NODISCARD consteval hash_t static_hash(const _Ty&) noexcept;
template <typename _Ty, size_t _N>
NODISCARD consteval hash_t static_hash(const _Ty(&)[_N]) noexcept;

template <typename _Ty>
constexpr bool is_static_hashable_v = std::is_integral_v<_Ty>
|| std::is_enum_v<_Ty> || std::is_pointer_v<_Ty>;

template <typename _Ty>
concept ExplicitlyHashSpecialized = requires (const _Ty & __val)
{
	{ static_hash<_Ty>( ) } -> std::same_as<hash_t>;
	{ static_hash(__val) } -> std::same_as<hash_t>;
};

namespace detail
{
template <typename _Ty>
struct bytes_unwrapped
{
	static_assert(std::is_convertible_v<_Ty, byte_t>,
		"given type can't be directly converted to bytes");
	using value_type = _Ty;

	static constexpr size_t _N = sizeof(_Ty);

	byte_t bytes[_N];

	constexpr explicit bytes_unwrapped(const _Ty& _val) noexcept
	{
		for (size_t i = 0; i < _N; i++)
			bytes[i] = static_cast<byte_t>((_val >> (8 * i)) & 0xff);
	}
};

template <typename _Ty, size_t _N>
struct bytes_unwrapped<_Ty[_N]>
{
	static_assert(std::is_convertible_v<_Ty, byte_t>,
		"given type can't be directly converted to bytes");
	using value_type = _Ty[_N];

	static constexpr size_t _Nx = sizeof(_Ty[_N]);

	byte_t bytes[_Nx];

	constexpr explicit bytes_unwrapped(const _Ty(&_val)[_N]) noexcept
	{
		size_t size = sizeof(_Ty);
		for (size_t i = 0; i < _N; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				bytes[i * size + j] = static_cast<byte_t>((_val[i] >> (8 * j)) & 0xff);
			}
		}
	}
};

template <size_t _Idx>
NODISCARD consteval hash_t staticFVN1AAccu(
	const byte_t* _first,
	hash_t _val) noexcept
{
	return (staticFVN1AAccu<_Idx - 1>(_first, _val)
		^ static_cast<hash_t>(_first[_Idx])) * _HASH_FNV_PRIME_;
}

template <>
NODISCARD consteval hash_t staticFVN1AAccu<0>(
	const byte_t* _first,
	hash_t _val) noexcept
{
	return (_val ^ static_cast<hash_t>(_first[0])) * _HASH_FNV_PRIME_;
}
}

NODISCARD consteval hash_t staticHashRep(const byte_t(&_val)[1]) noexcept
{
	return detail::staticFVN1AAccu<0>(_val, _HASH_FNV_OFFSET_BASIS_);
}

template <size_t _N>
NODISCARD consteval hash_t staticHashRep(const byte_t(&_val)[_N]) noexcept
{
	return detail::staticFVN1AAccu<_N - 1>(_val, _HASH_FNV_OFFSET_BASIS_);
}

template <typename _Ty>
NODISCARD consteval hash_t static_hash(const _Ty& _val) noexcept
{
	static_assert(is_static_hashable_v<_Ty>, "given type can't be directly converted to byte");

	return staticHashRep({ static_cast<const byte_t>(_val) });
}

template <typename _Ty, size_t _N>
NODISCARD consteval hash_t static_hash(const _Ty(&_val)[_N]) noexcept
{
	static_assert(is_static_hashable_v<_Ty>, "given type can't be directly converted to byte");
	auto uwr = detail::bytes_unwrapped<_Ty[_N]>{ _val };

	return staticHashRep(uwr.bytes);
}
