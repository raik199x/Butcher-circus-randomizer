#include <cstddef>

#include <QBoxLayout>
#include <QMessageBox>
#include <QScrollArea>

#include "heroselection.h"
#include "config.h"

/**
 * @brief Construct a new Hero Selection:: Hero Selection object
 *
 * @param parent
 * @param numTeam
 */
HeroSelection::HeroSelection(QWidget *const parent, std::shared_ptr<RandomizeRules> player_rules) : QDialog(parent) {
  this->setWindowTitle("Random settings");
  this->setStyleSheet("background-color: #323232");

  this->rules = std::move(player_rules);

  // Init Window
  auto *main_v_layout = new QVBoxLayout(this);
  auto *scroll_area   = new QScrollArea();
  main_v_layout->addWidget(scroll_area);

  auto *widget_with_buttons = new QWidget();
  auto *layout_for_buttons  = new QVBoxLayout(widget_with_buttons);

  for (auto &button_line : this->buttons_grid) {
    auto *h_box_sub = new QHBoxLayout();

    for (auto &button : button_line) {
      button = std::make_unique<QPushButton>(new QPushButton);
      button->setFixedSize(HeroSelection::kIconSize);
      h_box_sub->addWidget(button.get());
      h_box_sub->addSpacerItem(new QSpacerItem(HeroSelection::kSpacingBetweenIcons, 0));

      connect(button.get(), SIGNAL(clicked()), this, SLOT(buttonClicked()));
    }

    layout_for_buttons->addLayout(h_box_sub);
    layout_for_buttons->addSpacerItem(new QSpacerItem(0, HeroSelection::kSpacingBetweenIcons));
  }
  scroll_area->setWidget(widget_with_buttons);

  // updating ui
  for (uint8_t index_of_fighter = 0; index_of_fighter < kTotalNumberOfFighters; index_of_fighter++) {
    this->updateUiLine(index_of_fighter);
  }

  // Calculating preferred size for window
  // +1 since does not show one button without it
  const size_t window_width = (HeroSelection::kAmountOfButtonsForEachFighter + 1) * HeroSelection::kIconSize.width() +
                              (HeroSelection::kAmountOfButtonsForEachFighter + 1) * HeroSelection::kSpacingBetweenIcons;
  // +4 to show 3 line
  const size_t window_height = static_cast<uint64_t>(HeroSelection::kIconSize.height()) * 4;
  this->resize(window_width, window_height);
}

bool HeroSelection::updateUiLine(const uint8_t index_of_fighter) {
  if (index_of_fighter >= kTotalNumberOfFighters) {
    return false;
  }

  GetterFighterResult getter_result = this->rules->getFighter(kFighters[index_of_fighter]);
  if (getter_result.code != RandomizeRulesReturnCodes::kNoError) {
    return false;
  }

  // analyzing and changing
  QString color = getter_result.fighter.is_participates ? this->kColorEnabled : this->kColorDisabled;
  QString style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" +
                  kFighters[index_of_fighter] + "/hero_" + kFighters[index_of_fighter] + ")"; // changing hero frame
  buttons_grid[index_of_fighter][kHeroStatusSwitcherButtonIndex]->setStyleSheet(style);

  uint8_t button_index = kHeroStatusSwitcherButtonIndex + 1;
  for (size_t iter_skills_state = 0; iter_skills_state < getter_result.fighter.skills.size(); iter_skills_state++) {
    color = getter_result.fighter.skills[iter_skills_state] ? kColorEnabled : kColorDisabled;
    style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" +
            kFighters[index_of_fighter] + "/" + QString::number(iter_skills_state + 1) + ".png)";
    buttons_grid[index_of_fighter][button_index]->setStyleSheet(style);
  }

  return true;
}

void HeroSelection::buttonClicked() {
  auto   *button                     = (QPushButton *)sender();
  uint8_t searched_number_of_fighter = UINT8_MAX;
  uint8_t searched_number_of_button  = UINT8_MAX;
  for (int number_of_fighter = 0; number_of_fighter < kTotalNumberOfFighters; number_of_fighter++) {
    for (int fighter_button = 0; fighter_button < HeroSelection::kAmountOfButtonsForEachFighter; fighter_button++) {

      if (this->buttons_grid[number_of_fighter][fighter_button].get() == button) {
        searched_number_of_fighter = number_of_fighter;
        searched_number_of_button  = fighter_button;
        break;
      }
    }

    // if already found
    if (searched_number_of_fighter != UINT8_MAX) {
      break;
    }
  }

  RandomizeRulesReturnCodes result_code = RandomizeRulesReturnCodes::kDefaultValue;
  if (searched_number_of_button == kHeroStatusSwitcherButtonIndex) {
    result_code = this->rules->reverseFighterRule(kFighters[searched_number_of_fighter]);
  } else {
    result_code = this->rules->reverseSkillRule(kFighters[searched_number_of_fighter], searched_number_of_button - 1);
  }

  switch (result_code) {
  case RandomizeRulesReturnCodes::kTooFewHeroes:
    QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 heroes for randomizing");
    break;
  case RandomizeRulesReturnCodes::kTooFewSpells:
    QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 spells for hero");
    break;
  case RandomizeRulesReturnCodes::kForbiddenStateChange:
    QMessageBox::warning(this, "Random settings analyze", "Abomination has all abilities");
    break;
  case RandomizeRulesReturnCodes::kNoHero: /*fallthrough*/
  default:
    break;
  }
  this->updateUiLine(searched_number_of_fighter);
}

HeroSelection::~HeroSelection() {
}
