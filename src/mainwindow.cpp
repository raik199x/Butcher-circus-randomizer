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
#include "squad_normal_widget.hpp"

MainWindow::MainWindow(QWidget * /*parent*/) {
  //! \note Create tricky prng
  this->prng = new Random::Tricky<uint>(1U, 10U);

  this->ui = new CentralWidget(this);
  setCentralWidget(ui);

  this->playVoice = true;
  // Setting up own made ui
  // this->setFixedSize(QSize(1192, 665));
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

  this->leftSide  = new QVBoxLayout();
  this->rightSide = new QVBoxLayout();

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
  this->RandomSettings[0]->setStyleSheet("background-color: #3F3F3F;");
  this->RandomSettings[0]->setFixedHeight(50);
  this->RandomSettings[1]->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  this->RandomSettings[1]->setFixedHeight(50);

  this->doRandom->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  // this->doRandom->setFixedHeight(50);
  this->screenShot->setStyleSheet("background-color: #3F3F3F; background-image: none;");
  this->radio1t->setChecked(true);

  this->level[0]->setStyleSheet("background-color: #242424; background-image: none;");
  this->level[1]->setStyleSheet("background-color: #242424; background-image: none;");
  this->level[0]->setMinimum(0);
  this->level[1]->setMinimum(0);
  this->level[0]->setMaximum(75);
  this->level[1]->setMaximum(75);

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
  top_window->addSpacerItem(new QSpacerItem(400, 1));

  auto *top_center_buttons = new QVBoxLayout();
  top_center_buttons->addWidget(this->doRandom);
  top_center_buttons->addWidget(this->screenShot);
  top_window->addLayout(top_center_buttons);

  top_window->addSpacerItem(new QSpacerItem(400, 1));
  top_window->addLayout(level_setter2);
  top_window->addWidget(this->RandomSettings[1]);

  auto *check_settings = new QVBoxLayout();
  check_settings->addWidget(this->sameTeamLevel);
  check_settings->addWidget(this->muteAncestor);

  auto *radio_settings = new QVBoxLayout();
  radio_settings->addWidget(this->radio1t);
  radio_settings->addWidget(this->radio3t);

  auto *middle = new QHBoxLayout();
  middle->addSpacerItem(new QSpacerItem(500, 1));
  middle->addLayout(check_settings);
  middle->addLayout(radio_settings);
  middle->addSpacerItem(new QSpacerItem(500, 1));

  auto *bottom = new QHBoxLayout();
  this->leftSide->addSpacerItem(new QSpacerItem(1, 600)); // for placing ui parts in the top of screen
  bottom->addLayout(this->leftSide);
  bottom->addLayout(this->rightSide);

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
void MainWindow::ClearLayout(QLayout *layout) {
  QLayoutItem *item;
  while ((item = layout->takeAt(0)) != nullptr) {
    if (QWidget *widget = item->widget()) {
      delete widget;
    } else if (QLayout *sublayout = item->layout()) {
      ClearLayout(sublayout);
    }
    delete item;
  }
}

void MainWindow::on_doRandom_clicked() {
  ClearLayout(this->leftSide);
  ClearLayout(this->rightSide);

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

    for (size_t squad_iter = 0; squad_iter < generated_squad.size(); squad_iter++) {
      if (current_player == MainWindow::kLeftPlayer) {
        // TODO(alexander): rewrite function so no need in +1
        this->leftSide->addWidget(new SquadNormalWidget(generated_squad[squad_iter], squad_iter + 1, false));
      } else {
        this->rightSide->addWidget(new SquadNormalWidget(generated_squad[squad_iter], squad_iter + 1, true));
      }
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
    this->level[0]->setValue(arg1);
    this->level[1]->setValue(arg1);
  }
}

void MainWindow::on_sameTeamLevel_clicked() {
  if (this->sameTeamLevel->isChecked()) {
    this->level[1]->setValue(this->level[0]->value());
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
  size_t imageSize        = 700;
  auto  *screenshot_label = new QLabel(dialog);
  screenshot_label->setPixmap(screenshot.scaled(imageSize, imageSize, Qt::KeepAspectRatio));
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
  ClearLayout(this->leftSide);
  ClearLayout(this->rightSide);
  this->leftSide->addSpacerItem(new QSpacerItem(1, 600));
}

void MainWindow::on_muteAncestor_clicked() {
  this->playVoice = !this->muteAncestor->isChecked();
}