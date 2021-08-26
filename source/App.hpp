#pragma once
#include "ModManager.hpp"

#include <Spectre2D/FileSystem.h>

#include <thread>

class fa_App
{
public:
	fa_App();

	void run(const std::vector<std::string>& args);

	void quit();

private:
	std::map<std::string, std::string> startup_options;
	std::map<std::string, bool> startup_flags;

	sp::FileSystem appdata_fs;
	sp::FileSystem local_fs;

	std::ofstream log;

	std::thread console_thread;

	fa_ModManager modmanager;

	void parse_arguments(const std::vector<std::string>& args, std::map<std::string, std::string>& options, std::map<std::string, bool>& flags, std::vector<std::string>& unparsed) const;

	void console();

	// Console commands
	void cmd_modmanager_list(const std::vector<std::string>& args);
	void cmd_modmanager_checkvalid(const std::vector<std::string>& args);
	void cmd_modmanager_info(const std::vector<std::string>& args);
	void cmd_modmanager_enable(const std::vector<std::string>& args);
	void cmd_modmanager_disable(const std::vector<std::string>& args);
	void cmd_modmanager_saveconfig(const std::vector<std::string>& args);
	void cmd_modmanager_loadconfig(const std::vector<std::string>& args);
	void cmd_modmanager_pushmod(const std::vector<std::string>& args);
	void cmd_modmanager_pushdir(const std::vector<std::string>& args);
	void cmd_modmanager_rmmod(const std::vector<std::string>& args);
	void cmd_modmanager_rmdir(const std::vector<std::string>& args);
	void cmd_modmanager_unignore(const std::vector<std::string>& args);
};