/*
*
*/

#pragma once

#include "memory/memory.hpp"
#include <chrono>
#include <iostream>

#define LOGGER_INFO_HDR		"INFO  "
#define LOGGER_WARN_HDR		"WARN  "
#define LOGGER_DEBUG_HDR	"DEBUG "
#define LOGGER_ERROR_HDR	"Error "
#define LOGGER_LOG_HDR		"LOG   "
#define LOGGER_INFO_HDRC	"\033[0;37mINFO\033[0;0m  "
#define LOGGER_WARN_HDRC	"\033[0;33mWARN\033[0;0m  "
#define LOGGER_DEBUG_HDRC	"\033[0;33mDEBUG\033[0;0m "
#define LOGGER_ERROR_HDRC	"\033[0;31mError\033[0;0m "
#define LOGGER_LOG_HDRC		"\033[1;37mLOG\033[0;0m   "

class Logger
{
public:
	enum Level
	{
		Default,
		Info,
		Warn,
		Debug,
		Error,
		Log,
	};

	static std::pmr::vector<Logger> instances;

	template <typename... _Args>
	static Logger& registerLogger(_Args&&... _args)
	{
		return instances.emplace_back(std::forward<_Args>(_args)...);
	}

	template <typename _Ty>
	static void i(const _Ty& _e)
	{
		for (auto& i : instances)
			i.info(_e);
	}

	template <typename _Ty>
	static void w(const _Ty& _e)
	{
		for (auto& i : instances)
			i.warn(_e);
	}

	template <typename _Ty>
	static void d(const _Ty& _e)
	{
		for (auto& i : instances)
			i.debug(_e);
	}

	template <typename _Ty>
	static void e(const _Ty& _e)
	{
		for (auto& i : instances)
			i.error(_e);
	}

	template <typename _Ty>
	static void l(const _Ty& _e)
	{
		for (auto& i : instances)
			i.log(_e);
	}

	Logger(
		std::ostream& _os,
		const char* _infoHdr = LOGGER_INFO_HDRC,
		const char* _warnHdr = LOGGER_WARN_HDRC,
		const char* _debugHdr = LOGGER_DEBUG_HDRC,
		const char* _errorHdr = LOGGER_ERROR_HDRC,
		const char* _logHdr = LOGGER_LOG_HDRC
	);

	void flush( )
	{
		m_ostream.flush( );
	}

	void setLevel(Level _level)
	{
		m_level = _level;
	}

	template <typename _Ty>
	Logger& info(const _Ty& _e)
	{
		if (m_level <= Info)
			m_ostream << "[ " << m_infoHdr << _getTime() << " ]: " << _e << '\n';

		return *this;
	}

	template <typename _Ty>
	Logger& warn(const _Ty& _e)
	{
		if (m_level <= Warn)
			m_ostream << "[ " << m_warnHdr << _getTime( ) << " ]: "  << _e << '\n';

		return *this;
	}

	template <typename _Ty>
	Logger& debug(const _Ty& _e)
	{
		if (m_level <= Debug)
			m_ostream << "[ " << m_debugHdr << _getTime( ) << " ]: "  << _e << '\n';

		return *this;
	}

	template <typename _Ty>
	Logger& error(const _Ty& _e)
	{
		if (m_level <= Error)
			m_ostream << "[ " << m_errorHdr << _getTime( ) << " ]: "  << _e << '\n';

		return *this;
	}

	template <typename _Ty>
	Logger& log(const _Ty& _e)
	{
		if (m_level <= Log)
			m_ostream << "[ " << m_logHdr << _getTime( ) << " ]: "  << _e << '\n';

		return *this;
	}

	template <typename _Ty>
	friend Logger& operator<<(Logger&, const _Ty&);

private:
	std::time_t m_start;
	std::ostream& m_ostream;
	Level m_level;
	const char* m_infoHdr;
	const char* m_warnHdr;
	const char* m_debugHdr;
	const char* m_errorHdr;
	const char* m_logHdr;

	auto _getTime( ) const noexcept
	{
		const std::time_t time
			= std::chrono::system_clock::to_time_t(std::chrono::system_clock::now( ));

		return std::put_time(std::localtime(&time), "%F %T");
	}
};

template <typename _Ty>
Logger& operator<<(Logger& _lhs, const _Ty& _rhs)
{
	_lhs.log(_rhs);

	return _lhs;
}
