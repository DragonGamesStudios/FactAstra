#pragma once

#include <filesystem>
#include <fstream>
#include <map>

namespace sp
{
	namespace fs = std::filesystem;

	class FileSystem
	{
	private:
		fs::path current_path;
		std::map<std::string, fs::path> path_templates;

	public:
		FileSystem(bool appdata = false);

		bool createDir(const fs::path& dirname);
		bool deleteDirRecursively(const fs::path& dirname);
		bool enterDir(const fs::path& path);
		std::string getDirectory() const;
		fs::path getCurrentPath() const;

		void exit();
		void exitTo(const fs::path& dirname);

		bool createDirIfNecessary(const fs::path& dirname);

		std::ifstream openFile(const fs::path& path) const;

		std::ofstream openOfile(const fs::path& path) const;

		bool createFile(const fs::path& filename);
		bool createFileIfNecessary(const fs::path& filename);

		bool deleteFile(const fs::path& filename);
		bool deleteFileIfExists(const fs::path& filename);

		bool exists(const fs::path& path) const;

		bool addPathTemplate(const std::string& temp, const fs::path& target);
		static bool isTemplate(const std::string& name);

		fs::path getCorrectPath(const fs::path& path) const;

		fs::directory_iterator getFilesInDirectory(const fs::path& dirname);

		bool isRegularFile(const fs::path& path) const;
		bool isDirectory(const fs::path& path) const;

		fs::path getExecutable() const;
	};
}