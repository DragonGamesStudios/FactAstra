#pragma once
#include "Version.hpp"
#include "errors.hpp"

#include <Spectre2D/FileSystem.h>

#include <string>
#include <memory>
#include <filesystem>
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
	fa_ModManager();

	fa_Error add_mod(const std::filesystem::path& path, bool is_additional, std::ostream& log_stream);

	void add_ignored_mod(const std::string& path);

	fa_Error add_mod_directory(const std::filesystem::path& path, std::ostream& log_stream);

	fa_Error load_configuration(const std::filesystem::path& path, std::ostream& log_stream);
	void save_configuration(const std::filesystem::path& path, std::ostream& log_stream) const;

	void register_fs(sp::FileSystem* fs);

	bool is_synchronized() const;
	void synchronize();

	fa_Error load_mod_info(const std::filesystem::path& path, fa_Mod* mod_struct, std::ostream& log_stream);

	const std::map<std::string, fa_Mod>& get_mods() const;

private:
	sp::FileSystem* fs;

	std::map<std::string, fa_Mod> mods;
	std::set<std::filesystem::path> mod_directories;
	std::set<std::filesystem::path> additional_mods;
	std::set<std::string> ignored_mods;

	bool synchronized;
};