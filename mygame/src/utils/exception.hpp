/*
* 
*/

#pragma once

#include "define.hpp"
#include <exception>

struct InvalidatedCaller
	: std::exception
{
	InvalidatedCaller(const char* const _msg)
		: std::exception(_msg)
	{ }
};

ALWAYSINLINE void xBadAlloc( )
{
	throw std::bad_alloc( );
}
