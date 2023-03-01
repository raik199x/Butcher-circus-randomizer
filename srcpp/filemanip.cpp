#include "../include/filemanip.h"
#include <fstream>

#ifdef Q_OS_LINUX
#include "filemanip_linux.cpp"
#elif defined(Q_OS_WIN)
#include "filemanip_windows.cpp"
#endif

/**
 * @brief Recreates file with default settings
 *
 * @param fileName
 * @param fighters Default list of heroes
 * @return true if file was recreated
 * @return false if file wasn't recreated
 */
bool recreate(std::string fileName, std::string *fighters) {
	std::ofstream file(fileName);
	if (!file)
		return false;
	for (int i = 0; i < 18; i++) {
		std::string line = fighters[i] + ":" + std::string(8, '1') + '\n';
		file << line;
	}
	file.close();
	return true;
}

/**
 * @brief Changes line in file
 *
 * @param fileName
 * @param heroName which specific hero need to be changed
 * @param what which specific spell need to be changed
 * @param AccessableHeroes amount of heroes that can be randomized
 * @return int result code of operation
 */
int changeLine(std::string fileName, std::string heroName, int what, int AccessableHeroes) {
	int returnCode;
	if (heroName == "abomination" && what != 0)
		return -4;
	std::fstream file(fileName);
	if (!file)
		return -1;

	while (1) {
		std::string line;
		file >> line;
		if (file.eof())
			return false;
		if (line.find(heroName) != std::string::npos) {
			file.seekp(-line.size(), std::ios::cur); // moving back in file
			// finding pos of first
			size_t posStart = 0;
			while (line[posStart] != ':')
				posStart++;
			posStart++;
			if (what == 0 && AccessableHeroes == 4 && line[posStart + what] == '1')
				return -2;
			else if (what == 0 && line[posStart + what] == '1')
				returnCode = 0;
			else if (what == 0 && line[posStart + what] == '0')
				returnCode = 1;
			else
				returnCode = 3;
			int checkSpells = 0;
			for (unsigned int i = posStart + 1; i < line.length(); i++)
				if (line[i] == '1')
					checkSpells++;
			if (checkSpells == 4 && line[posStart + what] == '1' && what != 0)
				return -3;
			line[posStart + what] == '0' ? line[posStart + what] = '1' : line[posStart + what] = '0';
			line += '\n';
			file << line;
			break;
		}
	}

	return returnCode;
}

std::string *getPossibleHeroes(std::string fileName) {
	std::ofstream file(fileName);
	if (!file)
		return nullptr;
}
