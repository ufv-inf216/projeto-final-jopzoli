/*
*
*/

#pragma once

#include "memory/memory.hpp"
#include "utils/rttisupport.hpp"

#define OBJECT_STATICTII_IMPL(x, ...)										\
template <> NODISCARD consteval hash_t static_hash<x>( ) noexcept {			\
return static_hash(GLUE(GLUE("class ", STRINGFY(x)), STRINGFY(__VA_ARGS__)));\
}																			\
template <> NODISCARD consteval hash_t static_hash<x>(const x&) noexcept {	\
return static_hash<x>();													\
}

#define OBJECT_()		OBJECT_STATICTII_IMPL(Object) RTTI_REGISTER_TYPE(Object)
#define OBJECT_IMPL_()	RTTI_POLYMORPHIC_IMPL()

#define OBJIMPL()		RTTI_POLYMORPHIC_IMPL()
#define OBJDECL(x, ...)	OBJECT_STATICTII_IMPL(x, __VA_ARGS__) RTTI_REGISTER_TYPE(x, __VA_ARGS__)

class Object;

template <typename _Ty>
constexpr bool is_object_v = std::is_base_of_v<Object, _Ty>;

template <typename _Ty>
	requires is_object_v<std::remove_pointer_t<_Ty>>
struct object_ptr
{
public:
	using value_type = _Ty;
	using pointer_type = std::add_pointer_t<_Ty>;

	constexpr object_ptr( ) noexcept = default;

	constexpr object_ptr(std::nullptr_t) noexcept { }

	template <typename _OtherTy, typename =
		std::enable_if_t<std::is_convertible_v<_OtherTy, pointer_type>>>
	constexpr object_ptr(_OtherTy _val) noexcept
		: m_ptr{ _val }
	{ }

	template <typename _OtherTy, typename =
		std::enable_if_t<std::is_convertible_v<_OtherTy*, pointer_type>>>
	constexpr object_ptr(const object_ptr<_OtherTy>& _other) noexcept
		: m_ptr{ _other.get( ) }
	{ }

	template <typename _OtherTy, typename =
		std::enable_if_t<std::is_convertible_v<_OtherTy, pointer_type>>>
	constexpr object_ptr& operator=(_OtherTy _val) noexcept
	{
		m_ptr = _val;

		return *this;
	}

	constexpr object_ptr& operator=(std::nullptr_t) noexcept
	{
		return *this = object_ptr( );
	}

	template <typename _OtherTy>
	constexpr object_ptr& operator=(const object_ptr<_OtherTy>& _other) noexcept
	{
		return *this = object_ptr<_OtherTy>(_other);
	}

	template <typename _OtherTy,
		std::enable_if_t<std::is_convertible_v<_OtherTy*, pointer_type>>>
	constexpr object_ptr& operator=(object_ptr<_OtherTy>&& _other) noexcept
	{
		return *this = object_ptr<_OtherTy>(_other);
	}

	NODISCARD constexpr pointer_type get( ) const noexcept
	{
		return m_ptr;
	}

	template <typename _OtherTy>
	constexpr void swap(object_ptr& _other) noexcept
	{
		std::swap(m_ptr, _other.m_ptr);
	}

	NODISCARD constexpr explicit operator bool( ) const noexcept
	{
		return static_cast<bool>(m_ptr);
	}

	NODISCARD constexpr std::add_lvalue_reference_t<_Ty> operator*( ) const noexcept
	{
		return *m_ptr;
	}

	NODISCARD constexpr pointer_type operator->( ) const noexcept
	{
		return m_ptr;
	}

private:
	pointer_type m_ptr{ };
};

template <typename _Ty>
object_ptr(_Ty*) -> object_ptr<_Ty>;

template <typename _Ty, typename... _Args>
NODISCARD constexpr object_ptr<_Ty> makeObjectPtr(_Args&&... _args) noexcept
{
	return object_ptr<_Ty>(new _Ty(std::forward<_Args>(_args)...));
}

template <typename _Ty1, typename _Ty2>
NODISCARD constexpr bool operator==(const object_ptr<_Ty1>& _lhs, const object_ptr<_Ty2>& _rhs) noexcept
{
	return _lhs.get( ) == _rhs.get( );
}

template <typename _Ty>
NODISCARD constexpr bool operator==(const object_ptr<_Ty>& _lhs, std::nullptr_t) noexcept
{
	return !_lhs.get( );
}

template <typename _Ty>
NODISCARD constexpr bool operator==(std::nullptr_t, const object_ptr<_Ty>& _rhs) noexcept
{
	return !_rhs.get( );
}

template <typename _Ty1, typename _Ty2>
	requires std::three_way_comparable_with<
		typename object_ptr<_Ty1>::pointer_type,
			typename object_ptr<_Ty2>::pointer_type>
NODISCARD constexpr std::compare_three_way_result_t<
	typename object_ptr<_Ty1>::pointer_type,
	typename object_ptr<_Ty2>::pointer_type>
	operator<=>(const object_ptr<_Ty1>& _lhs, const object_ptr<_Ty2>& _rhs) noexcept
{
	return _lhs.get( ) <=> _rhs.get( );
}

template <typename _Ty>
	requires std::three_way_comparable<typename object_ptr<_Ty>::pointer_type>
NODISCARD constexpr std::compare_three_way_result_t<
	typename object_ptr<_Ty>::pointer_type>
	operator<=>(const object_ptr<_Ty>& _lhs, std::nullptr_t) noexcept
{
	return _lhs.get( ) <=> static_cast<typename object_ptr<_Ty>::pointer_type>(nullptr_t);
}

class Object
{
public:
	OBJECT_IMPL_( );

	Object() noexcept;

	NODISCARD Object* owner( ) noexcept
	{
		return thisOwner.get( );
	}

	NODISCARDRAWPTR static void* operator new(std::size_t _count);

	NODISCARDRAWPTR static void* operator new(std::size_t _count, size_t _align);

	static void operator delete(void* _block, size_t _count);

	static void operator delete(void* _block, size_t _count, size_t _align);

protected:
	object_ptr<Object> thisOwner{ };
};
OBJECT_( );
