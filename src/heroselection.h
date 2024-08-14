#pragma once

#include <QDialog>
#include <QPushButton>
#include <QWidget>

#include "randomize_rules.hpp"

class HeroSelection : public QDialog {
  Q_OBJECT

private slots:
  void buttonClicked();

public:
  explicit HeroSelection(QWidget *parent, std::shared_ptr<RandomizeRules> player_rules);
  ~HeroSelection() override;

  static constexpr size_t  kAmountOfButtonsForEachFighter = 8; // 7 spells + 1 hero
  static constexpr uint8_t kHeroStatusSwitcherButtonIndex = 0;
  static constexpr QSize   kIconSize                      = QSize(75, 80);
  static constexpr size_t  kSpacingBetweenIcons           = 10;

private:
  const QString kColorEnabled  = "Green;";
  const QString kColorDisabled = "Red";

  std::array<std::array<std::unique_ptr<QPushButton>, kAmountOfButtonsForEachFighter>, kTotalNumberOfFighters>
      buttons_grid;
  //  QPushButton                  ***buttons;
  std::shared_ptr<RandomizeRules> rules;

  bool updateUiLine(uint8_t index_of_fighter);
};
