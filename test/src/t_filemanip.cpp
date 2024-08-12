
#include <filesystem>
#include <fstream>

#include "filemanip.h"
#include "heroselection.h"
#include "t_filemanip.hpp"

FileIterationTestWithFolderCreation::FileIterationTestWithFolderCreation()  = default;
FileIterationTestWithFolderCreation::~FileIterationTestWithFolderCreation() = default;

void FileIterationTestWithFolderCreation::SetUp() {
  std::filesystem::create_directory(FileIterationTestWithFolderCreation::kTestDirName);
}

void FileIterationTestWithFolderCreation::TearDown() {
  std::filesystem::remove_all(FileIterationTestWithFolderCreation::kTestDirName);
}

std::string FileIterationTestWithFolderCreation::getFileContentRaw(const std::string &path_to_file) {
  std::ifstream file(path_to_file, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Could not open file " + path_to_file);
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

TEST(FileIterationTest, BaseContentGenerator) {
  std::string result = getBaseFileContent();
  EXPECT_EQ(result, FileIterationTestWithFolderCreation::kRawBaseFile);
}

TEST(FileIterationTest, CreatingFileInvalidDirectory) {
  const std::string invalid_path =
      std::string(kStubString) + "/" + std::string(FileIterationTestWithFolderCreation::kTestFileName);
  EXPECT_FALSE(recreate(invalid_path));
}

TEST_F(FileIterationTestWithFolderCreation, CreatingFileInEmptyDir) {
  EXPECT_TRUE(recreate(this->fullTestPath));

  // Checking file integrity
  EXPECT_EQ(FileIterationTestWithFolderCreation::kRawBaseFile,
            FileIterationTestWithFolderCreation::getFileContentRaw(this->fullTestPath));
}

TEST_F(FileIterationTestWithFolderCreation, CreateWithRewritingExistingFile) {
  EXPECT_TRUE(recreate(this->fullTestPath));
  EXPECT_TRUE(recreate(this->fullTestPath));

  EXPECT_EQ(FileIterationTestWithFolderCreation::kRawBaseFile,
            FileIterationTestWithFolderCreation::getFileContentRaw(this->fullTestPath));
}

TEST(FileIterationTest, ChangeLineInvalidButtonIndex) {
  Errors::ChangeLine result = changeLine(kStubString, kStubString, kInvalidButtonIndex, kValidAccessibleHeros);
  EXPECT_EQ(result, Errors::ChangeLine::OutOfRange) << "Got: " << int(result);
}

TEST(FileIterationTest, ChangeLineInvalidAccessibleHeroes) {
  Errors::ChangeLine result = changeLine(kStubString, kStubString, kValidButtonIndex, kInvalidAccessibleHeroes);
  EXPECT_EQ(result, Errors::ChangeLine::InvalidHeroesAmount) << "Got: " << int(result);
}

TEST(FileIterationTest, ChangeLineSpecialHero) {
  Errors::ChangeLine result =
      changeLine(kStubString, HeroSelection::kHeroWithAllSkills, kValidButtonIndex, kValidAccessibleHeros);
  EXPECT_EQ(result, Errors::ChangeLine::HeroForbidden) << "Got: " << int(result);
}

TEST(FileIterationTest, ChangeLineInvalidFile) {
  Errors::ChangeLine result = changeLine(kStubString, kStubString, kValidButtonIndex, kValidAccessibleHeros);
  EXPECT_EQ(result, Errors::ChangeLine::NoFile) << "Got: " << int(result);
}

TEST_F(FileIterationTestWithFolderCreation, ChangeLineInvalidHero) {
  recreate(this->fullTestPath);
  Errors::ChangeLine result = changeLine(this->fullTestPath, kStubString, kValidButtonIndex, kValidAccessibleHeros);
  EXPECT_EQ(result, Errors::ChangeLine::NoHero) << "Got: " << int(result);
}

TEST_F(FileIterationTestWithFolderCreation, ChangeLineRemovingHeroLimit) {
  recreate(this->fullTestPath);
  Errors::ChangeLine result = changeLine(this->fullTestPath, kTestHeroName,
                                         HeroSelection::kHeroStatusSwitcherButtonIndex, kRequiredNumberOfFighters);
  EXPECT_EQ(result, Errors::ChangeLine::TooFewHeroes) << "Got: " << int(result);
}