#include "../include/filemanip.h"
#include "../include/config.h"
#include <QMessageBox>
#include <fstream>
#include <vector>

/**
 * @file filemanip.cpp
 * @author raik
 * @brief This file contains the functions for the file manipulation
*/

/**
 * @brief Recreates file with default settings
 *
 * @param fileName
 * @param fighters Default list of heroes
 * @return true if file was recreated
 * @return false if file wasn't recreated
 */
bool recreate(std::string fileName) {
	std::ofstream file(fileName);
	if (!file)
		return false;
	for (int i = 0; i < 18; i++) {
		//! \note something strange was here:
		//		  std::string line = fighters[i] + ":" + std::string(8, '1')['\n'];
		file << fighters[i] << ":" << std::string(8, '1') << '\n';
	}
	file.close();
	return true;
}

/**
 * @brief Changes line in file
 *
 * @param fileName
 * @param heroName which specific hero need to be changed
 * @param what which specific argument need to be changed (numeration begins from 0)
 * @param AccessibleHeroes amount of heroes that can be randomized
 * @return int result code of operation
 */
int changeLine(std::string fileName, std::string heroName, int what, int AccessibleHeroes) {
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
			if (what == 0 && AccessibleHeroes == 4 && line[posStart + what] == '1')
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

/*!
 * \brief Get the Possible Heroes function
 * 
 * \param fileName The name of the file to be parsed
 * \return std::string* List of all possible heros
 */
std::vector<std::string> getPossibleHeroes(std::string fileName) {
	std::vector<string> possibleHeroes;

	std::ifstream file(fileName);
	if (!file)
		QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
	else
		for (int i = 0; i < 18; i++) {
			string line;
			file >> line;
			if (line[line.find(":") + 1] == '1')
				possibleHeroes.push_back(fighters[i]);
		}
	return possibleHeroes;
}

/**
 * @brief Get the Skills
 *
 * @param numCommand
 * @param fighters already used fighters
 * @return QString* List of all possible skills
 */
QString *getPossibleSkills(int numCommand, QString *fighters) {
	using namespace std;
	vector<string> possibleSkills;
	QString *result;

	ifstream file(numCommand == 0 ? "BCR_T1.txt" : "BCR_T2.txt");
	if (!file) {
		QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
		return nullptr;
	}
	for (int i = 0; i < 18; i++) {
		string line;
		file >> line;
		if (line[line.find(":") + 1] == '1') {
			string skills;
			for (unsigned int i = line.find(":") + 2; i < line.size(); i++)
				skills += line[i];
			//! \note saving hero's skills
			possibleSkills.push_back(skills);
		}
	}
	return result;
}
