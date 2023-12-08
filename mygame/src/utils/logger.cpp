#include "logger.hpp"

std::pmr::vector<Logger> Logger::instances{ singleton_memory::getAssynchronousPool( ) };

Logger::Logger(
	std::ostream& _os,
	const char* _infoHdr,
	const char* _warnHdr,
	const char* _debugHdr,
	const char* _errorHdr,
	const char* _logHdr
)
	: m_start{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now( )) },
	m_ostream{ _os },
	m_level{ Level::Default },
	m_infoHdr{ _infoHdr },
	m_warnHdr{ _warnHdr },
	m_debugHdr{ _debugHdr },
	m_errorHdr{ _errorHdr },
	m_logHdr{ _logHdr }
{ }
