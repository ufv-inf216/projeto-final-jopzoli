/*
*
*/

#pragma once

#include "utils/asserts.hpp"
#include "utils/exception.hpp"
#include "utils/util.hpp"
#include <memory>
#include <memory_resource>

#define NODISCARDRAWPTR NODISCARDY("discarding result may cause memory leak")

#if defined(_MSC_VER) // vv _MSC_VER vv
#define ALLOCA(x)		__malloca(x)
#define ALLOCA_MAXSZ	_ALLOCA_S_THRESHOLD
#define FREEA(x)		__freea(x)
#define MALLOC_MAXSZ	_HEAP_MAXREQ
#elif defined(__GNUC__) // ^^ _MSC_VER ^^ vv __GNUC__ vv
#define ALLOCA(x)		alloca(x)
#define ALLOCA_MAXSZ	INT_MAX
#define FREEA(x)
#define MALLOC_MAXSZ	INT_MAX
#else // ^^ _MSC_VER / vv __GNUC__ ^^ vv!
#define ALLOCA(x)
#define ALLOCA_MAXSZ
#define FREEA(x)
#define MALLOC_MAXSZ
#endif // ^^ !_MSC_VER / !__GNUC__ ^^

constexpr size_t Default_new_align = __STDCPP_DEFAULT_NEW_ALIGNMENT__;

namespace detail
{
struct identity_equal_resource
	: std::pmr::memory_resource
{
protected:
	bool do_is_equal(const memory_resource& _other) const noexcept override
	{
		return this == &_other;
	}
};

template <size_t _N>
struct alignas(1) padding
{
	byte_t pad[_N];
};

template <typename _TagTy = void>
struct single_link
{
	using tag_type = _TagTy;
	single_link* next;
};

template <typename _TagTy = void>
struct double_link
{
	using tag_type = _TagTy;
	double_link* prev;
	double_link* next;
};

template <typename _Ty, typename _TagTy = void>
struct linked_stack
{
	using link_type = single_link<_TagTy>;
	static_assert(std::is_base_of_v<link_type, _Ty>,
		"linked_stack requires single_link<typename> to be base of given type");

	link_type* head{ nullptr };
#if defined(_DEBUG) // vv _DEBUG vv
	size_t count{ 0 };
#endif // ^^ _DEBUG ^^

	constexpr linked_stack( ) noexcept = default;

	constexpr linked_stack(linked_stack&& _other) noexcept
		: head{ std::exchange(_other.head, nullptr) }
	{ }

	constexpr linked_stack& operator=(linked_stack&& _other) noexcept
	{
		if (&_other != this)
			head = std::exchange(_other.head, nullptr);

		return *this;
	}

	NODISCARD static constexpr link_type* asLink(_Ty* const _ptr) noexcept
	{
		return static_cast<link_type*>(_ptr);
	}

	NODISCARD static constexpr _Ty* asItem(link_type* const _ptr) noexcept
	{
		return static_cast<_Ty*>(_ptr);
	}

	NODISCARD constexpr bool isEmpty( ) const noexcept
	{
		return !head;
	}

	constexpr _Ty* pop( ) noexcept
	{
		_EXPECTS(head, "stack shouldn't be empty");
		_Ty* const ptr = asItem(head);
		head = head->next;
		ptr->next = nullptr;
#if defined(_DEBUG) // vv _DEBUG vv
		count--;
#endif // ^^ _DEBUG ^^

		return ptr;
	}

	constexpr void push(_Ty* const _val) noexcept
	{
		link_type* const ptr = asLink(_val);
		ptr->next = head;
		head = ptr;
#if defined(_DEBUG) // vv _DEBUG vv
		count++;
#endif // ^^ _DEBUG ^^
	}

	constexpr _Ty* remove(_Ty* const _val) noexcept
	{
		_Ty* const ptr = asLink(_val);
		for (link_type** nxt = &head; *nxt; nxt = &(*nxt)->next)
		{
			if (*nxt == ptr)
			{
				*nxt = ptr->next;
				ptr->next = nullptr;
#if defined(_DEBUG) // vv _DEBUG vv
				count--;
#endif // ^^ _DEBUG ^^
				return ptr;
			}
		}

		return nullptr;
	}

	constexpr _Ty* top( ) const noexcept
	{
		return asItem(head);
	}
};
}

struct singleton_memory
{
	NODISCARD static std::pmr::synchronized_pool_resource* getPool( )
	{
		static std::pmr::synchronized_pool_resource pool{ };

		return &pool;
	}

	template <typename _Ty>
	NODISCARD static std::pmr::polymorphic_allocator<_Ty> getAllocator( )
	{
		return { getPool( ) };
	}
};

NODISCARD constexpr size_t adjustOffset(size_t _offset, size_t _align) noexcept
{
	return (_offset + (--_align)) & ~_align;
}

NODISCARD bool isAligned(void* const _ptr, size_t _align) noexcept;

NODISCARD constexpr bool isPow2(size_t _val) noexcept
{
	return _val != 0 && (_val & (_val - 1)) == 0;
}
