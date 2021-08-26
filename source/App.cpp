#include "App.hpp"
#include "util.hpp"

#include <iostream>
#include <functional>
#include <regex>

// Deciders
static bool decider_or(bool previous, bool loaded)
{
	return previous || loaded;
}

static bool decider_and(bool previous, bool loaded)
{
	return previous && loaded;
}

static bool decider_xor(bool previous, bool loaded)
{
	return previous ^ loaded;
}

static bool decider_nor(bool previous, bool loaded)
{
	return !(previous || loaded);
}

static bool decider_nand(bool previous, bool loaded)
{
	return !(previous && loaded);
}

static bool decider_xnor(bool previous, bool loaded)
{
	return !(previous ^ loaded);
}

static bool decider_override(bool previous, bool loaded)
{
	return loaded;
}

const std::map<std::string, std::function<bool(bool, bool)>> deciders = {
	{ "or", decider_or },
	{ "and", decider_and },
	{ "xor", decider_xor },
	{ "nor", decider_nor },
	{ "nand", decider_nand },
	{ "xnor", decider_xnor },
	{ "override", decider_override },
};

fa_App::fa_App()
{
	appdata_fs = sp::FileSystem(true);

	appdata_fs.createDirIfNecessary("DragonGames");
	appdata_fs.createDirIfNecessary("DragonGames/FactAstra");
	appdata_fs.enterDir("DragonGames/FactAstra");
	appdata_fs.createDirIfNecessary("mods");

	// __local__ is directory the executable is run from
	// __appdata__ is the game's appdata directory
	//__root__ is the game's root, where the executable is placed
	local_fs.addPathTemplate("__local__", local_fs.getCurrentPath());
	local_fs.enterDir(local_fs.getCorrectPath(local_fs.getExecutable()));
	local_fs.addPathTemplate("__appdata__", appdata_fs.getCurrentPath());
	local_fs.addPathTemplate("__root__", local_fs.getCurrentPath());

	startup_flags = {
		{ "console", false },
		{ "modmanager", false },
		{ "savemanager", false },
		{ "window", false },
	};

	startup_options = {
		{ "l", appdata_fs.getCorrectPath("log.log").string() },
	};

	modmanager.register_fs(&local_fs);
}

void fa_App::run(const std::vector<std::string>& args)
{
	if (args.size() > 1)
	{
		std::vector<std::string> arguments;
		std::vector<std::string> unparsed;

		auto it = args.begin();
		it++; // Skip program name
		for (; it != args.end(); it++)
		{
			arguments.push_back(*it);
		}

		parse_arguments(arguments, startup_options, startup_flags, unparsed);

		if (!unparsed.empty())
			std::cerr << "Unexpected argument: " << unparsed[0] << std::endl;

		// Validation
		if (startup_flags.at("console"))
		{
			startup_flags.at("modmanager") = true;
			startup_flags.at("savemanager") = true;
		}

		std::filesystem::path log_path = startup_options.at("l");

		if (!local_fs.isDirectory(log_path))
		{
			log = local_fs.openOfile(log_path);
		}
		else
		{
			log = local_fs.openOfile(log_path / "log.log");
		}
	}
	else
	{
		startup_flags.at("console") = true;
		startup_flags.at("modmanager") = true;
		startup_flags.at("savemanager") = true;
		startup_flags.at("window") = true;

		log = local_fs.openOfile(startup_options.at("l"));
	}

	// Log success message
	log << "App started successfully with options: " << std::endl;

	for (const auto& [key, value] : startup_options)
	{
		log << key << ": " << value << std::endl;
	}

	log << "And flags: ";

	for (const auto& [flag, _] : startup_flags)
		log << flag << ' ';

	log << std::endl;

	if (appdata_fs.isRegularFile("mods/configuration.json"))
	{
		if (modmanager.load_configuration("__appdata__/mods/configuration.json", decider_override, false, log).code != fa_errno::ok)
			return;
	}
	else
	{
		if (modmanager.add_mod_directory("__appdata__/mods/", log).code != fa_errno::ok)
			return;
		//modmanager.add_mod_directory("__root__/data/");
	}

	// Console executed AT THE END!
	if (startup_flags.at("console") || startup_flags.at("modmanager") || startup_flags.at("savemanager"))
	{
		console_thread = std::thread(&fa_App::console, this);
	}
}

void fa_App::quit()
{
	if (console_thread.joinable())
	{
		log << "Joining console thread..." << std::endl;
		console_thread.join();
	}

	// Save current config
	modmanager.save_configuration("__appdata__/mods/configuration.json", log);

	log << "Safely terminating the process.";
	log.close();
}

void fa_App::parse_arguments(const std::vector<std::string>& args, std::map<std::string, std::string>& options, std::map<std::string, bool>& flags, std::vector<std::string>& unparsed) const
{
	for (auto it = args.begin(); it != args.end(); it++)
	{
		const auto& arg = *it;
		if (arg[0] == '-')
		{
			if (arg[1] == '-')
			{
				// Flag
				std::string flag = arg.substr(2);
				auto flag_it = flags.find(flag);

				if (flag_it != flags.end())
					flag_it->second = true;
				else
					unparsed.push_back(arg);
			}
			else
			{
				// Option
				std::string option = arg.substr(1);
				auto option_it = options.find(option);

				if (option_it != options.end())
				{
					it++;

					if (it != args.end())
						option_it->second = *it;
					else
					{
						unparsed.push_back(arg);
					}
				}
				else
					unparsed.push_back(arg);
			}
		}
		else
			unparsed.push_back(arg);
	}
}

void fa_App::console()
{
	std::string command;
	std::vector<std::string> args;
	bool running = true;

	bool modmanager_enabled = startup_flags.at("modmanager");
	bool savemanager_enabled = startup_flags.at("savemanager");
	bool console_enabled = startup_flags.at("console");

	std::map<std::string, std::map<std::string, std::function<void(const std::vector<std::string>&)>>> commands;

	// Load requested modules
	if (modmanager_enabled)
		commands.insert({ "modmanager", {
			{ "list", std::bind(&fa_App::cmd_modmanager_list, this, std::placeholders::_1) },
			{ "checkvalid", std::bind(&fa_App::cmd_modmanager_checkvalid, this, std::placeholders::_1) },
			{ "info", std::bind(&fa_App::cmd_modmanager_info, this, std::placeholders::_1) },
			{ "enable", std::bind(&fa_App::cmd_modmanager_enable, this, std::placeholders::_1) },
			{ "disable", std::bind(&fa_App::cmd_modmanager_disable, this, std::placeholders::_1) },
			{ "saveconfig", std::bind(&fa_App::cmd_modmanager_saveconfig, this, std::placeholders::_1) },
			{ "loadconfig", std::bind(&fa_App::cmd_modmanager_loadconfig, this, std::placeholders::_1) },
			{ "pushmod", std::bind(&fa_App::cmd_modmanager_pushmod, this, std::placeholders::_1) },
			{ "pushdir", std::bind(&fa_App::cmd_modmanager_pushdir, this, std::placeholders::_1) },
			{ "rmmod", std::bind(&fa_App::cmd_modmanager_rmmod, this, std::placeholders::_1) },
			{ "rmdir", std::bind(&fa_App::cmd_modmanager_rmdir, this, std::placeholders::_1) },
			{ "unignore", std::bind(&fa_App::cmd_modmanager_unignore, this, std::placeholders::_1) },
		} });

	if (savemanager_enabled)
		commands.insert({ "savemanager", {
		} });

	while (running)
	{
		std::cerr << ">>> ";
		
		while (!getline(std::cin, command)) {}

		args = fa_util::split(command, " ");

		if (args[0] == "quit")
			running = false;
		else if (args.size() > 1)
		{
			std::string mod = args[0];
			std::string cmd = args[1];

			auto mod_it = commands.find(mod);

			if (mod_it != commands.end())
			{
				auto cmd_it = mod_it->second.find(cmd);

				if (cmd_it != mod_it->second.end())
				{
					std::vector<std::string> arguments;
					auto arg_it = args.begin();
					std::advance(arg_it, 2); // Skip first two

					for (; arg_it != args.end(); arg_it++)
						arguments.push_back(*arg_it);

					cmd_it->second(arguments);
				}
				else
				{
					std::cerr << "Command '" << mod << " -> " << cmd << "' not recognized." << std::endl;
				}
			}
			else
			{
				std::cerr << "Module '" << mod << "' not recognized." << std::endl;
			}


		}
		else
		{
			std::cerr << "Command '" << args[0] << "' not recognized." << std::endl;
		}
	}

	std::cerr << "Terminating console." << std::endl;
}

void fa_App::cmd_modmanager_list(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {
		{"t", "all"},
		{"r", ".+"},
		{"f", "all"}
	};

	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	std::string t_option = options.at("t");
	bool includes_dir = t_option != "zip";
	bool includes_zip = t_option != "dir";

	std::string f_option = options.at("f");
	bool includes_enabled = f_option != "disabled";
	bool includes_disabled = f_option != "enabled";

	std::regex pattern(options.at("r"));

	for (const auto& [name, mod] : modmanager.get_mods())
	{
		if (
			((mod.is_zip && includes_zip) || (!mod.is_zip && includes_dir)) &&
			((mod.enabled && includes_enabled) || (!mod.enabled && includes_disabled)) &&
			std::regex_match(name, pattern)
			)
		{
			std::cout << std::endl << "Name: " << name << std::endl << "Title: " << mod.title << std::endl << "Version: " << mod.version.dump() << std::endl;
		}
	}
}

void fa_App::cmd_modmanager_checkvalid(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	std::cerr << "Note: Dependency check is not implemented yet!" << std::endl;

	const auto& mods = modmanager.get_mods();

	if (unparsed.size())
	{
		std::string mod = unparsed[0];
		
		auto mod_it = mods.find(mod);

		if (mod_it != mods.end())
		{
			// Specific mod for dependencies
			std::cout << std::endl << "File structure: valid" << std::endl << "info.json: valid" << std::endl << "Dependencies: valid" << std::endl;
		}
		else
		{
			// Specififc path for full validity
			fa_Mod loaded_mod;
			fa_Error error = modmanager.load_mod_info(mod, &loaded_mod, std::cerr);
			
			std::cout << std::endl;

			switch (error.code)
			{
			case fa_errno::ok:
				// TODO: dependencies check
				std::cout << "Name: " << loaded_mod.name << std::endl << "File structure: valid" << std::endl << "info.json: valid" << std::endl << "Dependencies: valid" << std::endl;
				break;

			case fa_errno::fs_entry_does_not_exist:
			case fa_errno::invalid_filename:
				std::cout << "Name: unknown" << std::endl
					<< "File structure: invalid" << std::endl << std::endl
					<< error.description << std::endl << std::endl
					<< "info.json: unknown" << std::endl
					<< "Dependencies: unknown" << std::endl;
				break;

			case fa_errno::invalid_json:
			case fa_errno::invalid_version_string:
				std::cout << "Name: unknown" << std::endl
					<< "File structure: valid" << std::endl
					<< "info.json: invalid" << std::endl << std::endl
					<< error.description << std::endl << std::endl
					<< "Dependencies: unknown" << std::endl;
				break;

			case fa_errno::not_a_mod:
				std::cout << "The path does not point to a mod." << std::endl;
				break;

			default:
				std::cout << "Unexpected error occured: " << error.description << std::endl;
			}
		}
	}
	else
	{
		// All mods for dependencies
		for (const auto& [name, mod] : mods)
		{
			std::cout << std::endl << "Name: " << name << std::endl << "File structure: valid" << std::endl << "info.json: valid" << std::endl << "Dependencies: valid" << std::endl;
		}
	}
}

void fa_App::cmd_modmanager_info(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		std::string mod = unparsed[0];
		const fa_Mod* info = 0;
		fa_Mod loaded;
		const auto& mods = modmanager.get_mods();

		auto mod_it = mods.find(mod);

		if (mod_it != mods.end())
			info = &mod_it->second;
		else
		{
			fa_Error error = modmanager.load_mod_info(mod, &loaded, std::cerr);

			if (error.code != fa_errno::ok)
			{
				std::cout << std::endl << "Operation has failed because of an error when loading mod information: " << error.description << std::endl;
				return;
			}

			info = &loaded;
		}

		std::cout << std::endl
			<< "Name: " << info->name << std::endl
			<< "Title: " << info->title << std::endl
			<< "Description: " << info->description << std::endl
			<< "Version: " << info->version.dump() << std::endl
			<< "Path: " << info->path << std::endl
			<< "Type: " << (info->is_zip ? "zip" : "dir") << std::endl
			<< "Enabled: " << (info->enabled ? "yes" : "no") << std::endl;
	}
	else
		std::cerr << "Missing value for parameter <name/path>" << std::endl;
}

void fa_App::cmd_modmanager_enable(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {
		{"t", "all"},
		{"r", ".+"},
		{"f", "all"}
	};

	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		if (!modmanager.set_mod_enabled(args[0], true))
			std::cerr << "Specified mod is not present in the configuration." << std::endl;
	}
	else
	{
		std::string t_option = options.at("t");
		bool includes_dir = t_option != "zip";
		bool includes_zip = t_option != "dir";

		std::string f_option = options.at("f");
		bool includes_enabled = f_option != "disabled";
		bool includes_disabled = f_option != "enabled";

		std::regex pattern(options.at("r"));

		for (auto mod_it = modmanager.get_mods().begin(); mod_it != modmanager.get_mods().end(); mod_it++)
		{
			if (
				((mod_it->second.is_zip && includes_zip) || (!mod_it->second.is_zip && includes_dir)) &&
				((mod_it->second.enabled && includes_enabled) || (!mod_it->second.enabled && includes_disabled)) &&
				std::regex_match(mod_it->first, pattern)
				)
			{
				modmanager.set_mod_enabled(mod_it, true);
			}
		}
	}
}

void fa_App::cmd_modmanager_disable(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {
		{"t", "all"},
		{"r", ".+"},
		{"f", "all"}
	};

	std::map<std::string, bool> flags = {
		{ "includebase", false }
	};

	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	bool include_base = flags.at("includebase");

	if (unparsed.size())
	{
		if (unparsed[0] != "core" && (unparsed[0] != "base" || include_base))
		{
			if (!modmanager.set_mod_enabled(unparsed[0], false))
				std::cerr << "Specified mod is not present in the configuration." << std::endl;
		}
		else
			std::cerr << "Attempt to disable core/base mod." << std::endl;
	}
	else
	{
		std::string t_option = options.at("t");
		bool includes_dir = t_option != "zip";
		bool includes_zip = t_option != "dir";

		std::string f_option = options.at("f");
		bool includes_enabled = f_option != "disabled";
		bool includes_disabled = f_option != "enabled";

		std::regex pattern(options.at("r"));

		for (auto mod_it = modmanager.get_mods().begin(); mod_it != modmanager.get_mods().end(); mod_it++)
		{
			if (
				((mod_it->second.is_zip && includes_zip) || (!mod_it->second.is_zip && includes_dir)) &&
				((mod_it->second.enabled && includes_enabled) || (!mod_it->second.enabled && includes_disabled)) &&
				std::regex_match(mod_it->first, pattern) &&
				mod_it->first != "core" &&
				(mod_it->first != "base" || include_base)
				)
			{
				modmanager.set_mod_enabled(mod_it, false);
			}
		}
	}
}

void fa_App::cmd_modmanager_saveconfig(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		modmanager.save_configuration(unparsed[0], std::cerr);
	}
	else
		std::cerr << "Missing value for parameter <path>." << std::endl;
}

void fa_App::cmd_modmanager_loadconfig(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {
		{ "o", "override" }
	};

	std::map<std::string, bool> flags = {
		{ "negate", false, }
	};

	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		auto decider_it = deciders.find(options.at("o"));

		if (decider_it != deciders.end())
			modmanager.load_configuration(unparsed[0], decider_it->second, flags.at("negate"), std::cerr);
		else
			std::cerr << "Unknown -o option. Only or/and/xor/nor/nand/xnor/override are allowed." << std::endl;
	}
	else
		std::cerr << "Missing value for parameter <path>." << std::endl;
}

void fa_App::cmd_modmanager_pushmod(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		fa_Error error = modmanager.add_mod(unparsed[0], true, std::cerr);

		if (error.code != fa_errno::ok)
			std::cout << std::endl << "Operation failed because of an error: " << error.description << std::endl;
	}
	else
		std::cerr << "Missing value for parameter <path>." << std::endl;
}

void fa_App::cmd_modmanager_pushdir(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		fa_Error error = modmanager.add_mod_directory(unparsed[0], std::cerr);

		if (error.code != fa_errno::ok)
			std::cout << std::endl << "Operation failed because of an error: " << error.description << std::endl;
	}
	else
		std::cerr << "Missing value for parameter <path>." << std::endl;
}

void fa_App::cmd_modmanager_rmmod(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		if (unparsed[0] != "base" && unparsed[0] != "core")
			modmanager.remove_mod(unparsed[0]);
		else
			std::cerr << "Attempt to remove core/base mod. Base mod can only be disabled." << std::endl;
	}
	else
		std::cerr << "Missing value for parameter <name>." << std::endl;
}

void fa_App::cmd_modmanager_rmdir(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		// TODO: uncomment the condition, when base and core mods are used in the game
		if (/*unparsed[0] != modmanager.get_mods().at("base").path.parent_path()*/true)
			modmanager.remove_mod_directory(unparsed[0]);
		else
			std::cerr << "Attempt to remove core/base mod directory. Base mod can only be disabled." << std::endl;
	}
	else
		std::cerr << "Missing value for parameter <path>." << std::endl;
}

void fa_App::cmd_modmanager_unignore(const std::vector<std::string>& args)
{
	std::map<std::string, std::string> options = {};
	std::map<std::string, bool> flags;
	std::vector<std::string> unparsed;

	parse_arguments(args, options, flags, unparsed);

	if (unparsed.size())
	{
		modmanager.remove_ignored_mod(unparsed[0], std::cerr);
	}
	else
		std::cerr << "Missing value for parameter <name>." << std::endl;
}
