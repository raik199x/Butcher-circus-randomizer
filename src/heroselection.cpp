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
  // this->setFixedSize(1065, 342);
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
  auto *scroll_area = new QScrollArea();
  main_v_layout->addWidget(scroll_area);

  auto *widget_with_buttons = new QWidget();
  auto *layout_for_buttons = new QVBoxLayout(widget_with_buttons);

  for (int i = 0; i < kTotalNumberOfFighters; i++) {
    auto *h_box_sub = new QHBoxLayout();
    for (int j = 0; j < HeroSelection::kAmountOfButtonsForEachFighter; j++) {
      this->buttons[i][j] = new QPushButton();
      buttons[i][j]->setFixedSize(HeroSelection::kIconSize);
      h_box_sub->addWidget(buttons[i][j]);
      connect(this->buttons[i][j], SIGNAL(clicked()), this, SLOT(buttonClicked()));
    }
    layout_for_buttons->addLayout(h_box_sub);
  }
  scroll_area->setWidget(widget_with_buttons);

  // updating ui
  for (int i = 0; i < kTotalNumberOfFighters; i++) {
    updateUiLine(i + 1);
  }
}

bool HeroSelection::updateUiLine(const int line) {
  //! \todo Check  line's borders (for out of range values)
  std::ifstream file(this->fileName);
  if (!file) {
    return false;
  }
  // getting statistic line
  std::string lines;
  for (int i = 0; i < line; i++) {
    getline(file, lines);
  }
  file.close();

  // analyzing and changing
  size_t pos = lines.find(':');
  pos++;
  std::string color;
  lines[pos] == '0' ? color = "Red;" : color = "Green;";
  std::string style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" +
                      kFighters[line - 1] + "/hero_" + kFighters[line - 1] + ")"; // changing hero frame
  buttons[line - 1][0]->setStyleSheet(QString::fromStdString(style));
  pos++;
  for (unsigned int i = pos; i < pos + 7; i++) { // 7 since hero was checked before
    lines[i] == '0' ? color = "Red;" : color = "Green;";
    style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" + kFighters[line - 1] +
            "/" + std::to_string(i - pos + 1) + ".png)";
    buttons[line - 1][i - pos + 1]->setStyleSheet(QString::fromStdString(style));
  }
  return true;
}

void HeroSelection::buttonClicked() {
  using namespace Errors;
  auto *button = (QPushButton *)sender();
  for (int x = 0; x < kTotalNumberOfFighters; x++) {
    for (int c = 0; c < 8; c++) {
      if (this->buttons[x][c] == button) {
        switch (changeLine(this->fileName, kFighters[x], c, this->AccessibleHeroes)) {
        case ChangeLine::HeroRemoved:
          this->AccessibleHeroes--;
          break;
        case ChangeLine::HeroAdded:
          this->AccessibleHeroes++;
          break;
        case ChangeLine::NoFile:
          QMessageBox::critical(this, "File open error", "Could not open file BCR_T(1,2)");
          std::terminate();
          break;
        case ChangeLine::TooFewHeroes:
          QMessageBox::warning(this, "Random settings analyze",
                               "You are trying to set less than 4 heroes for randomizing");
          break;
        case ChangeLine::TooFewSpells:
          QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 spells for hero");
          break;
        case ChangeLine::HeroForbidden:
          QMessageBox::warning(this, "Random settings analyze", "B.r.u.h.");
          break;
        case ChangeLine::SkillRemoved:
        default:
          break;
        }
        updateUiLine(x + 1);
        return;
      }
    }
  }
}

HeroSelection::~HeroSelection() {
}
