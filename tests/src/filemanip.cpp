#include <criterion/criterion.h>
#include <filesystem>

#include "../../include/config.h"
#include "../../include/filemanip.h"

using std::string;

Test(Filemanip, Unit1, .disabled = false) {
	constexpr static const auto TEST_FILE = "test1.txt";
	constexpr static const auto TEST_DIR = "test1_dir";
	std::filesystem::create_directory(TEST_DIR);

	for (auto i : {1, 2, 3, 4, 5})
		cr_expect(recreate(TEST_FILE), "File should be created");
	for (auto i : {1, 2, 3, 4, 5})
		cr_expect(!recreate(TEST_DIR), "File should not be created because it's path is a directory");

	std::filesystem::remove(TEST_FILE);
	std::filesystem::remove(TEST_DIR);
}

Test(Filemanip, Unit2, .disabled = false) {
	constexpr static const auto TEST_FILE = "test2.txt";
	constexpr static const auto TEST_DIR = "test2_dir";
	std::filesystem::create_directory(TEST_DIR);

	auto dir = getPossibleHeroes(TEST_DIR, true);
	cr_expect(dir.empty(), "Directory's vector should be empty");
	cr_expect(recreate(TEST_FILE), "File should be created");
	auto file = getPossibleHeroes(TEST_FILE, true);
	cr_expect(!file.empty(), "File' vector should not be empty");
	for (auto j = 0; j < file.size(); ++j)
		cr_expect(file[j] == fighters[j], "Hero %s should be %s", file[j].c_str(), fighters[j]);

	std::filesystem::remove(TEST_FILE);
	std::filesystem::remove(TEST_DIR);
}

Test(Filemanip, Unit3, .disabled = false) {
	constexpr static const auto TEST_FILE = "test3.txt";
	constexpr static const auto TEST_DIR = "test3_dir";
	std::filesystem::create_directory(TEST_DIR);

	using namespace Errors;
	cr_expect(changeLine(TEST_DIR, "", 0, 4) == ChangeLine::NoFile, "Should return NoFile, because file doesn't exist");
	cr_expect(recreate(TEST_FILE), "File should be created");
	cr_expect(changeLine(TEST_FILE, "", 0, 0) == ChangeLine::InvalidHeroesAmount, "Valid heroes amount should have been detected");
	auto heroes = getPossibleHeroes(TEST_FILE, true);
	auto amount = heroes.size();
	for (auto i = 0; i < amount; ++i)
		cr_expect(heroes[i] == fighters[i], "Hero %s should be %s", heroes[i].c_str(), fighters[i]);
	cr_expect(changeLine(TEST_FILE, "non_existing_hero", 0, amount) == ChangeLine::NoHero, "Should return NoHero, because such hero doesn't exist");
	cr_expect(changeLine(TEST_FILE, "abomination", 0, amount--) == ChangeLine::HeroRemoved, "Should return HeroRemoved, because 'abomination' can be removed");
	cr_expect(changeLine(TEST_FILE, "abomination", 0, amount++) == ChangeLine::HeroAdded, "Should return HeroAdded, because 'abomination' was removed and can be added again");
	cr_expect(changeLine(TEST_FILE, "abomination", 0, amount--) == ChangeLine::HeroRemoved, "Should return HeroRemoved, because 'abomination' can be removed");
	cr_expect(changeLine(TEST_FILE, "abomination", 1, amount) == ChangeLine::HeroForbidden, "Should return HeroForbidden, because 'abomination's' skills cannot be modified");
	
	heroes = getPossibleHeroes(TEST_FILE, true);
	auto new_amount = heroes.size();
	cr_expect(new_amount == amount, "Amount of heroes should be %zu, but is %zu", amount, new_amount);
	for (auto i = 0; i < new_amount; ++i)
		cr_expect(heroes[i] == fighters[i+1], "Hero %s should be %s", heroes[i].c_str(), fighters[i]);

	cr_expect(changeLine(TEST_FILE, "antiquarian", 0, new_amount--) == ChangeLine::HeroRemoved, "Should return HeroRemoved, because 'antiquarian' can be removed");
	cr_expect(changeLine(TEST_FILE, "antiquarian", 0, new_amount++) == ChangeLine::HeroAdded, "Should return HeroAdded, because 'antiquarian' was removed and can be added again");
	for (auto i : {1, 2, 3})
		cr_expect(changeLine(TEST_FILE, "antiquarian", i, new_amount) == ChangeLine::SkillRemoved, "Should return SkillRemoved, because 'antiquarian's' skill can be removed");
	for (auto i : {4, 5, 6, 7})
		cr_expect(changeLine(TEST_FILE, "antiquarian", i, new_amount) == ChangeLine::TooFewSpells, "Should return TooFewSpells, because 'antiquarian's' skill cannot be removed");
	for (auto i : {8, 9, 10, 11})
		cr_expect(changeLine(TEST_FILE, "antiquarian", i, new_amount) == ChangeLine::OutOfRange, "Should return OutOfRange, because of an invalid skill index");
	
	cr_expect(changeLine(TEST_FILE, "abomination", 0, amount++) == ChangeLine::HeroAdded, "Should return HeroAdded, because 'abomination' was removed and can be added again");

	for (auto j : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13})
		cr_expect(changeLine(TEST_FILE, fighters[j], 0, amount--) == ChangeLine::HeroRemoved, "Should return HeroRemoved, because '%s' can be removed", fighters[j]);
	for (auto j : {14, 15, 16, 17})
		cr_expect(changeLine(TEST_FILE, fighters[j], 0, amount) == ChangeLine::TooFewHeroes, "Should return TooFewHeroes, because '%s' cannot be removed", fighters[j]);

	std::filesystem::remove(TEST_FILE);
	std::filesystem::remove(TEST_DIR);
}


Test(Filemanip, Unit4) {
	constexpr static const auto TEST_FILE = "BCR_T1.txt";
	constexpr static const auto TEST_DIR = "BCR_T1_dir";
	cr_expect(recreate(TEST_FILE), "File should be created");
	std::filesystem::create_directory(TEST_DIR);

	QString qt_fighters[] = {fighters[0], fighters[1], fighters[2], fighters[3]};
	const std::array<std::string, 4> std_fighters = {fighters[0], fighters[1], fighters[2], fighters[3]};

	auto qt_possibleSkills = d_getPossibleSkills(0, qt_fighters, true);
	auto std_possibleSkills = getPossibleSkills(TEST_FILE, std_fighters, true);
	for (auto i : {0, 1, 2, 3}) {
		auto skill_depr = qt_possibleSkills[i].toStdString();
		auto skill_std = std_possibleSkills[i].toStdString();
		cr_expect(skill_depr == skill_std, "%s should be the same as %s", skill_depr.c_str(), skill_std.c_str());
	}

	std::filesystem::remove(TEST_FILE);
	std::filesystem::remove(TEST_DIR);
}
