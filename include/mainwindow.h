#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>

#include "config.h"
#include "random.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

private slots:
  void on_doRandom_clicked();

  void on_level_valueChanged(int arg1);

  void on_sameTeamLevel_clicked();

  void on_RandomSettings1_clicked();

  void on_RandomSettings2_clicked();

  void on_screenShot_clicked();

  void on_radio_clicked();

  void on_muteAncestor_clicked();

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  static constexpr uint8_t kStandardModeAmountOfTeams    = 2;
  static constexpr uint8_t kCompetitiveModeAmountOfTeams = 6;
  static constexpr uint8_t kAmountOfTrinketsForTeam      = 8;
  static constexpr uint8_t kLeftPlayer                   = 0;
  static constexpr uint8_t kRightPlayer                  = 1;

private:
  bool     playVoice;
  QWidget *ui;

  // Recreating mainwindow.ui
  QPushButton **RandomSettings;
  QPushButton  *screenShot;
  QPushButton  *doRandom;

  QCheckBox *sameTeamLevel;
  QCheckBox *muteAncestor;

  QRadioButton *radio1t;
  QRadioButton *radio3t;
  QSpinBox    **level;

  QVBoxLayout *layout;

  QVBoxLayout *leftSide;
  QVBoxLayout *rightSide;

  void Randomizing(int numCommand);

  std::optional<std::array<QString, kRequiredNumberOfFighters>> getFighters(uint8_t player);

  static std::array<QString, kRequiredNumberOfFighters>
  getSkills(uint8_t player, const std::array<QString, kRequiredNumberOfFighters> &Fighters);

  std::array<QString, MainWindow::kAmountOfTrinketsForTeam>
  getTrinkets(uint8_t lvl, std::array<QString, kRequiredNumberOfFighters> fighters);

  void ClearLayout(QLayout *layout);

  static std::string getFileNameBasedOnPlayer(uint8_t player);

  //! \note Tricky prng
  Random::Tricky<uint> *prng;
};
