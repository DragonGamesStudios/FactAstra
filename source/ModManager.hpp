#pragma once
#include "Version.hpp"
#include "errors.hpp"

#include <Spectre2D/FileSystem.h>

#include <string>
#include <memory>
#include <filesystem>
#include <functional>
#include <set>

struct fa_Mod
{
	std::string title;
	std::string name;
	std::string description;

	bool is_zip;
	std::filesystem::path path;
	std::filesystem::path inner_path;
	
	fa_Version version;

	bool enabled;
};

class fa_ModManager
{
public:
	typedef std::function<bool(bool, bool)> decider_func;

	fa_ModManager();

	fa_Error add_mod(const std::filesystem::path& path, bool is_additional, std::ostream& log_stream);
	void remove_mod(const std::string& name);

	void add_ignored_mod(const std::string& name);
	void remove_ignored_mod(const std::string& name, std::ostream& log_stream);

	fa_Error add_mod_directory(const std::filesystem::path& path, std::ostream& log_stream);
	void remove_mod_directory(const std::filesystem::path& path);

	fa_Error load_configuration(const std::filesystem::path& path, const decider_func& decider, bool invert, std::ostream& log_stream);
	void save_configuration(const std::filesystem::path& path, std::ostream& log_stream) const;

	void register_fs(sp::FileSystem* fs);

	bool is_synchronized() const;
	void synchronize();

	fa_Error load_mod_info(const std::filesystem::path& path, fa_Mod* mod_struct, std::ostream& log_stream);

	const std::map<std::string, fa_Mod>& get_mods() const;

	bool set_mod_enabled(const std::string& mod, bool enabled);
	bool set_mod_enabled(const std::map<std::string, fa_Mod>::const_iterator& mod, bool enabled);

private:
	sp::FileSystem* fs;

	std::map<std::string, fa_Mod> mods;
	std::map<std::filesystem::path, std::filesystem::path> mod_directories;
	std::map<std::filesystem::path, std::filesystem::path> additional_mods;
	std::map<std::string, std::filesystem::path> ignored_mods;

	bool synchronized;
};