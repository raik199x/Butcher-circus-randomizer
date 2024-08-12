#pragma once

#include <gtest/gtest.h>

#include "config.h"

constexpr auto    kStubString              = "stub";
constexpr uint8_t kInvalidButtonIndex      = -1;
constexpr uint8_t kValidButtonIndex        = 1;
constexpr uint8_t kInvalidAccessibleHeroes = 2;
constexpr uint8_t kValidAccessibleHeros    = 5;
const std::string kTestHeroName            = kFighters[1];

class FileIterationTestWithFolderCreation : public ::testing::Test {
public:
  FileIterationTestWithFolderCreation();
  virtual ~FileIterationTestWithFolderCreation();

  void SetUp() override;
  void TearDown() override;

  static std::string getFileContentRaw(const std::string &path_to_file);

  static constexpr auto kTestDirName  = "file_iteration_test_folder/";
  static constexpr auto kTestFileName = "test_file.txt";
  const std::string     fullTestPath  = std::string(kTestDirName) + std::string(kTestFileName);
  static constexpr auto kRawBaseFile  = R"(abomination:11111111
antiquarian:11111111
bounty_hunter:11111111
crusader:11111111
arbalest:11111111
flagellant:11111111
grave_robber:11111111
hellion:11111111
highwayman:11111111
houndmaster:11111111
jester:11111111
leper:11111111
man_at_arms:11111111
musketeer:11111111
occultist:11111111
plague_doctor:11111111
shieldbreaker:11111111
vestal:11111111
)";
};