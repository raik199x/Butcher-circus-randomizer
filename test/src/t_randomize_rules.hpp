#pragma once

#include <gtest/gtest.h>

#include "config.h"

constexpr auto    kStubString   = "stub";
const std::string kTestHeroName = kFighters[1];

class RandomizeRulesTest : public ::testing::Test {
public:
  RandomizeRulesTest();
  virtual ~RandomizeRulesTest();

  void SetUp() override;
  void TearDown() override;
};