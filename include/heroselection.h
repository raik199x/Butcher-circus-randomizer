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

private:
  QPushButton ***buttons;
  std::string    fileName;
  int            AccessibleHeroes;

  static constexpr size_t kAmountOfButtonsForEachFighter = 8; // 7 spells + 1 hero
  static constexpr QSize  kIconSize                      = QSize(75, 80);

  bool updateUiLine(int line);
};
