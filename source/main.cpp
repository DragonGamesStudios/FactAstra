#include "App.hpp"

#include <memory>
#include <iostream>

int main(int argc, const char** argv)
{
	std::vector<std::string> cmd_args;

	for (int i = 0; i < argc; i++)
	{
		cmd_args.push_back(argv[i]);
	}

	App app;

	app.run(cmd_args);

	app.quit();

	return 0;
}
