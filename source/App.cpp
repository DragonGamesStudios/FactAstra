#include "App.hpp"
#include "util.hpp"

#include <iostream>
#include <functional>
#include <regex>

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
		if (modmanager.load_configuration("__appdata__/mods/configuration.json", log).code != fa_errno::ok)
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
	// Save current config
	modmanager.save_configuration("__appdata__/mods/configuration.json", log);

	if (console_thread.joinable())
	{
		log << "Joining console thread..." << std::endl;
		console_thread.join();
	}

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
