#include "ModManager.hpp"
#include "json.hpp"
#include "errors.hpp"

#include <minizip/mz_zip.h>
#include <minizip/mz_strm_os.h>
#include <minizip/mz.h>

static fa_Error read_string_field(const fa_json::object& obj, const std::string& field, std::string* value)
{
	fa_Error error;

	// FactAstra version
	auto it = obj.find(field);

	if (it == obj.end())
	{
		error.code = fa_errno::invalid_json;
		error.description = "Invalid JSON: " + field + " field was missing.";
		return error;
	}

	if (it->second.index() != FA_JSON_STRING)
	{
		error.code = fa_errno::invalid_json;
		error.description = "Invalid JSON: " + field + " field was of wrong type (not string).";
		return error;
	}

	*value = std::get<fa_json::string>(it->second);

	return error;
}

fa_ModManager::fa_ModManager()
{
	fs = 0;
	synchronized = true;
}

fa_Error fa_ModManager::add_mod(const std::filesystem::path& path, bool add_additional, std::ostream& log_stream)
{
	fa_Error error;

	fa_Mod mod;
	error = load_mod_info(path, &mod, log_stream);

	if (add_additional)
		additional_mods.insert({ fs->getCorrectPath(path), path });

	if (error.code == fa_errno::invalid_json || error.code == fa_errno::invalid_filename || error.code == fa_errno::invalid_version_string)
	{
		return error;
	}

	error.code = fa_errno::ok;
	error.description.clear();

	auto ignored_it = ignored_mods.find(mod.name);

	if (ignored_it != ignored_mods.end())
	{
		if (add_additional)
		{
			ignored_mods.erase(ignored_it);
			log_stream << "Erasing mod from ignored list." << std::endl;
		}
		else
		{
			error.code = fa_errno::mod_ignored;
			error.description = "The mod is in the ignored list.";
			log_stream << error.description << std::endl;
			return error;
		}
	}

	auto added_it = mods.find(mod.name);

	if (added_it != mods.end() && added_it->second.version >= mod.version)
	{
		error.code = fa_errno::higher_version_present;
		error.description = "The mod already has a higher version present in the configuration.";
		log_stream << error.description << std::endl;
		return error;
	}

	mods[mod.name] = mod;

	return error;
}

void fa_ModManager::remove_mod(const std::string& name)
{
	auto mod_it = mods.find(name);

	if (mod_it != mods.end())
	{
		const auto& mod_path = mod_it->second.path;
		auto parent_path = mod_path.parent_path();


		auto additional_it = additional_mods.find(mod_path);
		auto dir_it = mod_directories.find(parent_path);

		if (additional_it != additional_mods.end())
		{
			additional_mods.erase(additional_it);
		}

		if (dir_it != mod_directories.end())
		{
			ignored_mods.insert({ name, mod_it->second.path });
		}

		mods.erase(mod_it);
	}
}

void fa_ModManager::add_ignored_mod(const std::string& name)
{
	// Unload mod
	std::filesystem::path path;
	auto mod_it = mods.find(name);
	
	if (mod_it != mods.end())
	{
		mods.erase(mod_it);
		path = mod_it->second.path;
	}

	ignored_mods.insert({ name, path });
}

void fa_ModManager::remove_ignored_mod(const std::string& name, std::ostream& log_stream)
{
	auto it = ignored_mods.find(name);

	if (it != ignored_mods.end())
	{
		std::filesystem::path path = it->second;
		ignored_mods.erase(it);

		add_mod(path, false, log_stream);
	}
}

fa_Error fa_ModManager::add_mod_directory(const std::filesystem::path& path, std::ostream& log_stream)
{
	fa_Error error;
	auto correct = fs->getCorrectPath(path);

	log_stream << "Adding mod directory at " << correct << "." << std::endl;

	if (!fs->isDirectory(correct))
	{
		mod_directories.insert({ correct, path });
		error.code = fa_errno::fs_entry_does_not_exist;
		error.description = "Requested directory does not exist/is not a directory.";
		log_stream << error.description << std::endl;
		return error;
	}

	correct = std::filesystem::canonical(correct);
	const auto& inserted = mod_directories.insert({ correct, path });

	if (inserted.second)
	{
		// Add the dir and mods in it
		for (const auto& entry : fs->getFilesInDirectory(correct))
		{
			std::filesystem::path mod_path = entry.path();

			if (mod_path.filename() != "configuration.json")
			{
				error = add_mod(mod_path, false, log_stream);

				if (error.code == fa_errno::invalid_json || error.code == fa_errno::invalid_filename || error.code == fa_errno::invalid_version_string)
				{
					return error;
				}
			}
		}
	}
	else
	{
		log_stream << "Requested directory is already loaded." << std::endl;
	}

	return error;
}

void fa_ModManager::remove_mod_directory(const std::filesystem::path& path)
{
	if (fs->exists(path))
	{
		auto full_path = std::filesystem::canonical(fs->getCorrectPath(path));
		auto dir_it = mod_directories.find(full_path);

		if (dir_it != mod_directories.end())
		{
			for (auto mod_it = mods.begin(); mod_it != mods.end();)
			{
				auto additional_it = additional_mods.find(mod_it->second.path);

				if (mod_it->second.path.parent_path() == full_path && additional_it == additional_mods.end())
					mod_it = mods.erase(mod_it);
				else
					mod_it++;
			}

			mod_directories.erase(dir_it);
		}
	}
}

fa_Error fa_ModManager::load_configuration(const std::filesystem::path& path, const decider_func& decider, bool invert, std::ostream& log_stream)
{
	fa_Error error;

	if (fs)
	{
		log_stream << "Loading configuration at " << fs->getCorrectPath(path) << std::endl;

		fa_json config;

		std::ifstream file = fs->openFile(path);

		fa_json_error err = config.load(file);

		file.close();

		if (err.code != fa_json_errno::ok)
		{
			error.code = fa_errno::invalid_json;
			error.description = "Invalid JSON file: " + err.description;
			log_stream << error.description << std::endl;
			return error;
		}

		// Json reading
		// The json has to be an object optionally containing the following objects: directories, additional, configuration

		if (config.index() == FA_JSON_OBJECT)
		{
			std::map<std::string, bool> previous_config;

			for (const auto& [name, mod] : mods)
				previous_config.insert({ name, mod.enabled });

			const auto& object = std::get<fa_json::object>(config);

			auto directories_it = object.find("directories");
			auto additional_it = object.find("additional");
			auto configuration_it = object.find("configuration");
			auto ignored_it = object.find("ignored");

			// Mod directories
			if (directories_it != object.end() && directories_it->second.index() == FA_JSON_ARRAY)
			{
				for (const auto& dir : std::get<fa_json::arr>(directories_it->second))
				{
					if (dir.index() == FA_JSON_STRING)
					{
						error = add_mod_directory(std::get<fa_json::string>(dir), log_stream);
						
						if (error.code == fa_errno::invalid_filename || error.code == fa_errno::invalid_json || error.code == fa_errno::invalid_version_string)
							return error;
						else
						{
							error.code = fa_errno::ok;
							error.description.clear();
						}
					}
				}
			}
			else
			{
				log_stream << "Mod directories configuration field was ignored because it was missing/had a wrong type (not-array)." << std::endl;
			}

			// Additional mods
			if (additional_it != object.end() && additional_it->second.index() == FA_JSON_ARRAY)
			{
				std::string err;
				for (const auto& mod : std::get<fa_json::arr>(additional_it->second))
				{
					if (mod.index() == FA_JSON_STRING)
					{
						error = add_mod(std::get<fa_json::string>(mod), true, log_stream);

						if (error.code == fa_errno::invalid_filename || error.code == fa_errno::invalid_json)
							return error;
						else
						{
							error.code = fa_errno::ok;
							error.description.clear();
						}
					}
				}
			}
			else
			{
				log_stream << "Additional mods configuration field was ignored because it was missing/had a wrong type (not-array)." << std::endl;
			}

			// Ignored mods
			if (ignored_it != object.end() && ignored_it->second.index() == FA_JSON_ARRAY)
			{
				for (const auto& mod : std::get<fa_json::arr>(ignored_it->second))
				{
					if (mod.index() == FA_JSON_STRING)
					{
						add_ignored_mod(std::get<fa_json::string>(mod));
					}
				}
			}
			else
			{
				log_stream << "Ignored mods configuration field was ignored because it was missing/had a wrong type (not-array)" << std::endl;
			}

			// Enabled configuration
			if (configuration_it != object.end() && configuration_it->second.index() == FA_JSON_OBJECT)
			{
				for (const auto& [mod, enabled] : std::get<fa_json::object>(configuration_it->second))
				{
					auto mod_it = mods.find(mod);

					if (mod_it != mods.end())
					{
						if (enabled.index() == FA_JSON_INTEGER)
						{
							// decider(previous_enabled, new_enabled)

							auto previous_it = previous_config.find(mod);
							bool loaded_enabled = std::get<fa_json::integer>(enabled);

							if (invert)
								loaded_enabled = !loaded_enabled;

							if (previous_it == previous_config.end())
								mod_it->second.enabled = loaded_enabled;
							else
								mod_it->second.enabled = decider(previous_it->second, loaded_enabled);
						}
					}
					else
						log_stream << "Configuration for mod \"" << mod << "\" was ignored, because it did not exist." << std::endl;
				}
			}
			else
			{
				log_stream << "Configuration configuration field was ignored because it was missing/had a wrong type (not-object)" << std::endl;
			}
		}
		else
		{
			error.code = fa_errno::invalid_json;
			error.description = "Configuration JSON's main structure is not an object.";
			log_stream << error.description << std::endl;
			return error;
		}
	}

	return error;
}

void fa_ModManager::save_configuration(const std::filesystem::path& path, std::ostream& log_stream) const
{
	fa_json::object obj;
	fa_json::arr directories;
	fa_json::arr additional;
	fa_json::object configuration;
	fa_json::arr ignored;

	for (const auto& dir : mod_directories)
		directories.push_back(dir.second.string());

	for (const auto& mod : additional_mods)
		additional.push_back(mod.second.string());

	for (const auto& mod : ignored_mods)
		ignored.push_back(mod.first);

	for (const auto& [name, mod] : mods)
	{
		configuration.insert({ name, (fa_json::integer)mod.enabled });
	}

	obj.insert({ "directories", directories });
	obj.insert({ "additional", additional });
	obj.insert({ "ignored", ignored });
	obj.insert({ "configuration", configuration });

	fa_json configuration_json = obj;

	log_stream << "Saving configuration to " << fs->getCorrectPath(path) << std::endl;

	fs->createFileIfNecessary(path);
	
	auto file = fs->openOfile(path);

	file << configuration_json;

	file.close();
}

void fa_ModManager::register_fs(sp::FileSystem* _fs)
{
	fs = _fs;
}

bool fa_ModManager::is_synchronized() const
{
	return synchronized;
}

void fa_ModManager::synchronize()
{
	synchronized = true;
}

fa_Error fa_ModManager::load_mod_info(const std::filesystem::path& _path, fa_Mod* mod_struct, std::ostream& log_stream)
{
	fa_Error error;

	auto path = fs->getCorrectPath(_path);

	bool is_dir = std::filesystem::is_directory(path);
	bool is_zip = path.extension() == ".zip";

	if (is_dir || is_zip)
	{
		// Validate filename
		std::string filename = path.filename().string();
		std::string name;
		fa_Version version;
		bool version_valid = true;

		for (auto it = filename.rbegin(); it != filename.rend(); it++)
		{
			if (*it == '_')
			{
				auto v_it = it.base();
				v_it++;

				if (v_it != filename.end())
				{
					version_valid = version.parse(std::string(it.base(), filename.end())) != 0;

					name = std::string(filename.begin(), (++it).base());

					break;
				}
			}
		}

		if (!version.major_parsed)
			name = filename;

		if (is_zip && !version.major_parsed)
		{
			error.code = fa_errno::invalid_filename;
			error.description = "Invalid .zip mod filename. 'mod_name_version.zip' expected.";
			log_stream << error.description << std::endl;
			return error;
		}

		// Load info.json
		fa_json info;
		fa_json_error info_err;

		if (is_dir)
		{
			// Dir version
			if (!fs->exists(path / "info.json"))
			{
				error.code = fa_errno::fs_entry_does_not_exist;
				error.description = "Missing info.json. In directory mods info.json has to be directly in the mod root.";
				log_stream << error.description << std::endl;
				return error;
			}

			auto info_stream = fs->openFile(path / "info.json");

			// Has to be parsed here, as zip version may use other kind of stream.
			info_err = info.load(info_stream);

			info_stream.close();
		}
		else
		{
			/*
			// Zip version
			void* stream = 0;
			void* handle = 0;

			// Open stream
			mz_stream_os_create(&stream);
			mz_stream_os_open(stream, path.string().c_str(), MZ_OPEN_MODE_READ);

			// Open zip
			mz_zip_create(&handle);

			if (mz_zip_open(handle, stream, MZ_OPEN_MODE_READ))
			{
				errstring = "Not a valid zip archive.";
				*log << errstring << std::endl;

				mz_zip_delete(&handle);
				mz_stream_os_close(stream);
				mz_stream_os_delete(&stream);

				return;
			}

			// Get info.json location
			if (mz_zip_locate_entry(handle, "info.json", 1))
			{
				if (mz_zip_locate_entry(handle, (filename + "/info.json").c_str(), 1))
				{
					errstring = "Missing info.json file.";
					*log << errstring << std::endl;

					// Close zip
					mz_zip_close(handle);
					mz_zip_delete(&handle);

					// Close stream
					//mz_stream_os_close(stream);
					//mz_stream_os_delete(&stream);

					return;
				}
			}

			mz_zip_entry_read_open(handle, 0, 0);

			mz_zip_file* info_data;
			mz_zip_entry_get_info(handle, &info_data);

			char* buff = new char[info_data->uncompressed_size + 1]{ 0 };
			mz_zip_entry_read(handle, buff, info_data->uncompressed_size);

			code = buff;

			delete[] buff;

			mz_zip_entry_close(handle);

			// Close zip
			mz_zip_close(handle);
			mz_zip_delete(&handle);

			// Close stream
			mz_stream_os_close(stream);
			mz_stream_os_delete(&stream);
			*/
		}

		if (info_err.code != fa_json_errno::ok)
		{
			error.code = fa_errno::invalid_json;
			error.description = "Invalid info.json file: " + info_err.description;
			log_stream << error.description << std::endl;
			return error;
		}

		// Validate and read info.json

		if (info.index() != FA_JSON_OBJECT)
		{
			error.code = fa_errno::invalid_json;
			error.description = "Invalid info.json file: Main structure is not an object.";
			log_stream << error.description << std::endl;
			return error;
		}

		fa_json::object info_object = std::get<fa_json::object>(info);

		// Required

		// FactAstra version
		std::string factastra_version_string;
		error = read_string_field(info_object, "factastra_version", &factastra_version_string);

		if (error.code != fa_errno::ok)
		{
			log_stream << error.description << std::endl;
			return error;
		}

		fa_Version loaded_version;

		if (loaded_version.parse(factastra_version_string))
		{
			error.code = fa_errno::invalid_version_string;
			error.description = "Invalid info.json file: factastra_version field was not a valid version string.";
			log_stream << error.description << std::endl;
			return error;
		}

		if (!loaded_version.major_parsed || !loaded_version.minor_parsed)
		{
			error.code = fa_errno::invalid_json;
			error.description = "Invalid info.json file: factastra_version field did not have at least major and minor version numbers.";
			log_stream << error.description << std::endl;
			return error;
		}

		if ((loaded_version.patch_parsed && loaded_version != factastra_version) || loaded_version.major != factastra_version.major || loaded_version.minor != factastra_version.minor)
		{
			error.code = fa_errno::mod_incompatible;
			error.description = "The loaded mod is not compatible with the game.";
			log_stream << error.description << std::endl;
			return error;
		}

		fa_Error tmp_err;

		// Name
		std::string info_name;
		tmp_err = read_string_field(info_object, "name", &info_name);

		if (tmp_err.code != fa_errno::ok)
			error = tmp_err;

		std::string title;
		tmp_err = read_string_field(info_object, "title", &title);

		if (tmp_err.code != fa_errno::ok)
			error = tmp_err;

		fa_Version info_version;
		std::string version_string;
		tmp_err = read_string_field(info_object, "version", &version_string);

		if (tmp_err.code != fa_errno::ok)
			error = tmp_err;

		if (error.code != fa_errno::ok)
		{
			log_stream << error.description << std::endl;
			return error;
		}

		if (info_version.parse(version_string))
		{
			error.code = fa_errno::invalid_version_string;
			error.description = "Invalid info.json file: version field was not a valid version string.";
			log_stream << error.description << std::endl;
			return error;
		}

		if (name != info_name)
		{
			error.code = fa_errno::invalid_filename;
			error.description = "The given mod path is not a valid. \"" + info_name + "/\" or \"" + info_name + "_" + version_string + "/\" was expected, got \"" + path.filename().string() + "\"";
			log_stream << error.description << std::endl;
			return error;
		}

		if (version.major_parsed && version != info_version)
		{
			error.code = fa_errno::invalid_filename;
			error.description = "The given mod path is not a valid. \"" + info_name + "/\" or \"" + info_name + "_" + version_string + "/\" was expected, got \"" + path.filename().string() + "\"";
			log_stream << error.description << std::endl;
			return error;
		}

		// Optional fields
		std::string description;
		read_string_field(info_object, "description", &description);

		mod_struct->name = name;
		mod_struct->title = title;
		mod_struct->version = version;
		mod_struct->description = description;
		mod_struct->is_zip = is_zip;
		mod_struct->path = path;
		mod_struct->enabled = true;
	}
	else
	{
		error.code = fa_errno::not_a_mod;
		error.description = "Given path does not point to a mod.";
		log_stream << error.description << std::endl;
		return error;
	}

	return error;
}

const std::map<std::string, fa_Mod>& fa_ModManager::get_mods() const
{
	return mods;
}

bool fa_ModManager::set_mod_enabled(const std::string& mod, bool enabled)
{
	auto it = mods.find(mod);

	if (it != mods.end())
	{
		it->second.enabled = enabled;
		return true;
	}

	return false;
}

bool fa_ModManager::set_mod_enabled(const std::map<std::string, fa_Mod>::const_iterator& mod, bool enabled)
{
	if (mod != mods.end())
	{
		// const iterator -> iterator trick
		std::map<std::string, fa_Mod>::iterator it = mods.erase(mod, mod);

		it->second.enabled = enabled;

		return true;
	}

	return false;
}
