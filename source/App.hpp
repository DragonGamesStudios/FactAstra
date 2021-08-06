#pragma once

#include <Spectre2D/FileSystem.h>

#include <thread>

class App
{
public:
	App();

	void run(const std::vector<std::string>& args);

	void quit();

private:
	std::map<std::string, std::string> startup_options;
	std::map<std::string, bool> startup_flags;

	sp::FileSystem appdata_fs;
	sp::FileSystem local_fs;

	std::ofstream log;

	std::thread console_thread;

	void parse_arguments(const std::vector<std::string>& args, std::map<std::string, std::string>& options, std::map<std::string, bool>& flags, std::vector<std::string>& unparsed) const;

	void console();

	// Console commands
	void cmd_modmanager_list(const std::vector<std::string>& args);
};