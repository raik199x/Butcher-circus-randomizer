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
#include <QPushButton>
#include <QRadioButton>

#include <cstddef>
#include <string>
#include <vector>
#include <optional>

#include "mainwindow.h"
#include "centralwidget.h"
#include "filemanip.h"
#include "heroselection.h"
#include "random.h"

MainWindow::MainWindow(QWidget *parent) {
  //! \note Create tricky prng
  this->prng = new Random::Tricky<uint>(1u, 10u);

  this->ui = new CentralWidget(this);
  setCentralWidget(ui);

  this->playVoice = true;
  // Setting up own made ui
  this->setFixedSize(QSize(1192, 665));
  this->ui->setStyleSheet("color: #FFFFFF;");

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

std::string MainWindow::getFileNameBasedOnPlayer(const uint8_t player) {
  return player == MainWindow::kLeftPlayer ? "BCR_T1.txt" : "BCR_T2.txt";
}

/**
 * @brief Clears the layout items, but does not deletes layout itself
 *
 * @param layout this->leftSide or this->rightSide
 */
void MainWindow::ClearLayout(QLayout *layout) {
  QLayoutItem *item;
  while ((item = layout->takeAt(0))) {
    if (QWidget *widget = item->widget()) {
      delete widget;
    } else if (QLayout *sublayout = item->layout()) {
      ClearLayout(sublayout);
    }
    delete item;
  }
}

QString parseTrinketName(QString line) {
  long long open_bracket_pos = line.indexOf('[');
  if (open_bracket_pos == -1) {
    return line;
  }
  return line.mid(0, open_bracket_pos);
}

QString parseTrinketHeroLimit(const QString &line) {
  long long open_bracket_pos = line.indexOf('[');
  if (open_bracket_pos == -1) {
    return "NO_HERO_LIMIT"; // TODO(alexander): Remove hardcode
  }
  long long closing_bracket_pos = line.indexOf(']');
  return line.mid(open_bracket_pos + 1, closing_bracket_pos - open_bracket_pos - 1);
}

std::array<QString, MainWindow::kAmountOfTrinketsForTeam>
MainWindow::getTrinkets(uint8_t lvl, std::array<QString, kRequiredNumberOfFighters> fighters) {
  std::vector<QString> possible_trinkets;
  QFile                trinket_list(":/trinkets/trinkets/list.txt");
  if (!trinket_list.open(QIODevice::ReadOnly)) {
    std::terminate();
  }
  QTextStream file_stream_read(&trinket_list);
  QString     stop_line;

  // TODO(alexander): move to filemanip
  // guessing stop line
  if (lvl >= 69) {
    stop_line = "-----";
  } else {
    stop_line = "--" + QString::number(lvl + 1);
    lvl + 1 < 10 ? stop_line += "--" : stop_line += "-";
  }

  // obtaining possible trinkets for team
  while (true) {
    QString line_from_file = file_stream_read.readLine();
    qDebug() << line_from_file;
    if (line_from_file == stop_line) {
      break;
    }
    if (line_from_file.indexOf("--") != -1) { // skipping level delimeter
      continue;
    }
    if (line_from_file.indexOf('[') != -1) { // means there somewhere symbol '['
      for (int i = 0; i < kRequiredNumberOfFighters; i++) {
        if (parseTrinketHeroLimit(line_from_file) == fighters[i]) {
          possible_trinkets.push_back(line_from_file);
          break;
        }
      }
    } else {
      possible_trinkets.push_back(line_from_file);
    }
  }

  std::array<QString, kAmountOfTrinketsForTeam> trinkets;

  // randomize
  while (true) {
    bool escape = true;
    for (uint8_t iter_trinket = 0; iter_trinket < kAmountOfTrinketsForTeam; iter_trinket++) {
      if (trinkets[iter_trinket].isEmpty()) {
        escape = false;
        break;
      }
    }
    if (escape) {
      return trinkets;
    }

    int random_trinket_number = static_cast<int>(possible_trinkets.size() - 1);
    random_trinket_number     = Random::Uniform::integral(0, random_trinket_number);

    if (possible_trinkets[random_trinket_number] == parseTrinketHeroLimit(possible_trinkets[random_trinket_number])) {
      for (int iter_trinket = 0; iter_trinket < kAmountOfTrinketsForTeam; iter_trinket++) {
        if (trinkets[iter_trinket].isEmpty() &&
            (((iter_trinket + 1) % 2 == 0 && possible_trinkets[random_trinket_number] != trinkets[iter_trinket - 1]) ||
             (iter_trinket + 1) % 2 != 0)) {
          trinkets[iter_trinket] = possible_trinkets[random_trinket_number];
          break;
        }
      }
    } else {
      for (long iter_fighters = 0; iter_fighters < kRequiredNumberOfFighters; iter_fighters++) {
        if (parseTrinketHeroLimit(possible_trinkets[random_trinket_number]) == fighters[iter_fighters]) {
          // TODO(alexander): if hero have two empty trinket slots?
          // TODO(alexander): add two variables to store position of both trinkets of current fighter
          if (trinkets[iter_fighters * 2] != "" && trinkets[iter_fighters * 2 + 1] != "") {
            break;
          }
          trinkets[iter_fighters * 2] == ""
              ? trinkets[iter_fighters * 2]     = parseTrinketName(possible_trinkets[random_trinket_number])
              : trinkets[iter_fighters * 2 + 1] = parseTrinketName(possible_trinkets[random_trinket_number]);
          break;
        }
      }
    }
    possible_trinkets.erase(possible_trinkets.cbegin() + random_trinket_number);
  }
  return trinkets;
}

/**
 * @brief Get array of randomly selected fighters
 *
 * @param player Player index (Left or Right)
 * @return std::optional<std::array<QString, kRequiredNumberOfFighters>> nullopt if fails, array otherwise
 */
std::optional<std::array<QString, kRequiredNumberOfFighters>> MainWindow::getFighters(uint8_t player) {
  // opening file
  std::string file_name = MainWindow::getFileNameBasedOnPlayer(player);
  if (!std::filesystem::exists(file_name) && !recreate(file_name)) {
    QMessageBox::critical(this, "Cannot create file",
                          "For some reason BCR cannot create file for team random settings");
    return std::nullopt;
  }

  std::vector<std::string>                       possible_heroes = getPossibleHeroes(file_name);
  std::array<QString, kRequiredNumberOfFighters> result;

  // randomize heroes
  for (int iter_getting_hero = 0; iter_getting_hero < kRequiredNumberOfFighters; iter_getting_hero++) {
    uint8_t index       = static_cast<int>(possible_heroes.size()); // variable for storing index that has hero skills
    uint8_t hero_number = Random::Uniform::integral(0, index - 1);  // variable for storing hero index
    result[iter_getting_hero] = QString::fromStdString(possible_heroes[hero_number]);
    possible_heroes.erase(possible_heroes.cbegin() + hero_number);
  }

  return result;
}

/**
 * @brief Get already randomized QString of skills
 *
 * @param Fighters Already randomized fighters
 * @return array of QString (format ex: "1234" "1234" "1234" "1234"")
 */
std::array<QString, kRequiredNumberOfFighters>
MainWindow::getSkills(uint8_t player, const std::array<QString, kRequiredNumberOfFighters> &fighters) {
  std::array<QString, kRequiredNumberOfFighters> skills =
      getPossibleSkills(MainWindow::getFileNameBasedOnPlayer(player), fighters, false);
  std::array<QString, kRequiredSpellsForFighter> result; // skills for each hero

  for (size_t iter_fighters = 0; iter_fighters < kRequiredNumberOfFighters; iter_fighters++) {
    for (size_t iter_skills = 0; iter_skills < kRequiredSpellsForFighter; iter_skills++) {
      int get_num = Random::Uniform::integral(0, (int)skills[iter_fighters].size() - 1);
      result[iter_fighters] += skills[iter_fighters][get_num];
      // delete symbol from QString
      skills[iter_fighters].remove(get_num, 1);
    }
  }

  return result;
}

QVBoxLayout *generateTeam(std::array<QString, kRequiredNumberOfFighters>            fighters,
                          std::array<QString, kRequiredSpellsForFighter>            skillsArray,
                          std::array<QString, MainWindow::kAmountOfTrinketsForTeam> trinkets,
                          bool is_competitive_enabled, uint8_t player) {
  auto *result = new QVBoxLayout;

  const int hero_image_size  = is_competitive_enabled ? 50 : 60;
  const int skill_image_size = is_competitive_enabled ? 40 : 60;
  // TODO(alexander): Add macros for '2' below
  const int trinket_image_size[2] = {is_competitive_enabled ? 40 : 50, is_competitive_enabled ? 70 : 100};
  const int position_image_size   = is_competitive_enabled ? 30 : 40;

  QHBoxLayout *remember; // needed for radio3t to remember previous team layout
  // generating whole team (4 heroes with ther trinkets and spells)
  for (size_t iter_current_fighter = 0; iter_current_fighter < kRequiredNumberOfFighters; iter_current_fighter++) {
    auto *team = new QHBoxLayout;
    // creating trinkets (equal for radio3t)
    auto *trinkets_layout = new QHBoxLayout;
    auto *trinket1        = new QLabel;
    auto *trinket2        = new QLabel;

    trinket1->setFixedSize(trinket_image_size[0], trinket_image_size[1]);
    trinket2->setFixedSize(trinket_image_size[0], trinket_image_size[1]);
    trinket1->setPixmap(QPixmap(":/trinkets/trinkets/" + trinkets[iter_current_fighter * 2] + ".png")
                            .scaled(trinket_image_size[0], trinket_image_size[1], Qt::KeepAspectRatio));
    trinket2->setPixmap(QPixmap(":/trinkets/trinkets/" + trinkets[iter_current_fighter * 2 + 1] + ".png")
                            .scaled(trinket_image_size[0], trinket_image_size[1], Qt::KeepAspectRatio));
    trinkets_layout->addWidget(trinket1);
    trinkets_layout->addWidget(trinket2);

    // creating hero (this part equal for radio3t)
    auto *hero = new QLabel;
    hero->setFixedSize(hero_image_size, hero_image_size);
    QPixmap hero_image(":/heroes/heroes+spells/" + fighters[iter_current_fighter] + "/hero_" +
                       fighters[iter_current_fighter] + ".png");
    hero_image = hero_image.scaled(hero_image_size, hero_image_size, Qt::KeepAspectRatio);
    if (player != 0) { // if we place hero in right side, we rotate image
      hero_image = hero_image.transformed(QTransform().scale(-1, 1));
    }
    hero->setPixmap(hero_image);

    // creating skills
    auto **skills = new QLabel *[kRequiredSpellsForFighter];
    // without for loop because of better speed
    skills[0] = new QLabel;
    skills[1] = new QLabel;
    skills[2] = new QLabel;
    skills[3] = new QLabel;
    if (is_competitive_enabled) {            // Since we have 3 teams, we need to shrink size of images
      auto *skills_layout = new QVBoxLayout; // and place them in vertical layout (for less space)
      for (long long j = 0; j < 4;
           j += 2) { // we make step 2 because we want to place 2 skills in one row (using horizontal layout)
        skills[j]->setFixedSize(skill_image_size, skill_image_size);
        skills[j + 1]->setFixedSize(skill_image_size, skill_image_size);

        if (fighters[iter_current_fighter] == "abomination") { // if we have abomination, we place NA images
          QPixmap pixmap(":/banner/heroes+spells/NA.png");
          pixmap = pixmap.scaled(skill_image_size, skill_image_size, Qt::KeepAspectRatio);
          skills[j]->setPixmap(pixmap);
          skills[j + 1]->setPixmap(pixmap);
        } else {
          QPixmap pixmap(":/heroes/heroes+spells/" + fighters[iter_current_fighter] + "/" +
                         skillsArray[iter_current_fighter][j] + ".png");
          QPixmap pixmap2(":/heroes/heroes+spells/" + fighters[iter_current_fighter] + "/" +
                          skillsArray[iter_current_fighter][j + 1] + ".png");
          pixmap  = pixmap.scaled(skill_image_size, skill_image_size, Qt::KeepAspectRatio);
          pixmap2 = pixmap2.scaled(skill_image_size, skill_image_size, Qt::KeepAspectRatio);
          skills[j]->setPixmap(pixmap);
          skills[j + 1]->setPixmap(pixmap2);
        }
        // and then placing hero skills together
        auto *sub_skills_layout = new QHBoxLayout;
        sub_skills_layout->addWidget(skills[j]);
        sub_skills_layout->addWidget(skills[j + 1]);
        skills_layout->addLayout(sub_skills_layout);
      }
      auto *hero_layout = new QVBoxLayout;
      hero_layout->addWidget(hero);
      auto *pos_text = new QLabel("Pos " + QString::number(iter_current_fighter + 1));
      pos_text->setFixedSize(45, 15); // here hard coded numbers since it is only for radio3t
      hero_layout->addWidget(pos_text);

      team->addLayout(trinkets_layout);
      team->addLayout(hero_layout);
      team->addLayout(skills_layout);

      if (iter_current_fighter % 2 == 0) {
        remember = team; // remember previous team
      } else {
        // and then put them together in ui (for space saving)
        auto *connect_teams = new QHBoxLayout;
        connect_teams->addLayout(remember);
        connect_teams->addLayout(team);
        result->addLayout(connect_teams); // saving result
      }

      continue;
    }
    auto *skills_layout = new QHBoxLayout; // since we have a lot of space we will use another type of layout
    for (long long j = 0; j < 4; j++) {
      auto *skill = new QLabel;
      skill->setFixedSize(skill_image_size, skill_image_size);
      QPixmap pixmap;
      if (fighters[iter_current_fighter] == "abomination") { // still skipping abomination
        pixmap = QPixmap(":/banner/heroes+spells/NA.png");
      } else {
        pixmap = QPixmap(":/heroes/heroes+spells/" + fighters[iter_current_fighter] + "/" +
                         skillsArray[iter_current_fighter][j] + ".png");
      }
      pixmap = pixmap.scaled(skill_image_size, skill_image_size, Qt::KeepAspectRatio);
      skill->setPixmap(pixmap);
      skills_layout->addWidget(skill);
    }

    auto *hero_layout        = new QHBoxLayout;
    auto *skills_hero_layout = new QVBoxLayout;
    skills_hero_layout->addLayout(hero_layout);
    skills_hero_layout->addLayout(skills_layout);

    if (player == 0) { // if we place hero in left side, we place it normally
      hero_layout->addWidget(hero);
      for (long long j = 0; j < 4; j++) {
        auto *pos = new QLabel;
        if (iter_current_fighter == j) { // if hero is in this position, then we need to show full circle
          pos->setPixmap(QPixmap(":/Position/circles/fullCircle.png")
                             .scaled(position_image_size, position_image_size, Qt::KeepAspectRatio));
        } else {
          pos->setPixmap(QPixmap(":/Position/circles/emptyCircle.png")
                             .scaled(position_image_size, position_image_size, Qt::KeepAspectRatio));
        }
        hero_layout->addWidget(pos);
      }

      team->addLayout(trinkets_layout);
      team->addLayout(skills_hero_layout);
      hero_layout->setAlignment(Qt::AlignLeft);
      team->addSpacerItem(new QSpacerItem(50 * (4 - iter_current_fighter), 1));
    } else { // we need to make effect of mirror (right side)
      for (size_t j = 0; j < 4; j++) {
        QLabel *pos = new QLabel;
        if (3 - iter_current_fighter == j) { // since it is mirror, we need to change order of positions
          pos->setPixmap(QPixmap(":/Position/circles/fullCircle.png")
                             .scaled(position_image_size, position_image_size, Qt::KeepAspectRatio));
        } else {
          pos->setPixmap(QPixmap(":/Position/circles/emptyCircle.png")
                             .scaled(position_image_size, position_image_size, Qt::KeepAspectRatio));
        }
        hero_layout->addWidget(pos);
      }
      hero_layout->addWidget(hero);
      team->addSpacerItem(new QSpacerItem(50 * (4 - iter_current_fighter), 1));
      team->addLayout(skills_hero_layout);
      team->addLayout(trinkets_layout);
      hero_layout->setAlignment(Qt::AlignRight);
    }
    result->addLayout(team); // saving result in layout
  }
  if (is_competitive_enabled) {
    // if 3t, then we need to add empty space between teams
    result->addWidget(new QLabel(""));
  }
  return result;
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

    auto fighters = getFighters(current_player).value_or(std::array<QString, kRequiredNumberOfFighters>{"Fail"});
    if (fighters[0] == "Fail") {
      return;
    }
    auto skills   = getSkills(current_player, fighters);
    auto trinkets = getTrinkets(current_player == 0 ? this->level[0]->value() : this->level[1]->value(), fighters);

    current_player == kLeftPlayer ? this->leftSide->addLayout(generateTeam(fighters, skills, trinkets,
                                                                           this->radio3t->isChecked(), current_player))
                                  : this->rightSide->addLayout(generateTeam(
                                        fighters, skills, trinkets, this->radio3t->isChecked(), current_player));
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
  if (this->sameTeamLevel->isChecked())
    this->level[1]->setValue(this->level[0]->value());
}

void MainWindow::on_RandomSettings1_clicked() {
  HeroSelection win(this, 0);
  win.setModal(true);
  win.exec();
}

void MainWindow::on_RandomSettings2_clicked() {
  HeroSelection win(this, 1);
  win.setModal(true);
  win.exec();
}

void MainWindow::on_screenShot_clicked() {
  QPixmap     screenshot = this->grab(); // take screenshot
  QClipboard *clipboard  = QGuiApplication::clipboard();
  clipboard->setPixmap(screenshot);

  QDialog *dialog = new QDialog(this);

  // pasting screenshot into window
  size_t  imageSize       = 700;
  QLabel *screenshotLabel = new QLabel(dialog);
  screenshotLabel->setPixmap(screenshot.scaled(imageSize, imageSize, Qt::KeepAspectRatio));
  QRect screenGeometry = screen()->geometry();

  // setting up window
  dialog->setStyleSheet("color: #FFFFFF;");
  dialog->setGeometry(screenGeometry.width(), screenGeometry.height(), screenshotLabel->pixmap().width(),
                      screenshotLabel->pixmap().height());
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
  if (this->muteAncestor->isChecked())
    this->playVoice = false;
  else
    this->playVoice = true;
}