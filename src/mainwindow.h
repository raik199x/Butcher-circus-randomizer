#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "random.h"
#include "randomize_rules.hpp"
#include "random_master.hpp"

class MainWindow : public QMainWindow {
  Q_OBJECT

private slots:
  void onDoRandomClicked();

  void onLevelValueChanged(int arg1);

  void onSameTeamLevelClicked();

  void onRandomSettings1Clicked();

  void onRandomSettings2Clicked();

  void onScreenShotClicked();

  void onRadioClicked();

  void onMuteAncestorClicked();

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

  bool                     playVoice;
  std::unique_ptr<QWidget> ui;

  const QSize minimum_window_size = QSize(1300, 850);
  const QSize menu_to_top_spacing = QSize(1, 800);

  std::array<QPushButton, kTotalPlayers> random_settings;
  QPushButton                           *screenShot;
  QPushButton                           *doRandom;

  QCheckBox sameTeamLevel;
  QCheckBox muteAncestor;

  QRadioButton                        radio1t;
  QRadioButton                        radio3t;
  std::array<QSpinBox, kTotalPlayers> level;

  std::unique_ptr<QVBoxLayout> layout;

  const QSize spacing_normal      = QSize(400, 1);
  const QSize spacing_competitive = QSize(200, 1);
  // Changing pointer bellow to smart pointer cause seg fault on program exit
  QSpacerItem *spacing_between_players = new QSpacerItem(this->spacing_normal.width(), this->spacing_normal.height());

  std::array<QVBoxLayout *, 2> player_layouts;

  // Methods
  void clearLayout(QLayout *layout);

  static QGridLayout *getCompetitiveTeamLayout(const squad &fighters, bool mirrored);
  static QVBoxLayout *getNormalTeamLayout(const squad &fighters, bool mirrored);
  static QLayout     *getTeamLayout(uint8_t mode, const squad &fighters, bool mirrored);

  //! \note Tricky prng
  std::unique_ptr<Random::Tricky<uint>> prng;

  // Media
  std::unique_ptr<QMediaPlayer> media_player = std::make_unique<QMediaPlayer>();
  std::unique_ptr<QAudioOutput> audio_output = std::make_unique<QAudioOutput>();
};
