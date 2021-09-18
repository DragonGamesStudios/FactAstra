#pragma once
#include "ModManager.hpp"
#include "LuaStateManager.hpp"
#include "LuaStorage.hpp"

#include <thread>

struct fa_LoadedMod
{
	std::string name;
	std::string title;

	std::filesystem::path path;
	std::filesystem::path inner_path;

	bool loaded = false;
	bool is_zip = false;
};

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

	std::map<std::string, fa_LoadedMod> loaded_mods;

	fa_ModManager modmanager;
	fa_LuaStateManager state_manager;
	fa_LuaStorage storage;

	void parse_arguments(const std::vector<std::string>& args, std::map<std::string, std::string>& options, std::map<std::string, bool>& flags, std::vector<std::string>& unparsed) const;

	void console();

	// Prototypes
	fa_Error load(std::ostream& log_stream);

	void prepare_prototypes(lua_State* L, void* udata);
	void execute_prototypes(lua_State* L, void* udata);
	void cleanup_prototypes(lua_State* L, void* udata);

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

	void cmd_prototype_list(const std::vector<std::string>& args);
	void cmd_prototype_load(const std::vector<std::string>& args);
	void cmd_prototype_info(const std::vector<std::string>& args);
};