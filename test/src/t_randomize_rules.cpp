#include "t_randomize_rules.hpp"
#include "randomize_rules.hpp"

RandomizeRulesTest::RandomizeRulesTest()  = default;
RandomizeRulesTest::~RandomizeRulesTest() = default;

void RandomizeRulesTest::SetUp() {
}
void RandomizeRulesTest::TearDown() {
}

TEST_F(RandomizeRulesTest, reverseSkillRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(RandomizeRulesReturnCodes::kNoHero, test_subject.reverseSkillRule(kStubString, 0));
}

TEST_F(RandomizeRulesTest, reverseSkillRuleOutOfRange) {
  RandomizeRules test_subject;
  EXPECT_EQ(RandomizeRulesReturnCodes::kOutOfRange, test_subject.reverseSkillRule(kTestHeroName, kTotalFighterSpells));
}

TEST_F(RandomizeRulesTest, reverseSkillRuleReversingSkills) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.reverseSkillRule(kTestHeroName, iter_skills++);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.reverseSkillRule(kTestHeroName, iter_skills++);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }
}

TEST_F(RandomizeRulesTest, reverseSkillRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    test_subject.reverseSkillRule(kTestHeroName, iter_skills++);
  }

  auto result = test_subject.reverseSkillRule(kTestHeroName, kTotalFighterSpells - 1);
  EXPECT_EQ(result, RandomizeRulesReturnCodes::kTooFewSpells)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountOfEnabledSkills(kTestHeroName);
}

TEST_F(RandomizeRulesTest, setSkillRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(RandomizeRulesReturnCodes::kNoHero, test_subject.setSkillRule(kStubString, 0, true));
}

TEST_F(RandomizeRulesTest, setSkillRuleOutOfRange) {
  RandomizeRules test_subject;
  EXPECT_EQ(RandomizeRulesReturnCodes::kOutOfRange,
            test_subject.setSkillRule(kTestHeroName, kTotalFighterSpells, true));
}

TEST_F(RandomizeRulesTest, setSkillRuleSettingSkills) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.setSkillRule(kTestHeroName, iter_skills++, false);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.setSkillRule(kTestHeroName, iter_skills++, true);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }
}

TEST_F(RandomizeRulesTest, setSkillRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    test_subject.setSkillRule(kTestHeroName, iter_skills++, false);
  }

  auto result = test_subject.setSkillRule(kTestHeroName, kTotalFighterSpells - 1, false);
  EXPECT_EQ(result, RandomizeRulesReturnCodes::kTooFewSpells)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountOfEnabledSkills(kTestHeroName);
}

TEST_F(RandomizeRulesTest, reverseFighterRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(RandomizeRulesReturnCodes::kNoHero, test_subject.reverseFighterRule(kStubString));
}

TEST_F(RandomizeRulesTest, reverseFighterRuleReversingFighters) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.reverseFighterRule(kFighters[iter_fighter++]);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_fighter, test_subject.getAmountParticipates());
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.reverseFighterRule(kFighters[iter_fighter++]);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_fighter, test_subject.getAmountParticipates());
  }
}

TEST_F(RandomizeRulesTest, reverseFighterRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    test_subject.reverseFighterRule(kFighters[iter_fighter++]);
  }

  auto result = test_subject.reverseFighterRule(kFighters[kTotalNumberOfFighters - 1]);
  EXPECT_EQ(result, RandomizeRulesReturnCodes::kTooFewHeroes)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountParticipates();
}

TEST_F(RandomizeRulesTest, setFighterRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(RandomizeRulesReturnCodes::kNoHero, test_subject.setFighterRule(kStubString, false));
}

TEST_F(RandomizeRulesTest, setFighterRuleSettingFighters) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.setFighterRule(kFighters[iter_fighter++], false);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_fighter, test_subject.getAmountParticipates());
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.setFighterRule(kFighters[iter_fighter++], true);
    EXPECT_EQ(RandomizeRulesReturnCodes::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_fighter, test_subject.getAmountParticipates());
  }
}

TEST_F(RandomizeRulesTest, setFighterRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    test_subject.setFighterRule(kFighters[iter_fighter++], false);
  }

  auto result = test_subject.setFighterRule(kFighters[kTotalNumberOfFighters - 1], false);
  EXPECT_EQ(result, RandomizeRulesReturnCodes::kTooFewHeroes)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountParticipates();
}

TEST_F(RandomizeRulesTest, getterSpellsNoFighter) {
  RandomizeRules     test_subject;
  GetterSpellsResult return_value = test_subject.getHeroSpellsStates(kStubString);
  EXPECT_EQ(return_value.code, RandomizeRulesReturnCodes::kNoHero);
}

TEST_F(RandomizeRulesTest, getterSpellsNoError) {
  RandomizeRules            test_subject;
  FighterSpellsStateArray_t array;
  array.fill(true);

  GetterSpellsResult return_value = test_subject.getHeroSpellsStates(kTestHeroName);
  EXPECT_EQ(return_value.code, RandomizeRulesReturnCodes::kNoError);
  EXPECT_EQ(array, return_value.requestedFighterSpellsStates);
}

TEST_F(RandomizeRulesTest, getterFighterNoFighter) {
  RandomizeRules      test_subject;
  GetterFighterResult return_value = test_subject.getFighter(kStubString);
  EXPECT_EQ(return_value.code, RandomizeRulesReturnCodes::kNoHero);
}

TEST_F(RandomizeRulesTest, getterFighterNoError) {
  RandomizeRules     test_subject;
  GetterSpellsResult return_value = test_subject.getHeroSpellsStates(kTestHeroName);
  EXPECT_EQ(return_value.code, RandomizeRulesReturnCodes::kNoError);
}

TEST_F(RandomizeRulesTest, getterAllParticipates) {
  RandomizeRules                                     test_subject;
  std::array<std::string, kRequiredNumberOfFighters> expected_heroes;

  // Disabling some heroes
  for (uint8_t iter = kTotalNumberOfFighters; iter > kRequiredNumberOfFighters; iter--) {
    test_subject.reverseFighterRule(kFighters[iter - 1]);
  }

  // Saving enable heroes
  for (uint8_t iter = 0; iter < kRequiredNumberOfFighters; iter++) {
    expected_heroes[iter] = (std::string(kFighters[iter]));
  }

  auto return_value = test_subject.getAllParticipates();
  EXPECT_EQ(return_value.size(), kRequiredNumberOfFighters);

  // Checking that hero names are the same, but because we can't tell how exactly heroes stored, we need to iterate
  // through result
  for (const auto &expected_iter : expected_heroes) {
    bool hero_found = false;
    for (const auto &return_iter : return_value) {
      if (return_iter.fighter_name == expected_iter) {
        hero_found = true;
        break;
      }
    }
    EXPECT_TRUE(hero_found);
  }
}