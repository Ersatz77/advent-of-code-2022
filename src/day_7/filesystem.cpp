#include "day_7/filesystem.h"

#include "fmt/format.h"

#include <iostream>
#include <string>
#include <vector>

namespace aoc
{
	Directory::Directory(Directory* parent, const std::string& name) :
		m_parent(parent), m_name(name)
	{
	}

	void Directory::add_directory(const std::string& name)
	{
		m_directories.emplace_back(this, name);
	}

	void Directory::add_file(const std::string& name, const size_t size)
	{
		m_files.emplace_back(this, name, size);
	}

	size_t Directory::size() const
	{
		size_t total_size = 0;

		// Get size of directories
		for (const Directory& directory : m_directories)
		{
			total_size += directory.size();
		}

		// Get size of files
		for (const File& file : m_files)
		{
			total_size += file.size;
		}

		return total_size;
	}

	std::ostream& operator<<(std::ostream& os, const Directory& dir)
	{
		for (const auto& directory : dir.directories())
		{
			os << fmt::format("[Dir] {}\n", directory.name());
		}

		for (const auto& file : dir.files())
		{
			os << fmt::format("[File] {} Size: {}\n", file.name, file.size);
		}

		return  os;
	}

} // aoc
