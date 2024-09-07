#include <QClipboard>
#include <QFile>
#include <QGuiApplication>
#include <QMessageBox>
#include <QScreen>
#include <QTimer>
#include <QDialog>
#include <QLayoutItem>
#include <QObject>
#include <QAudioOutput>

#include <cstddef>

#include "mainwindow.h"
#include "centralwidget.h"
#include "heroselection.h"
#include "random_master.hpp"
#include "squad_competitive_widget.hpp"
#include "squad_normal_widget.hpp"

MainWindow::MainWindow(QWidget * /*parent*/) {
  this->prng = std::make_unique<Random::Tricky<uint>>(1U, kTotalAncestorReplicas);

  this->ui = std::make_unique<CentralWidget>(this);
  setCentralWidget(ui.get());

  this->playVoice = true;
  this->media_player->setAudioOutput(this->audio_output.get());

  this->setMinimumSize(this->minimum_window_size);
  this->ui->setStyleSheet("color: #FFFFFF;");

  this->players_randomize_rules[kLeftPlayer]  = std::make_shared<RandomizeRules>();
  this->players_randomize_rules[kRightPlayer] = std::make_shared<RandomizeRules>();

  this->doRandom   = new QPushButton("Randomize", this);
  this->screenShot = new QPushButton("Screenshot", this);

  this->radio1t.setText("1 Team");
  this->radio3t.setText("3 Teams");

  this->sameTeamLevel.setText("Same Team Level");
  this->muteAncestor.setText("Mute Ancestor");

  for (auto &iter : this->player_layouts) {
    iter = new QVBoxLayout();
  }

  // connecting signals
  connect(&this->random_settings[kLeftPlayer], SIGNAL(clicked()), this, SLOT(onRandomSettings1Clicked()));
  connect(&this->random_settings[kRightPlayer], SIGNAL(clicked()), this, SLOT(onRandomSettings2Clicked()));
  connect(this->doRandom, SIGNAL(clicked()), this, SLOT(onDoRandomClicked()));
  connect(this->screenShot, SIGNAL(clicked()), this, SLOT(onScreenShotClicked()));
  connect(&this->radio1t, SIGNAL(clicked()), this, SLOT(onRadioClicked()));
  connect(&this->radio3t, SIGNAL(clicked()), this, SLOT(onRadioClicked()));
  connect(&this->sameTeamLevel, SIGNAL(clicked()), this, SLOT(onSameTeamLevelClicked()));
  connect(&this->muteAncestor, SIGNAL(clicked()), this, SLOT(onMuteAncestorClicked()));
  for (auto &iter : this->level) {
    iter.setStyleSheet("background-color: #242424; background-image: none;");
    iter.setMinimum(0);
    iter.setMaximum(kMaximumAccountLevel);
    connect(&iter, SIGNAL(valueChanged(int)), this, SLOT(onLevelValueChanged(int)));
  }

  // Designing ui
  for (size_t iter = 0; iter < this->random_settings.size(); iter++) {
    constexpr uint8_t kButtonHeight = 50;
    this->random_settings[iter].setText("Random Settings T" + QString::number(iter + 1));
    this->random_settings[iter].setStyleSheet("background-color: #3F3F3F;");
    this->random_settings[iter].setFixedHeight(kButtonHeight);
  }

  this->doRandom->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  this->screenShot->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  this->radio1t.setChecked(true);

  // Managing layouts
  auto *level_setter1 = new QVBoxLayout();
  level_setter1->addWidget(new QLabel("Level"));
  level_setter1->addWidget(&this->level[kLeftPlayer]);

  auto *level_setter2 = new QVBoxLayout();
  level_setter2->addWidget(new QLabel("Level"));
  level_setter2->addWidget(&this->level[kRightPlayer]);

  constexpr unsigned kTopWindowMenuSpacer = 100;
  auto              *top_window           = new QHBoxLayout();
  top_window->addWidget(&this->random_settings[kLeftPlayer]);
  top_window->addLayout(level_setter1);
  top_window->addSpacerItem(new QSpacerItem(kTopWindowMenuSpacer, 1));

  auto *top_center_buttons = new QVBoxLayout();
  top_center_buttons->addWidget(this->doRandom);
  top_center_buttons->addWidget(this->screenShot);
  top_window->addLayout(top_center_buttons);

  top_window->addSpacerItem(new QSpacerItem(kTopWindowMenuSpacer, 1));
  top_window->addLayout(level_setter2);
  top_window->addWidget(&this->random_settings[kRightPlayer]);

  auto *check_settings = new QVBoxLayout();
  check_settings->addWidget(&this->sameTeamLevel);
  check_settings->addWidget(&this->muteAncestor);

  auto *radio_settings = new QVBoxLayout();
  radio_settings->addWidget(&this->radio1t);
  radio_settings->addWidget(&this->radio3t);

  auto              *middle           = new QHBoxLayout();
  constexpr unsigned kLeftMenuSpacer  = 525;
  constexpr unsigned kRightMenuSpacer = 475;
  middle->addSpacerItem(new QSpacerItem(kLeftMenuSpacer, 1));
  middle->addLayout(check_settings);
  middle->addLayout(radio_settings);
  middle->addSpacerItem(new QSpacerItem(kRightMenuSpacer, 1));

  auto *bottom = new QHBoxLayout();
  this->player_layouts[kLeftPlayer]->addSpacerItem(
      new QSpacerItem(this->menu_to_top_spacing.width(),
                      this->menu_to_top_spacing.height())); // for placing ui parts in the top of screen
  bottom->addLayout(this->player_layouts[kLeftPlayer]);
  bottom->addSpacerItem(this->spacing_between_players);
  bottom->addLayout(this->player_layouts[kRightPlayer]);

  this->layout = std::make_unique<QVBoxLayout>(this->ui.get());
  this->layout->addLayout(top_window);
  this->layout->addLayout(middle);
  this->layout->addLayout(bottom);
}

MainWindow::~MainWindow() = default;

/**
 * @brief Clears the layout items, but does not deletes layout itself
 *
 * @param layout this->leftSide or this->rightSide
 */
void MainWindow::clearLayout(QLayout *layout) {
  QLayoutItem *item;
  while ((item = layout->takeAt(0)) != nullptr) {
    QWidget *widget = item->widget();
    delete widget;

    if (QLayout *sub_layout = item->layout(); sub_layout != nullptr) {
      clearLayout(sub_layout);
    }
    delete item;
  }
}

/**
 * @brief Return layout containing whole squad based on NormalTeam widget
 *
 * @param fighters randomized squad of fighter
 * @param mirrored should widgets be mirrored?
 * @return QVBoxLayout* pointer to a new created layout
 */
QVBoxLayout *MainWindow::getNormalTeamLayout(const squad &fighters, bool mirrored) {
  auto *result_layout = new QVBoxLayout;
  for (size_t squad_iter = 0; squad_iter < fighters.size(); squad_iter++) {
    result_layout->addWidget(new SquadNormalWidget(fighters[squad_iter], squad_iter + 1, mirrored));
  }

  return result_layout;
}

/**
 * @brief Return layout containing whole squad based on CompetitiveTeam widget
 *
 * @param fighters randomized squad of fighter
 * @param mirrored should widgets be mirrored?
 * @return QGridLayout* pointer to a new created layout
 */
QGridLayout *MainWindow::getCompetitiveTeamLayout(const squad &fighters, bool mirrored) {
  auto *result_layout = new QGridLayout;
  for (uint8_t row = 0; row < kRequiredNumberOfFighters / 2; row++) {
    for (uint8_t column = 0; column < kRequiredNumberOfFighters / 2; column++) {
      const uint8_t fighter_index = row * (kRequiredNumberOfFighters / 2) + column;
      result_layout->addWidget(new SquadCompetitiveWidget(fighters[fighter_index], fighter_index + 1, mirrored), row,
                               column);
    }
  }

  return result_layout;
}

/**
 * @brief Returns team layout. Determines which team widget to use using mode.
 *
 * @param mode Decides which team widget should be used
 * @param fighters squad of randomized fighters
 * @param mirrored Should widget me mirrored?
 * @return QLayout* Team layout containing fighters
 */
QLayout *MainWindow::getTeamLayout(const uint8_t mode, const squad &fighters, bool mirrored) {
  switch (mode) {
  case MainWindow::kCompetitiveModeAmountOfTeams:
    return MainWindow::getCompetitiveTeamLayout(fighters, mirrored);
  case MainWindow::kStandardModeAmountOfTeams:
    return MainWindow::getNormalTeamLayout(fighters, mirrored);
  }

  return nullptr;
}

/**
 * @brief Slot that call s randomization function and show result to a user.
 *
 */
void MainWindow::onDoRandomClicked() {
  for (auto &iter : this->player_layouts) {
    clearLayout(iter);
  }

  size_t amount_of_teams_to_generate = 0;
  if (this->radio3t.isChecked()) {
    amount_of_teams_to_generate = MainWindow::kCompetitiveModeAmountOfTeams;
  } else {
    amount_of_teams_to_generate = MainWindow::kStandardModeAmountOfTeams;
  }

  for (size_t iter_generate_team = 0; iter_generate_team < amount_of_teams_to_generate; iter_generate_team++) {
    uint8_t current_player = iter_generate_team % 2 == 0 ? MainWindow::kLeftPlayer : MainWindow::kRightPlayer;

    squad generated_squad = RandomMaster::getFullRandomizedSquad(this->players_randomize_rules[current_player],
                                                                 this->level[current_player].value());

    this->player_layouts[current_player]->addLayout(
        MainWindow::getTeamLayout(amount_of_teams_to_generate, generated_squad, static_cast<bool>(current_player)));

    // TODO(alexander): need to find better approach
    if (iter_generate_team < amount_of_teams_to_generate - 2) {
      constexpr uint8_t kCompetitiveTeamSpacer = 40;
      this->player_layouts[current_player]->addSpacerItem(new QSpacerItem(1, kCompetitiveTeamSpacer));
    }
  }

  // TODO(alexander): make do not repeat one replica two times in a row
  if (!this->muteAncestor.isChecked() && this->playVoice) {

    //! \note Use Tricky PRNG to generate random number
    constexpr float kAudioVolume = 0.4; //! Hardcoded
    media_player->setSource(QUrl("qrc:/sounds/ancestor/" + QString::number((*prng)()) + ".wav"));
    audio_output->setVolume(kAudioVolume);
    media_player->play();
    this->playVoice = false;
  } else if (!this->muteAncestor.isChecked()) {
    this->playVoice = true;
  }
}

/**
 * @brief Level value change handler
 *
 * @param arg1 new level value
 */
void MainWindow::onLevelValueChanged(int arg1) {
  if (this->sameTeamLevel.isChecked()) {
    this->level[kLeftPlayer].setValue(arg1);
    this->level[kRightPlayer].setValue(arg1);
  }
}

/**
 * @brief Handler for same team level checkbox
 *
 */
void MainWindow::onSameTeamLevelClicked() {
  if (this->sameTeamLevel.isChecked()) {
    this->level[kRightPlayer].setValue(this->level[kLeftPlayer].value());
  }
}

/**
 * @brief Handler for calling random setting for team 1
 *
 */
void MainWindow::onRandomSettings1Clicked() {
  HeroSelection win(this, this->players_randomize_rules[kLeftPlayer]);
  win.setModal(true);
  win.exec();
}

/**
 * @brief Handler for calling random setting for team 2
 *
 */
void MainWindow::onRandomSettings2Clicked() {
  HeroSelection win(this, this->players_randomize_rules[kRightPlayer]);
  win.setModal(true);
  win.exec();
}

/**
 * @brief Handler for button "screenshot" click
 *
 * Creates pixmap that contains current state (view) of windows and save it in clipboard.
 * Also triggers appearing of created screenshot in down right corner.
 *
 */
void MainWindow::onScreenShotClicked() {
  QPixmap     screenshot = this->grab(); // take screenshot
  QClipboard *clipboard  = QGuiApplication::clipboard();
  clipboard->setPixmap(screenshot);

  auto *dialog = new QDialog(this);

  // pasting screenshot into window
  constexpr size_t kImageSize       = 700;
  auto            *screenshot_label = new QLabel(dialog);
  screenshot_label->setPixmap(screenshot.scaled(kImageSize, kImageSize, Qt::KeepAspectRatio));
  QRect screen_geometry = screen()->geometry();

  // setting up window
  dialog->setStyleSheet("color: #FFFFFF;");
  dialog->setGeometry(screen_geometry.width(), screen_geometry.height(), screenshot_label->pixmap().width(),
                      screenshot_label->pixmap().height());
  dialog->setWindowTitle("Saved to clipboard");
  dialog->setModal(true);

  // Setting windows geometry (win os issue fix)
  const int dialog_width  = screenshot_label->pixmap().width();
  const int dialog_height = screenshot_label->pixmap().height();
  const int x_cord        = screen_geometry.width() - dialog_width - 20;   // offset by 20 pixels from the right edge
  const int y_cord        = screen_geometry.height() - dialog_height - 20; // offset by 20 pixels from the bottom edge
  dialog->setGeometry(x_cord, y_cord, dialog_width, dialog_height);

  dialog->show();

  constexpr unsigned kShowTime = 1500;
  QTimer::singleShot(kShowTime, [=]() {
    dialog->accept();
    delete screenshot_label;
    delete dialog;
  }); // Clean up dialog after1 seconds
}

/**
 * @brief Handler for radio buttons
 *
 */
void MainWindow::onRadioClicked() {
  for (auto &iter : this->player_layouts) {
    clearLayout(iter);
  }
  this->player_layouts[kLeftPlayer]->addSpacerItem(
      new QSpacerItem(this->menu_to_top_spacing.width(),
                      this->menu_to_top_spacing.height())); // for placing ui parts in the top of screen

  if (this->radio3t.isChecked()) {
    this->spacing_between_players->changeSize(this->spacing_competitive.width(), this->spacing_competitive.height());
  } else {
    this->spacing_between_players->changeSize(this->spacing_normal.width(), this->spacing_normal.height());
  }
}

/**
 * @brief Handler for muting ancestor checkbox
 *
 */
void MainWindow::onMuteAncestorClicked() {
  this->playVoice = !this->muteAncestor.isChecked();
}