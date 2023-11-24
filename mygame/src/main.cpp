
#include "utils/logger.hpp"
#include "game.hpp"
#include <fstream>
#include <iostream>

int main( )
{
	Logger::registerLogger(std::clog);
	auto logFile = std::ofstream("logger.txt");
	Logger::registerLogger(
		logFile,
		LOGGER_INFO_HDR,
		LOGGER_WARN_HDR,
		LOGGER_DEBUG_HDR,
		LOGGER_ERROR_HDRC,
		LOGGER_LOG_HDRC).setLevel(Logger::Debug);
	Game g;
	g.run( );

	return 0;
}
