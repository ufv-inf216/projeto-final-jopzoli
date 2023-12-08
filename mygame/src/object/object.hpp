/*
*
*/

#pragma once

#include "memory/memory.hpp"
#include "memory/refcounter.hpp"
#include "utils/rttisupport.hpp"
#include <future>
#include <set>
std::shared_ptr<int>;
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

namespace detail
{
template <typename, typename = void>
struct has_rtti_cast_2 : std::false_type { };

template <typename _Ty>
struct has_rtti_cast_2<_Ty, std::void_t<
	decltype(std::declval<_Ty&>( ).rttiCast2(std::declval<typeid_t>( )))>>
	: std::true_type { };

template <typename, typename = void>
struct has_rttype_info_proxy : std::false_type { };

template <typename _Ty>
struct has_rttype_info_proxy<_Ty, std::void_t<
	decltype(rttype_info_proxy<_Ty>::get( ))>>
	: std::true_type { };
}

template <typename _Ty>
constexpr bool is_object_v = std::is_base_of_v<Object, _Ty>
&& detail::has_rtti_cast_2<_Ty>::value
&& detail::has_rttype_info_proxy<_Ty>::value;

template <typename _Ty>
struct object_ptr
	: ptr_rc_base<_Ty>
{
	static_assert(is_object_v<std::remove_cvref_t<std::remove_pointer_t<_Ty>>>,
		"given type is not an object");

	using _base_type = ptr_rc_base<_Ty>;
public:
	using value_type = _Ty;

	constexpr object_ptr( ) noexcept = default;

	constexpr object_ptr(std::nullptr_t) noexcept { }

	template <typename _OtherTy, typename =
		std::enable_if_t<std::is_convertible_v<_OtherTy, value_type>>>
	explicit object_ptr(_OtherTy* _val) noexcept
	{
		this->ptr = _val;
		this->refCounter = new detail::ref_counter<value_type>{ _val };
	}

	object_ptr(const object_ptr& _other) noexcept
	{
		this->copyConstructFrom(_other);
	}

	template <typename _OtherTy, typename =
		std::enable_if_t<std::is_convertible_v<_OtherTy, value_type>>>
	object_ptr(const object_ptr<_OtherTy>& _other) noexcept
	{
		this->copyConstructFrom(_other);
	}

	object_ptr(object_ptr&& _other) noexcept
	{
		this->moveConstructFrom(std::move(_other));
	}

	template <typename _OtherTy, typename =
		std::enable_if_t<std::is_convertible_v<_OtherTy, value_type>>>
	object_ptr(object_ptr<_OtherTy>&& _other) noexcept
	{
		this->moveConstructFrom(std::move(_other));
	}

	~object_ptr( ) noexcept
	{
		this->decRef( );
	}

	object_ptr& operator=(const object_ptr& _other) noexcept
	{
		object_ptr{ _other }.swap(*this);

		return *this;
	}

	template <typename _OtherTy>
	object_ptr& operator=(const object_ptr<_OtherTy>& _other) noexcept
	{
		object_ptr{ _other }.swap(*this);

		return *this;
	}

	object_ptr& operator=(object_ptr&& _other) noexcept
	{
		object_ptr{ std::move(_other) }.swap(*this);

		return *this;
	}

	template <typename _OtherTy>
	object_ptr& operator=(object_ptr<_OtherTy>&& _other) noexcept
	{
		object_ptr{ std::move(_other) }.swap(*this);

		return *this;
	}

	using _base_type::get;
	
	template <typename _OtherTy>
	void swap(object_ptr<_OtherTy>& _other) noexcept
	{
		_base_type::swap(_other);
	}

	NODISCARD explicit operator bool( ) const noexcept
	{
		return static_cast<bool>(this->ptr);
	}

	NODISCARD std::add_lvalue_reference_t<_Ty> operator*( ) const noexcept
	{
		return *get( );
	}

	NODISCARD value_type* operator->( ) const noexcept
	{
		return get( );
	}
};

template <typename _Ty>
object_ptr(_Ty*) -> object_ptr<_Ty>;

template <typename _Ty, typename... _Args>
NODISCARD constexpr object_ptr<_Ty> makeObjectPtr(_Args&&... _args) noexcept
{
	return object_ptr<_Ty>(new _Ty(std::forward<_Args>(_args)...));
}

template <typename _Ty1, typename _Ty2>
NODISCARD constexpr bool operator==(
	const object_ptr<_Ty1>& _lhs,
	const object_ptr<_Ty2>& _rhs) noexcept
{
	return _lhs.get( ) == _rhs.get( );
}

template <typename _Ty>
NODISCARD constexpr bool operator==(
	const object_ptr<_Ty>& _lhs,
	std::nullptr_t&) noexcept
{
	return !_lhs.get( );
}

template <typename _Ty1, typename _Ty2>
NODISCARD constexpr std::strong_ordering operator<=>(
	const object_ptr<_Ty1>& _lhs,
	const object_ptr<_Ty2>& _rhs) noexcept
{
	return _lhs.get( ) <=> _rhs.get( );
}

template <typename _Ty>
NODISCARD constexpr std::strong_ordering
operator<=>(const object_ptr<_Ty>& _lhs, std::nullptr_t&) noexcept
{
	return _lhs.get( ) <=> static_cast<std::add_pointer_t<
		typename object_ptr<_Ty>::value_type>>(nullptr_t);
}

struct singleton_object_memory
{
	static std::pmr::synchronized_pool_resource synchronousPool;

	NODISCARD static std::pmr::memory_resource* getSynchronousPool( )
	{
		return &synchronousPool;
	}

	template <typename _Ty>
	NODISCARD static std::pmr::polymorphic_allocator<_Ty> getSynchronousAllocator( )
	{
		return { getSynchronousPool( ) };
	}
};

class Object
{
public:
	OBJECT_IMPL_( );

	NODISCARDRAWPTR static void* operator new(std::size_t _count);

	NODISCARDRAWPTR static void* operator new(std::size_t _count, size_t _align);

	static void operator delete(void* _block, size_t _count);

	static void operator delete(void* _block, size_t _count, size_t _align);

	Object( ) noexcept = default;

	NODISCARD Object* owner( ) const noexcept
	{
		return thisOwner;
	}

protected:
	Object* thisOwner{ };

private:
	static void* _allocate(size_t _count);
	static void* _allocate(size_t _count, size_t _align);
	static void _deallocate(void* _block, size_t _count);
	static void _deallocate(void* _block, size_t _count, size_t _align);
};
OBJECT_( );
