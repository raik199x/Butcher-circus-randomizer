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
  EXPECT_EQ(ChangeRuleReturnCode::kNoHero, test_subject.reverseSkillRule(kStubString, 0));
}

TEST_F(RandomizeRulesTest, reverseSkillRuleOutOfRange) {
  RandomizeRules test_subject;
  EXPECT_EQ(ChangeRuleReturnCode::kOutOfRange, test_subject.reverseSkillRule(kTestHeroName, kTotalFighterSpells));
}

TEST_F(RandomizeRulesTest, reverseSkillRuleReversingSkills) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.reverseSkillRule(kTestHeroName, iter_skills++);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.reverseSkillRule(kTestHeroName, iter_skills++);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }
}

TEST_F(RandomizeRulesTest, reverseSkillRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    test_subject.reverseSkillRule(kTestHeroName, iter_skills++);
  }

  auto result = test_subject.reverseSkillRule(kTestHeroName, kTotalFighterSpells - 1);
  EXPECT_EQ(result, ChangeRuleReturnCode::kTooFewSpells)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountOfEnabledSkills(kTestHeroName);
}

TEST_F(RandomizeRulesTest, setSkillRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(ChangeRuleReturnCode::kNoHero, test_subject.setSkillRule(kStubString, 0, true));
}

TEST_F(RandomizeRulesTest, setSkillRuleOutOfRange) {
  RandomizeRules test_subject;
  EXPECT_EQ(ChangeRuleReturnCode::kOutOfRange, test_subject.setSkillRule(kTestHeroName, kTotalFighterSpells, true));
}

TEST_F(RandomizeRulesTest, setSkillRuleSettingSkills) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.setSkillRule(kTestHeroName, iter_skills++, false);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountOfEnabledSkills(kTestHeroName);
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    auto result = test_subject.setSkillRule(kTestHeroName, iter_skills++, true);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_skills, test_subject.getAmountOfEnabledSkills(kTestHeroName));
  }
}

TEST_F(RandomizeRulesTest, setSkillRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_skills = 0; iter_skills < kTotalFighterSpells - kRequiredSpellsForFighter;) {
    test_subject.setSkillRule(kTestHeroName, iter_skills++, false);
  }

  auto result = test_subject.setSkillRule(kTestHeroName, kTotalFighterSpells - 1, false);
  EXPECT_EQ(result, ChangeRuleReturnCode::kTooFewSpells)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountOfEnabledSkills(kTestHeroName);
}

TEST_F(RandomizeRulesTest, reverseFighterRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(ChangeRuleReturnCode::kNoHero, test_subject.reverseFighterRule(kStubString));
}

TEST_F(RandomizeRulesTest, reverseFighterRuleReversingFighters) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.reverseFighterRule(kFighters[iter_fighter++]);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_fighter, test_subject.getAmountParticipates());
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.reverseFighterRule(kFighters[iter_fighter++]);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_fighter, test_subject.getAmountParticipates());
  }
}

TEST_F(RandomizeRulesTest, reverseFighterRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    test_subject.reverseFighterRule(kFighters[iter_fighter++]);
  }

  auto result = test_subject.reverseFighterRule(kFighters[kTotalNumberOfFighters - 1]);
  EXPECT_EQ(result, ChangeRuleReturnCode::kTooFewHeroes)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountParticipates();
}

TEST_F(RandomizeRulesTest, setFighterRuleNoHero) {
  RandomizeRules test_subject;
  EXPECT_EQ(ChangeRuleReturnCode::kNoHero, test_subject.setFighterRule(kStubString, false));
}

TEST_F(RandomizeRulesTest, setFighterRuleSettingFighters) {
  RandomizeRules test_subject;

  // removing skills
  const uint8_t start_value_removing = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.setFighterRule(kFighters[iter_fighter++], false);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_removing - iter_fighter, test_subject.getAmountParticipates());
  }

  // enabling skills
  const uint8_t start_value_enabling = test_subject.getAmountParticipates();
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    auto result = test_subject.setFighterRule(kFighters[iter_fighter++], true);
    EXPECT_EQ(ChangeRuleReturnCode::kNoError, result);
    EXPECT_EQ(start_value_enabling + iter_fighter, test_subject.getAmountParticipates());
  }
}

TEST_F(RandomizeRulesTest, setFighterRuleLimitReached) {
  RandomizeRules test_subject;
  for (uint8_t iter_fighter = 0; iter_fighter < kTotalNumberOfFighters - kRequiredNumberOfFighters;) {
    test_subject.setFighterRule(kFighters[iter_fighter++], false);
  }

  auto result = test_subject.setFighterRule(kFighters[kTotalNumberOfFighters - 1], false);
  EXPECT_EQ(result, ChangeRuleReturnCode::kTooFewHeroes)
      << "Got: " << (int)result << " But enabled: " << test_subject.getAmountParticipates();
}
