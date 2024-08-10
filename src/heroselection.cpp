#include <cstddef>
#include <filesystem>
#include <fstream>

#include <QBoxLayout>
#include <QMessageBox>
#include <QScrollArea>

#include "heroselection.h"
#include "filemanip.h"
#include "config.h"

/**
 * @brief Construct a new Hero Selection:: Hero Selection object
 *
 * @param parent
 * @param numTeam
 */
HeroSelection::HeroSelection(QWidget *const parent, uint8_t numTeam) : QDialog(parent) {
  this->setWindowTitle("Random settings");
  this->setStyleSheet("background-color: #323232");

  this->fileName = "BCR_T" + std::to_string(numTeam + 1) + ".txt";
  // we should analyze how many heroes accessible for random
  if (!std::filesystem::exists(this->fileName)) {
    if (!recreate(this->fileName)) { // if file does not exist and cannot be created
      std::terminate();
    }
    this->AccessibleHeroes = kTotalNumberOfFighters;
  } else { // checking how many heroes enabled
    this->AccessibleHeroes = 0;
    std::ifstream file(this->fileName);
    if (!file) {
      QMessageBox::critical(this, "Cannot open file", "For some reason BCR cannot open file BCR_T(1,2).txt");
      std::terminate();
    }
    for (int i = 0; i < kTotalNumberOfFighters; i++) {
      std::string line;
      file >> line;
      if (line[line.find(':') + 1] == '1') {
        this->AccessibleHeroes++;
      }
      int spells_available = 0; // also checking that hero can random 4 spells
      for (unsigned int j = line.find(':') + 2; j < line.size(); j++) {
        if (line[j] == '1') {
          spells_available++;
        }
      }
      if (spells_available < kRequiredSpellsForFighter) {
        this->AccessibleHeroes = kTotalNumberOfFighters;
        QMessageBox::warning(this, "Random settings analyze",
                             "One of heroes had less than 4 skills for randomizing, file will be recreated");
        recreate(this->fileName);
        break;
      }
    }
    if (this->AccessibleHeroes < kRequiredNumberOfFighters) {
      QMessageBox::warning(this, "Random settings analyze",
                           "Less than 4 heroes where set for randomizing, file will be recreated");
      recreate(this->fileName);
      this->AccessibleHeroes = kTotalNumberOfFighters;
    }
  }

  this->buttons = new QPushButton **[kTotalNumberOfFighters];
  for (int i = 0; i < kTotalNumberOfFighters; i++) {
    this->buttons[i] = new QPushButton *[HeroSelection::kAmountOfButtonsForEachFighter];
  }

  // Init Window
  auto *main_v_layout = new QVBoxLayout(this);
  auto *scroll_area   = new QScrollArea();
  main_v_layout->addWidget(scroll_area);

  auto *widget_with_buttons = new QWidget();
  auto *layout_for_buttons  = new QVBoxLayout(widget_with_buttons);

  for (int i = 0; i < kTotalNumberOfFighters; i++) {
    auto *h_box_sub = new QHBoxLayout();
    for (int j = 0; j < HeroSelection::kAmountOfButtonsForEachFighter; j++) {
      this->buttons[i][j] = new QPushButton();
      buttons[i][j]->setFixedSize(HeroSelection::kIconSize);
      h_box_sub->addWidget(buttons[i][j]);
      connect(this->buttons[i][j], SIGNAL(clicked()), this, SLOT(buttonClicked()));

      h_box_sub->addSpacerItem(new QSpacerItem(HeroSelection::kSpacingBetweenIcons, 0));
    }
    layout_for_buttons->addLayout(h_box_sub);
    layout_for_buttons->addSpacerItem(new QSpacerItem(0, HeroSelection::kSpacingBetweenIcons));
  }
  scroll_area->setWidget(widget_with_buttons);

  // updating ui
  for (uint8_t index_of_fighter = 0; index_of_fighter < kTotalNumberOfFighters; index_of_fighter++) {
    this->updateUiLine(index_of_fighter + 1);
  }

  // Calculating preferred size for window
  // +1 since does not show one button without it
  const size_t window_width = (HeroSelection::kAmountOfButtonsForEachFighter + 1) * HeroSelection::kIconSize.width() +
                              (HeroSelection::kAmountOfButtonsForEachFighter + 1) * HeroSelection::kSpacingBetweenIcons;
  // +4 to show 3 line
  const size_t window_height = static_cast<long>(HeroSelection::kIconSize.height()) * 4;
  this->resize(window_width, window_height);
}

bool HeroSelection::updateUiLine(const uint8_t number_of_fighter) {
  //! \todo Check  line's borders (for out of range values)
  std::ifstream file(this->fileName);
  if (!file) {
    return false;
  }
  // getting statistic line
  std::string lines;
  for (int i = 0; i < number_of_fighter; i++) {
    getline(file, lines);
  }
  file.close();

  // analyzing and changing
  size_t  pos   = lines.find(':') + 1;
  QString color = lines[pos] == HeroSelection::kStateDisabled ? "Red;" : "Green;";
  QString style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" +
                  kFighters[number_of_fighter - 1] + "/hero_" + kFighters[number_of_fighter - 1] +
                  ")"; // changing hero frame
  buttons[number_of_fighter - 1][0]->setStyleSheet(style);

  for (unsigned int i = ++pos; i < pos + HeroSelection::kAmountOfButtonsForEachFighter - 1;
       i++) { // -1 since hero was checked before
    lines[i] == HeroSelection::kStateDisabled ? color = "Red;" : color = "Green;";
    style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" +
            kFighters[number_of_fighter - 1] + "/" + QString::number(i - pos + 1) + ".png)";
    buttons[number_of_fighter - 1][i - pos + 1]->setStyleSheet(style);
  }

  return true;
}

void HeroSelection::buttonClicked() {
  auto   *button                     = (QPushButton *)sender();
  uint8_t searched_number_of_fighter = UINT8_MAX;
  uint8_t searched_number_of_button  = UINT8_MAX;
  for (int number_of_fighter = 0; number_of_fighter < kTotalNumberOfFighters; number_of_fighter++) {
    for (int fighter_button = 0; fighter_button < HeroSelection::kAmountOfButtonsForEachFighter; fighter_button++) {
      if (this->buttons[number_of_fighter][fighter_button] != button) {
        continue;
      }
      searched_number_of_fighter = number_of_fighter;
      searched_number_of_button  = fighter_button;
    }
  }
  const auto operation_result = changeLine(this->fileName, kFighters[searched_number_of_fighter],
                                           searched_number_of_button, this->AccessibleHeroes);
  switch (operation_result) {
  case Errors::ChangeLine::HeroRemoved:
    this->AccessibleHeroes--;
    break;
  case Errors::ChangeLine::HeroAdded:
    this->AccessibleHeroes++;
    break;
  case Errors::ChangeLine::NoFile:
    QMessageBox::critical(this, "File open error", "Could not open file BCR_T(1,2)");
    std::terminate();
    break;
  case Errors::ChangeLine::TooFewHeroes:
    QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 heroes for randomizing");
    break;
  case Errors::ChangeLine::TooFewSpells:
    QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 spells for hero");
    break;
  case Errors::ChangeLine::HeroForbidden:
    QMessageBox::warning(this, "Random settings analyze", "Abomination has all abilities");
    break;
  case Errors::ChangeLine::SkillRemoved:
  default:
    break;
  }
  this->updateUiLine(searched_number_of_fighter + 1);
}

HeroSelection::~HeroSelection() {
}
