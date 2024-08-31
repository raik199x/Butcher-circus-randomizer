#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>

#include "random.h"
#include "randomize_rules.hpp"
#include "random_master.hpp"

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

  std::unique_ptr<QSpacerItem> spacing_between_players;
  const QSize                  spacing_normal      = QSize(400, 1);
  const QSize                  spacing_competitive = QSize(200, 1);

  std::array<QVBoxLayout *, 2> player_layouts;

  void clearLayout(QLayout *layout);

  static QGridLayout *getCompetitiveTeamLayout(const squad &fighters, bool mirrored);
  static QVBoxLayout *getNormalTeamLayout(const squad &fighters, bool mirrored);
  static QLayout     *getTeamLayout(uint8_t mode, const squad &fighters, bool mirrored);

  //! \note Tricky prng
  Random::Tricky<uint> *prng;
};
