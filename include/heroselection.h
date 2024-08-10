#pragma once

#include <QDialog>
#include <QPushButton>
#include <QWidget>

class HeroSelection : public QDialog {
  Q_OBJECT

private slots:
  void buttonClicked();

public:
  explicit HeroSelection(QWidget *parent = nullptr, uint8_t numTeam = 0);
  ~HeroSelection() override;

  static constexpr size_t kAmountOfButtonsForEachFighter = 8; // 7 spells + 1 hero
  static constexpr QSize  kIconSize                      = QSize(75, 80);
  static constexpr size_t kSpacingBetweenIcons           = 10;
  static constexpr char   kStateEnable                   = '1';
  static constexpr char   kStateDisabled                 = '0';

private:
  QPushButton ***buttons;
  std::string    fileName;
  int            AccessibleHeroes;

  bool updateUiLine(uint8_t number_of_fighter);
};
