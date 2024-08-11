#include <gtest/gtest.h>

#include <filesystem>

#include "config.h"
#include "filemanip.h"

using std::string;

TEST(Filemanip, Unit1) {
  constexpr static auto kTestFile = "test1.txt";
  constexpr static auto kTestDir  = "test1_dir";
  std::filesystem::create_directory(kTestDir);

  for (auto iter : {1, 2, 3, 4, 5}) {
    EXPECT_TRUE(recreate(kTestFile)) << "File should be created";
  }
  for (auto iter : {1, 2, 3, 4, 5}) {
    EXPECT_FALSE(recreate(kTestDir)) << "File should not be created because it's path is a directory";
  }

  std::filesystem::remove(kTestFile);
  std::filesystem::remove(kTestDir);
}

TEST(Filemanip, Unit2) {
  constexpr static auto kTestFile = "test2.txt";
  constexpr static auto kTestDir  = "test2_dir";
  std::filesystem::create_directory(kTestDir);

  auto dir = getPossibleHeroes(kTestDir, true);
  EXPECT_TRUE(dir.empty()) << "Directory's vector should be empty";
  EXPECT_TRUE(recreate(kTestFile)) << "File should be created";
  auto file = getPossibleHeroes(kTestFile, true);
  EXPECT_FALSE(file.empty()) << "File' vector should not be empty";
  for (auto j = 0; j < file.size(); ++j) {
    EXPECT_EQ(file[j], kFighters[j]) << "Hero " << file[j] << "should be " << kFighters[j];
  }

  std::filesystem::remove(kTestFile);
  std::filesystem::remove(kTestDir);
}