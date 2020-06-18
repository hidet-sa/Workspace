#pragma once

#include <assimp/DefaultLogger.hpp>

class LoggerManager
{
private:
	LoggerManager();
	static LoggerManager *_loggerManager;
public:
	virtual ~LoggerManager();
	static LoggerManager* getInstance();
	static void purge();
public:
	void info(const char *message) { if (_logger != nullptr) _logger->info(message); };	// Will add message to File with "info" Tag
	void info(const std::string& message) { info(message.c_str()); };
	void debug(const char *message) { if (_logger != nullptr) _logger->debug(message); };// Will add message to File with "debug" Tag
	void debug(const std::string& message) { debug(message.c_str()); };
	void error(const char *message) { if (_logger != nullptr) _logger->error(message); };// Will add message to File with "error" Tag
	void error(const std::string& message) { error(message.c_str()); };
protected:
	void create();
	void remove();
private:
	Assimp::Logger *_logger;
};
