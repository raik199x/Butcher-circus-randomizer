#include <gtest/gtest.h>

#include "asset_manager.hpp"

TEST(AssetManagerSingletone, ParsingJustTrinketName) {
  AssetManagerSingletone &test_subject = AssetManagerSingletone::getInstance();
  const QString           test_line    = "some_trinket";
  EXPECT_EQ(test_line, test_subject.parseTrinketName(test_line));
}

TEST(AssetManagerSingletone, ParsingTrinketNameWithRestriction) {
  AssetManagerSingletone &test_subject        = AssetManagerSingletone::getInstance();
  const QString           trinket_name        = "some_trinket";
  const QString           trinket_restriction = "[barbarian]";
  EXPECT_EQ(trinket_name, test_subject.parseTrinketName(trinket_name + trinket_restriction));
}

TEST(AssetManagerSingletone, ParsingTrinketLimitWithoutLimit) {
  AssetManagerSingletone &test_subject = AssetManagerSingletone::getInstance();
  const QString           test_line    = "some_trinket";
  EXPECT_EQ(test_subject.kTrinketWithoutHeroLimit, test_subject.parseTrinketHeroLimit(test_line));
}

TEST(AssetManagerSingletone, ParsingTrinketLimitWithLimit) {
  AssetManagerSingletone &test_subject = AssetManagerSingletone::getInstance();
  const QString           test_line    = "some_trinket[barbarian]";
  const QString           expected     = "barbarian";
  EXPECT_EQ(expected, test_subject.parseTrinketHeroLimit(test_line));
}
