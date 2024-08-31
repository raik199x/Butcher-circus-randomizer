#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <QPushButton>
#include <qradiobutton.h>
#include <qspinbox.h>

#include "random.h"
#include "randomize_rules.hpp"
#include "random_master.hpp"

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

  static constexpr uint8_t kTotalPlayers = 2;
  static constexpr uint8_t kLeftPlayer   = 0;
  static constexpr uint8_t kRightPlayer  = 1;

private:
  std::array<std::shared_ptr<RandomizeRules>, kTotalPlayers> players_randomize_rules;
  RandomMaster                                               master_of_random;

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

  void            ClearLayout(QLayout *layout);
  static QWidget *getFighterWidget(uint8_t mode, const Fighter &fighter, uint8_t position, bool mirrored);

  //! \note Tricky prng
  Random::Tricky<uint> *prng;
};
