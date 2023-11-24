/*
*
*/

#pragma once

#include <cstddef>
#include <cstdint>

typedef unsigned char byte_t;
typedef unsigned short word_t;
typedef unsigned int dword_t;
typedef unsigned long long qword_t;

#if defined(_DEBUG) // vv DEBUG vv
#define _CONSTEXPRG
#else // ^^ DEBUG ^^ vv ! vv
#define _CONSTEXPRG constexpr
#endif // ^^ !_DEBUG ^^

#if defined(_MSC_VER) // vv MSC_VER vv
#define ALLOCATOR		_declspec(allocator)
#define ALWAYSINLINE	_forceinline
#define ASSUME(pred)	_assume(pred)
#define INLINEASM		_asm
#define NOVTABLE		_declspec(novtable)

#define __PRETTY_FUNCTION__	_FUNCSIG__
#define WARN_VARINST_		26495
#define WARN_DEPRECATED_	4996
#elif defined(__GNUC__) // ^^ MSC_VER ^^ vv _GNUC__ vv
#define ALLOCATOR		_attribute__((malloc))
#define ALWAYSINLINE	_attribute__((always_inline))
#define ASSUME(pred)	((pred) ? static_cast<void>(0) : _builtin_unreachable( ))
#define INLINEASM		_asm__
#define NOVTABLE

#define WARN_VARINST_
#define WARN_DEPRECATED_
#else // ^^ MSC_VER / _GNUC__ ^^ vv ! vv
#define ALLOCATOR
#define ALWAYSINLINE
#define ASSUME(pred)	static_cast<void>((cond) ? 0 : 0)
#define INLINEASM		asm
#define NOVTABLE

#define WARN_VARINST_
#define WARN_DEPRECATED_
#endif // ^^ !_MSC_VER / !__GNUC__ ^^

#define CARRIESDEPENDENCY	[[carries_dependency]]
#define DEPRECATED			[[deprecated]]
#define DEPRECATEDY(reason)	[[deprecated(reason)]]
#define FALLTHROUGH			[[fallthrough]]
#define MAYBEUNUSED			[[maybe_unused]]
#define NODISCARD			[[nodiscard]]
#define NODISCARDY(reason)	[[nodiscard(reason)]]
#define NOUNIQUEADDR		[[no_unique_address]]
#define NORETURN			[[noreturn]]

#define EXPAND(...)			__VA_ARGS__
#define GLUE_(data, b)			data##b
#define GLUE(data, b)			GLUE_(data, b)
#define STRINGFY(x)			#x
#define VARCOUNT_(_, _9, _8, _7, _6, _5, _4, _3, _2, n, ...) n
#define VARCOUNT(...)		EXPAND(VARCOUNT_(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1,))
#define TUPLE_HEAD_(x, ...)	(x)
#define TUPLE_HEAD(...)		EXPAND(TUPLE_HEAD_(__VA_ARGS__))
#define TUPLE_TAIL_(_, ...)	(__VA_ARGS__)
#define TUPLE_TAIL(...)		EXPAND(TUPLE_TAIL_(__VA_ARGS__))

#define DROP_1(_, ...)	__VA_ARGS__
#define DROP_2(_, ...)	EXPAND(DROP_1(__VA_ARGS__))
#define DROP_3(_, ...)	EXPAND(DROP_2(__VA_ARGS__))
#define DROP_4(_, ...)	EXPAND(DROP_3(__VA_ARGS__))
#define DROP_5(_, ...)	EXPAND(DROP_4(__VA_ARGS__))
#define DROP_6(_, ...)	EXPAND(DROP_5(__VA_ARGS__))
#define DROP_7(_, ...)	EXPAND(DROP_6(__VA_ARGS__))
#define DROP_8(_, ...)	EXPAND(DROP_7(__VA_ARGS__))
#define DROP_9(_, ...)	EXPAND(DROP_8(__VA_ARGS__))
#define TAKEONE(_, ...)	_
#define ARG_1(...)	EXPAND(TAKEONE(__VA_ARGS__))
#define ARG_2(...)	ARG_1(EXPAND(DROP_1(__VA_ARGS__)))
#define ARG_3(...)	ARG_1(EXPAND(DROP_2(__VA_ARGS__)))
#define ARG_4(...)	ARG_1(EXPAND(DROP_3(__VA_ARGS__)))
#define ARG_5(...)	ARG_1(EXPAND(DROP_4(__VA_ARGS__)))
#define ARG_6(...)	ARG_1(EXPAND(DROP_5(__VA_ARGS__)))
#define ARG_7(...)	ARG_1(EXPAND(DROP_6(__VA_ARGS__)))
#define ARG_8(...)	ARG_1(EXPAND(DROP_7(__VA_ARGS__)))
#define ARG_9(...)	ARG_1(EXPAND(DROP_8(__VA_ARGS__)))

#define OP_AND_BITWISE_(x)													\
inline x operator&(x _lhs, x _rhs)											\
{																			\
	return static_cast<x>(static_cast<int>(_lhs) & static_cast<int>(_rhs));	\
}
#define OP_ANDEQ_BITWISE_(x)												\
inline x& operator&=(x& _lhs, x _rhs)										\
{																			\
	return _lhs = static_cast<x>(static_cast<int>(_lhs) & static_cast<int>(_rhs));\
}
#define OP_OR_BITWISE_(x)													\
inline x operator|(x _lhs, x _rhs)											\
{																			\
	return static_cast<x>(static_cast<int>(_lhs) | static_cast<int>(_rhs));	\
}
#define OP_OREQ_BITWISE_(x)													\
inline x& operator|=(x& _lhs, x _rhs)										\
{																			\
	return _lhs = static_cast<x>(static_cast<int>(_lhs) | static_cast<int>(_rhs));\
}
#define OP_XOR_BITWISE_(x)													\
inline x operator^(x _lhs, x _rhs)											\
{																			\
	return static_cast<x>(static_cast<int>(_lhs) ^ static_cast<int>(_rhs));	\
}
#define OP_XOREQ_BITWISE_(x)												\
inline x& operator^=(x& _lhs, x _rhs)										\
{																			\
	return _lhs = static_cast<x>(static_cast<int>(_lhs) ^ static_cast<int>(_rhs));\
}
#define OP_BITWISE_COMPAT_(x)				\
	OP_AND_BITWISE_(x) OP_ANDEQ_BITWISE_(x)	\
	OP_OR_BITWISE_(x) OP_OREQ_BITWISE_(x)	\
	OP_XOR_BITWISE_(x) OP_XOREQ_BITWISE_(x)
