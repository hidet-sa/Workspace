#include "Logger.h"

LoggerManager* LoggerManager::_loggerManager = nullptr;
LoggerManager::LoggerManager()
{
	_logger = nullptr;
}

LoggerManager::~LoggerManager()
{
	remove();
}

LoggerManager *LoggerManager::getInstance()
{
	if (!_loggerManager) {
		_loggerManager = new LoggerManager();
		_loggerManager->create();
	}
	return _loggerManager;
}

void LoggerManager::purge()
{
	if (!_loggerManager) {
		return;
	}
	auto p = _loggerManager;
	_loggerManager = nullptr;
	p->remove();
	delete p;
}

void LoggerManager::create()
{
	if (_logger != nullptr) {
		return;
	}
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

	// Create a logger instance for Console Output
	Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);

	// Create a logger instance for File Output (found in project folder or near .exe)
	Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);

	_logger = Assimp::DefaultLogger::get();
}

void LoggerManager::remove()
{
	if (_logger == nullptr) {
		return;
	}
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
	_logger = nullptr;
}
