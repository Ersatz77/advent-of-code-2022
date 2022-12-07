#ifndef AOC_2022_DAY_7_FILESYSTEM_H
#define AOC_2022_DAY_7_FILESYSTEM_H

#include <iostream>
#include <string>
#include <vector>

namespace aoc
{
	struct File;

	class Directory
	{
	public:
		Directory() = default;
		Directory(Directory* parent, const std::string& name);

		void add_directory(const std::string& name);
		void add_file(const std::string& name, const size_t size);

		Directory* parent() const { return m_parent; }
		std::string name() const { return m_name; }
		size_t size() const;

		std::vector<Directory> directories() const { return m_directories; }
		std::vector<Directory>& directories() { return m_directories; }

		std::vector<File> files() const { return m_files; }
		std::vector<File>& files() { return m_files; }

	private:
		Directory* m_parent;
		std::string m_name;
		std::vector<Directory> m_directories;
		std::vector<File> m_files;
	};

	struct File
	{
		Directory* parent;
		std::string name;
		size_t size;
	};

	std::ostream& operator<<(std::ostream& os, const Directory& dir);

} // aoc

#endif // !AOC_2022_DAY_7_FILESYSTEM_H
