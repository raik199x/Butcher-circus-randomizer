#pragma once

#include <QDialog>
#include <QPushButton>
#include <QWidget>

class HeroSelection : public QDialog {
  Q_OBJECT

public:
  explicit HeroSelection(QWidget *parent = nullptr, uint8_t numTeam = 0);
  ~HeroSelection() override;

private:
  QPushButton ***buttons;
  std::string    fileName;
  int            AccessibleHeroes;

  bool updateUiLine(const int line);

private slots:
  void ButtonClicked(void);
};
