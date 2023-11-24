/*
* 
*/

#pragma once

#include "define.hpp"
#include <cassert>
#include <source_location>

#if defined(_DEBUG) // vv _DEBUG vv

#if defined(_MSC_VER) // vv _MSC_VER vv
#define _ASSERTC_(cond)				_ASSERTE(cond)
#define _ASSERT_EXPRI_(cond, ...)															\
	(void) (																				\
		(!!(cond)) ||																		\
		(1 != _CrtDbgReport(_CRT_ASSERT,													\
			__FILE__, __LINE__, NULL,														\
			"%s `%s` failed: %s\ndetails: %s(%d:%d) `%s`",									\
			ARG_1(__VA_ARGS__), STRINGFY(cond), ARG_2(__VA_ARGS__),							\
			std::source_location::current().file_name(),									\
			std::source_location::current().line(), std::source_location::current().column(),\
			std::source_location::current().function_name())) ||							\
			(_CrtDbgBreak(), 0))
#define _WARN_EXPRI_(cond, ...)																\
	(void) (																				\
		(!!(cond)) ||																		\
		(1 != _CrtDbgReport(_CRT_WARN,														\
			__FILE__, __LINE__, NULL,														\
			"warn: `%s`: %s\ndetails: %s(%d:%d) `%s`",										\
			STRINGFY(cond), __VA_ARGS__,													\
			std::source_location::current().file_name(),									\
			std::source_location::current().line(), std::source_location::current().column(),\
			std::source_location::current().function_name())))
#elif defined(__GNUC__) // ^^ _MSC_VER ^^ vv __GNUC__ vv
#define _ASSERTC_(cond)				assert(cond)
#define _ASSERT_EXPRI_(cond, ...)
#define _WARN_EXPRI_(cond, ...)
#else // ^^ _MSC_VER / __GNUC__ ^^ vv ! vv
#define _ASSERTC_(cond)				assert(cond)
#define _ASSERT_EXPRI_(cond, ...)
#define _WARN_EXPRI_(cond, ...)
#endif // ^^ !_MSC_VER / !__GNUC__ ^^

#else // ^^ _DEBUG ^^ vv ! vv
#define _ASSERTC_(cond)
#define _ASSERT_EXPRI_(cond, ...)
#define _WARN_EXPRI_(cond, ...)
#endif // ^^ !_DEBUG ^^

#define _ASSERTION(cond, ...)	_ASSERT_EXPRI_(cond, "assertion", __VA_ARGS__)
#define _ENSURES(cond, ...)		_ASSERT_EXPRI_(cond, "poscondition", __VA_ARGS__)
#define _EXPECTS(cond, ...)		_ASSERT_EXPRI_(cond, "precondition", __VA_ARGS__)
#define _WARNING(cond, ...)		_WARN_EXPRI_(cond, __VA_ARGS__)
