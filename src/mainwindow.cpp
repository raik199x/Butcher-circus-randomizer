#include <QAudioOutput>
#include <QClipboard>
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>
#include <QSpacerItem>
#include <QTimer>
#include <QVBoxLayout>
#include <QBitmap>
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QLayoutItem>
#include <QObject>
#include <QRadioButton>

#include <cstddef>

#include "mainwindow.h"
#include "centralwidget.h"
#include "heroselection.h"
#include "random_master.hpp"
#include "squad_competitive_widget.hpp"
#include "squad_normal_widget.hpp"

MainWindow::MainWindow(QWidget * /*parent*/) {
  //! \note Create tricky prng
  this->prng = new Random::Tricky<uint>(1U, 10U);

  this->ui = new CentralWidget(this);
  setCentralWidget(ui);

  this->playVoice = true;
  // Setting up own made ui
  this->setMinimumSize(QSize(1300, 850));
  this->ui->setStyleSheet("color: #FFFFFF;");

  this->players_randomize_rules[kLeftPlayer]  = std::make_shared<RandomizeRules>();
  this->players_randomize_rules[kRightPlayer] = std::make_shared<RandomizeRules>();

  // allocating widgets
  this->RandomSettings    = new QPushButton *[2];
  this->RandomSettings[0] = new QPushButton("Random Settings T1");
  this->RandomSettings[1] = new QPushButton("Random Settings T2");

  this->level    = new QSpinBox *[2];
  this->level[0] = new QSpinBox();
  this->level[1] = new QSpinBox();

  this->doRandom   = new QPushButton("Randomize");
  this->screenShot = new QPushButton("Screenshot");

  this->radio1t = new QRadioButton("1 Team");
  this->radio3t = new QRadioButton("3 Teams");

  this->sameTeamLevel = new QCheckBox("Same Team Level");
  this->muteAncestor  = new QCheckBox("Mute Ancestor");

  for (auto &iter : this->player_layouts) {
    iter = new QVBoxLayout();
  }

  // connecting signals
  connect(this->RandomSettings[0], SIGNAL(clicked()), this, SLOT(on_RandomSettings1_clicked()));
  connect(this->RandomSettings[1], SIGNAL(clicked()), this, SLOT(on_RandomSettings2_clicked()));
  connect(this->doRandom, SIGNAL(clicked()), this, SLOT(on_doRandom_clicked()));
  connect(this->screenShot, SIGNAL(clicked()), this, SLOT(on_screenShot_clicked()));
  connect(this->radio1t, SIGNAL(clicked()), this, SLOT(on_radio_clicked()));
  connect(this->radio3t, SIGNAL(clicked()), this, SLOT(on_radio_clicked()));
  connect(this->level[0], SIGNAL(valueChanged(int)), this, SLOT(on_level_valueChanged(int)));
  connect(this->level[1], SIGNAL(valueChanged(int)), this, SLOT(on_level_valueChanged(int)));
  connect(this->sameTeamLevel, SIGNAL(clicked()), this, SLOT(on_sameTeamLevel_clicked()));
  connect(this->muteAncestor, SIGNAL(clicked()), this, SLOT(on_muteAncestor_clicked()));

  // Designing ui
  this->RandomSettings[kLeftPlayer]->setStyleSheet("background-color: #3F3F3F;");
  this->RandomSettings[kLeftPlayer]->setFixedHeight(50);
  this->RandomSettings[kRightPlayer]->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  this->RandomSettings[kRightPlayer]->setFixedHeight(50);

  this->doRandom->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  // this->doRandom->setFixedHeight(50);
  this->screenShot->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  this->radio1t->setChecked(true);

  this->level[kLeftPlayer]->setStyleSheet("background-color: #242424; background-image: none;");
  this->level[kLeftPlayer]->setMinimum(0);
  this->level[kLeftPlayer]->setMaximum(kMaximumAccountLevel);

  this->level[kRightPlayer]->setStyleSheet("background-color: #242424; background-image: none;");
  this->level[kRightPlayer]->setMinimum(0);
  this->level[kRightPlayer]->setMaximum(kMaximumAccountLevel);

  // Managing layouts
  auto *level_setter1 = new QVBoxLayout();
  level_setter1->addWidget(new QLabel("Level"));
  level_setter1->addWidget(this->level[0]);

  auto *level_setter2 = new QVBoxLayout();
  level_setter2->addWidget(new QLabel("Level"));
  level_setter2->addWidget(this->level[1]);

  auto *top_window = new QHBoxLayout();
  top_window->addWidget(this->RandomSettings[0]);
  top_window->addLayout(level_setter1);
  top_window->addSpacerItem(new QSpacerItem(100, 1));

  auto *top_center_buttons = new QVBoxLayout();
  top_center_buttons->addWidget(this->doRandom);
  top_center_buttons->addWidget(this->screenShot);
  top_window->addLayout(top_center_buttons);

  top_window->addSpacerItem(new QSpacerItem(100, 1));
  top_window->addLayout(level_setter2);
  top_window->addWidget(this->RandomSettings[1]);

  auto *check_settings = new QVBoxLayout();
  check_settings->addWidget(this->sameTeamLevel);
  check_settings->addWidget(this->muteAncestor);

  auto *radio_settings = new QVBoxLayout();
  radio_settings->addWidget(this->radio1t);
  radio_settings->addWidget(this->radio3t);

  auto *middle = new QHBoxLayout();
  middle->addSpacerItem(new QSpacerItem(525, 1));
  middle->addLayout(check_settings);
  middle->addLayout(radio_settings);
  middle->addSpacerItem(new QSpacerItem(475, 1));

  this->spacing_between_players =
      std::make_unique<QSpacerItem>(this->spacing_normal.width(), this->spacing_normal.height());
  auto *bottom = new QHBoxLayout();
  this->player_layouts[kLeftPlayer]->addSpacerItem(
      new QSpacerItem(1, 700)); // for placing ui parts in the top of screen
  bottom->addLayout(this->player_layouts[kLeftPlayer]);
  bottom->addSpacerItem(this->spacing_between_players.get());
  bottom->addLayout(this->player_layouts[kRightPlayer]);

  this->layout = new QVBoxLayout(this->ui);
  this->layout->addLayout(top_window);
  this->layout->addLayout(middle);
  this->layout->addLayout(bottom);
}

MainWindow::~MainWindow() {
  delete this->ui;
  delete this->prng;
}

/**
 * @brief Clears the layout items, but does not deletes layout itself
 *
 * @param layout this->leftSide or this->rightSide
 */
void MainWindow::clearLayout(QLayout *layout) {
  QLayoutItem *item;
  while ((item = layout->takeAt(0)) != nullptr) {
    if (QWidget *widget = item->widget()) {
      delete widget;
    } else if (QLayout *sub_layout = item->layout()) {
      clearLayout(sub_layout);
    }
    delete item;
  }
}

QVBoxLayout *MainWindow::getNormalTeamLayout(const squad &fighters, bool mirrored) {
  auto *result_layout = new QVBoxLayout;
  for (size_t squad_iter = 0; squad_iter < fighters.size(); squad_iter++) {
    result_layout->addWidget(new SquadNormalWidget(fighters[squad_iter], squad_iter + 1, mirrored));
  }

  return result_layout;
}

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

QLayout *MainWindow::getTeamLayout(const uint8_t mode, const squad &fighters, bool mirrored) {
  switch (mode) {
  case MainWindow::kCompetitiveModeAmountOfTeams:
    return MainWindow::getCompetitiveTeamLayout(fighters, mirrored);
  case MainWindow::kStandardModeAmountOfTeams:
    return MainWindow::getNormalTeamLayout(fighters, mirrored);
  }

  return nullptr;
}

void MainWindow::on_doRandom_clicked() {
  for (auto &iter : this->player_layouts) {
    clearLayout(iter);
  }

  size_t amount_of_teams_to_generate = 0;
  if (this->radio3t->isChecked()) {
    amount_of_teams_to_generate = MainWindow::kCompetitiveModeAmountOfTeams;
  } else {
    amount_of_teams_to_generate = MainWindow::kStandardModeAmountOfTeams;
  }

  for (size_t iter_generate_team = 0; iter_generate_team < amount_of_teams_to_generate; iter_generate_team++) {
    uint8_t current_player = iter_generate_team % 2 == 0 ? MainWindow::kLeftPlayer : MainWindow::kRightPlayer;

    squad generated_squad = RandomMaster::getFullRandomizedSquad(this->players_randomize_rules[current_player],
                                                                 this->level[current_player]->value());

    this->player_layouts[current_player]->addLayout(
        MainWindow::getTeamLayout(amount_of_teams_to_generate, generated_squad, static_cast<bool>(current_player)));

    // TODO(alexander): need to find better approach
    if (iter_generate_team < amount_of_teams_to_generate - 2) {
      this->player_layouts[current_player]->addSpacerItem(new QSpacerItem(1, 40));
    }
  }

  // TODO(alexander): make do not repeat one replica two times in a row
  if (!this->muteAncestor->isChecked() && this->playVoice) {
    std::unique_ptr<QMediaPlayer> media_player = std::make_unique<QMediaPlayer>(new QMediaPlayer);
    std::unique_ptr<QAudioOutput> audio_output = std::make_unique<QAudioOutput>(new QAudioOutput);
    media_player->setAudioOutput(audio_output.get());

    //! \note Use Tricky PRNG to generate random number
    constexpr float kAudioVolume = 0.4; //! Hardcoded
    media_player->setSource(QUrl("qrc:/sounds/ancestor/" + QString::number((*prng)()) + ".wav"));
    audio_output->setVolume(kAudioVolume);
    media_player->play();
    this->playVoice = false;
  } else if (!this->muteAncestor->isChecked()) {
    this->playVoice = true;
  }
}

void MainWindow::on_level_valueChanged(int arg1) {
  if (this->sameTeamLevel->isChecked()) {
    this->level[kLeftPlayer]->setValue(arg1);
    this->level[kRightPlayer]->setValue(arg1);
  }
}

void MainWindow::on_sameTeamLevel_clicked() {
  if (this->sameTeamLevel->isChecked()) {
    this->level[kRightPlayer]->setValue(this->level[kLeftPlayer]->value());
  }
}

void MainWindow::on_RandomSettings1_clicked() {
  HeroSelection win(this, this->players_randomize_rules[kLeftPlayer]);
  win.setModal(true);
  win.exec();
}

void MainWindow::on_RandomSettings2_clicked() {
  HeroSelection win(this, this->players_randomize_rules[kRightPlayer]);
  win.setModal(true);
  win.exec();
}

void MainWindow::on_screenShot_clicked() {
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

  QTimer::singleShot(1500, [=]() { dialog->accept(); }); // Clean up dialog after1 seconds

  dialog->show();
}

void MainWindow::on_radio_clicked() {
  for (auto &iter : this->player_layouts) {
    clearLayout(iter);
  }
  this->player_layouts[kLeftPlayer]->addSpacerItem(
      new QSpacerItem(1, 700)); // for placing ui parts in the top of screen

  if (this->radio3t->isChecked()) {
    this->spacing_between_players->changeSize(this->spacing_competitive.width(), this->spacing_competitive.height());
  } else {
    this->spacing_between_players->changeSize(this->spacing_normal.width(), this->spacing_normal.height());
  }
}

void MainWindow::on_muteAncestor_clicked() {
  this->playVoice = !this->muteAncestor->isChecked();
}