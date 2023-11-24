/*
*
*/

#pragma once

#include "asserts.hpp"
#include <functional>
#include <utility>

namespace detail
{
template <typename, typename = void>
struct is_comparamble_to_nullptr : std::false_type { };

template <typename _Ty>
struct is_comparamble_to_nullptr<_Ty,
	std::enable_if_t<std::is_convertible_v<decltype(std::declval<_Ty>( ) == nullptr), bool>>>
	: std::true_type
{ };

template <typename _Ty>
using value_or_reference_return_t = std::conditional_t<
	sizeof(_Ty) < 2 * sizeof(void*) && std::is_trivially_copy_constructible_v<_Ty>,
	_Ty,
	_Ty&>;
}

template <typename...> struct unique_parameters;
template <typename _FirstTy, typename _SecondTy>
struct unique_parameters<_FirstTy, _SecondTy>
{
	static constexpr bool value = !std::is_same_v<_FirstTy, _SecondTy>;
};

template <typename _FirstTy, typename _SecondTy, typename... _Remn>
struct unique_parameters<_FirstTy, _SecondTy, _Remn...>
{
	static constexpr bool value = !std::is_same_v<_FirstTy, _SecondTy>&& unique_parameters<_SecondTy, _Remn...>::value;
};

template <typename _Ty>
struct final_action
{
public:
	using value_type = _Ty;

	constexpr explicit final_action(const value_type& _fn) noexcept
		: m_fn(_fn)
	{ }

	constexpr explicit final_action(value_type&& _fn) noexcept
		: m_fn(std::move(_fn))
	{ }

	constexpr final_action(final_action&& _other) noexcept
		: m_fn(std::move(_other.m_fn)),
		m_invoke(std::exchange(_other.m_invoke, false))
	{ }

	constexpr ~final_action( ) noexcept
	{
		if (m_invoke) _invoke( );
	}

	final_action(const final_action&) = delete;
	final_action& operator=(const final_action&) = delete;
	final_action& operator=(final_action&&) = delete;

private:
	value_type m_fn;
	bool m_invoke{ true };

	constexpr void _invoke( ) noexcept(noexcept(std::invoke(m_fn)))
	{
		std::invoke(m_fn);
	}
};

template <typename _Ty>
_NODISCARD constexpr auto finally_do(_Ty&& _fn) noexcept
{
	return final_action<std::decay_t<_Ty>>{std::forward<_Ty>(_fn)};
}
#define FINALLY_DECL(fn)	auto fnl_##fn = finally_do(fn)

struct zero_then_vaargs_tag { };
struct one_then_vaargs_tag { };

template <typename _Ty1, typename _Ty2, bool = std::is_empty_v<_Ty1> && !std::is_final_v<_Ty1>>
struct compressed_pair final
	: private _Ty1
{
	using base_type = _Ty1;

	_Ty2 val2;

	template <typename... _Args>
	constexpr explicit compressed_pair(zero_then_vaargs_tag, _Args&&... _args)
		noexcept(std::conjunction_v<std::is_nothrow_default_constructible<_Ty1>,
			std::is_nothrow_constructible<_Ty2, _Args...>>)
		: _Ty1{ },
		val2{ std::forward<_Args>(_args)... }
	{ }

	template <typename _Arg1, typename... _Args2>
	constexpr compressed_pair(one_then_vaargs_tag, _Arg1&& _arg1, _Args2&&... _args2)
		noexcept(std::conjunction_v<std::is_nothrow_constructible<_Ty1, _Arg1>,
			std::is_nothrow_constructible<_Ty2, _Args2...>>)
		: _Ty1{ std::forward<_Arg1>(_arg1) },
		val2{ std::forward<_Args2>(_args2)... }
	{ }

	constexpr _Ty1& first( ) noexcept
	{
		return *this;
	}

	constexpr const _Ty1& first( ) const noexcept
	{
		return *this;
	}
};

template <typename _Ty1, typename _Ty2>
struct compressed_pair<_Ty1, _Ty2, false> final
{
	using base_type = _Ty1;

	_Ty1 val1;
	_Ty2 val2;

	template <typename... _Args>
	constexpr explicit compressed_pair(zero_then_vaargs_tag, _Args&&... _args)
		noexcept(std::conjunction_v<std::is_nothrow_default_constructible<_Ty1>,
			std::is_nothrow_constructible<_Ty2, _Args...>>)
		: val1{ },
		val2{ std::forward<_Args>(_args)... }
	{ }

	template <typename _Arg1, typename... _Args2>
	constexpr compressed_pair(one_then_vaargs_tag, _Arg1&& _arg1, _Args2&&... _args2)
		noexcept(std::conjunction_v<std::is_nothrow_constructible<_Ty1, _Arg1>,
			std::is_nothrow_constructible<_Ty2, _Args2...>>)
		: val1{ std::forward<_Arg1>(_arg1) },
		val2{ std::forward<_Args2>(_args2)... }
	{ }

	constexpr _Ty1& first( ) noexcept
	{
		return val1;
	}

	constexpr const _Ty1& first( ) const noexcept
	{
		return val1;
	}
};

template <typename _Ty>
struct not_null
{
	static_assert(detail::is_comparamble_to_nullptr<typename std::add_pointer<_Ty>::type>::value,
		"given type can't be compared to nullptr");
public:
	using value_type = _Ty;
	using pointer_type = std::add_pointer_t<_Ty>;

	template <typename _OtherTy, typename = std::enable_if_t<
		std::is_convertible_v<_OtherTy, value_type>>>
	constexpr not_null(_OtherTy&& _ptr) noexcept(
		std::is_nothrow_move_constructible_v<_OtherTy>
		)
		: m_ptr{ std::forward<_OtherTy>(_ptr) }
	{
		_EXPECTS(m_ptr != nullptr, "given pointer can't be nullptr");
	}

	template <typename = std::enable_if_t<!std::is_same_v<std::nullptr_t, pointer_type>>>
	constexpr not_null(pointer_type _ptr) noexcept(
		std::is_nothrow_move_constructible_v<pointer_type>
		)
		: m_ptr{ std::move(_ptr) }
	{
		_EXPECTS(m_ptr != nullptr, "given pointer can't be nullptr");
	}

	template <typename _OtherTy, typename = std::enable_if_t<std::is_convertible_v<_OtherTy, value_type>>>
	constexpr not_null(const not_null<_OtherTy>& _other) noexcept(
		std::is_nothrow_move_constructible_v<value_type>
		)
		: not_null(_other.get( ))
	{ }

	not_null(const not_null&) = default;
	not_null& operator=(const not_null&) = default;
	not_null(std::nullptr_t) = delete;
	not_null& operator=(std::nullptr_t) = delete;

	_NODISCARD constexpr const detail::value_or_reference_return_t<pointer_type> get( ) const noexcept(
		noexcept(detail::value_or_reference_return_t<pointer_type>{std::declval<pointer_type&>( )})
		)
	{
		return m_ptr;
	}

	constexpr decltype(auto) operator->( ) const
	{
		return get( );
	}

	constexpr decltype(auto) operator*( ) const
	{
		return *get( );
	}

	not_null& operator++( ) = delete;
	not_null& operator--( ) = delete;
	not_null operator++(int) = delete;
	not_null operator--(int) = delete;
	not_null& operator+=(std::ptrdiff_t) = delete;
	not_null& operator-=(std::ptrdiff_t) = delete;
	void operator[](std::ptrdiff_t) const = delete;

private:
	pointer_type m_ptr;
};

template <typename _Ty>
not_null(_Ty*) -> not_null<_Ty>;

template <typename _Ty, typename... _Args>
NODISCARD constexpr auto makeNotNull(_Args&&... _args) noexcept
{
	return not_null<_Ty>(new _Ty(std::forward<_Args>(_args)...));
}

template <class T, class U>
_NODISCARD auto operator==(const not_null<T>& lhs,
	const not_null<U>& rhs) noexcept(noexcept(lhs.get( ) == rhs.get( )))
	-> decltype(lhs.get( ) == rhs.get( ))
{
	return lhs.get( ) == rhs.get( );
}

template <typename _Ty1, typename _Ty2>
	requires std::three_way_comparable_with<
		typename not_null<_Ty1>::value_type,
			typename not_null<_Ty2>::value_type>
NODISCARD constexpr std::compare_three_way_result_t<
	typename not_null<_Ty1>::value_type,
	typename not_null<_Ty2>::value_type>
	operator<=>(const not_null<_Ty1>& _lhs, const not_null<_Ty2>& _rhs) noexcept
{
	return _lhs.get( ) <=> _rhs.get( );
}
