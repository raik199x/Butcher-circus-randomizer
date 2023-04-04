#include "../include/mainwindow.h"
#include "../include/centralwidget.h"
#include "../include/filemanip.h"
#include "../include/heroselection.h"
#include "../include/random.h"

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
#include <qbitmap.h>
#include <qboxlayout.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qmediaplayer.h>
#include <qnamespace.h>
#include <qobject.h>

#include <array>
#include <fstream>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <string>
#include <vector>

/*!
 * \file mainwindow.cpp
 * \brief MainWindow Interface
 *
 * \author raik
 */

using namespace std;

MainWindow::MainWindow(QWidget *parent) {
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
	QVBoxLayout *levelSetter1 = new QVBoxLayout();
	levelSetter1->addWidget(new QLabel("Level"));
	levelSetter1->addWidget(this->level[0]);

	QVBoxLayout *levelSetter2 = new QVBoxLayout();
	levelSetter2->addWidget(new QLabel("Level"));
	levelSetter2->addWidget(this->level[1]);

	QHBoxLayout *topWindow = new QHBoxLayout();
	topWindow->addWidget(this->RandomSettings[0]);
	topWindow->addLayout(levelSetter1);
	topWindow->addSpacerItem(new QSpacerItem(400, 1));

	QVBoxLayout *topCenterButtons = new QVBoxLayout();
	topCenterButtons->addWidget(this->doRandom);
	topCenterButtons->addWidget(this->screenShot);
	topWindow->addLayout(topCenterButtons);

	topWindow->addSpacerItem(new QSpacerItem(400, 1));
	topWindow->addLayout(levelSetter2);
	topWindow->addWidget(this->RandomSettings[1]);

	QVBoxLayout *checkSettings = new QVBoxLayout();
	checkSettings->addWidget(this->sameTeamLevel);
	checkSettings->addWidget(this->muteAncestor);

	QVBoxLayout *radioSettings = new QVBoxLayout();
	radioSettings->addWidget(this->radio1t);
	radioSettings->addWidget(this->radio3t);

	QHBoxLayout *middle = new QHBoxLayout();
	middle->addSpacerItem(new QSpacerItem(500, 1));
	middle->addLayout(checkSettings);
	middle->addLayout(radioSettings);
	middle->addSpacerItem(new QSpacerItem(500, 1));

	QHBoxLayout *bottom = new QHBoxLayout();
	this->leftSide->addSpacerItem(new QSpacerItem(1, 600)); // for placing ui parts in the top of screen
	bottom->addLayout(this->leftSide);
	bottom->addLayout(this->rightSide);

	this->layout = new QVBoxLayout(this->ui);
	this->layout->addLayout(topWindow);
	this->layout->addLayout(middle);
	this->layout->addLayout(bottom);
}

MainWindow::~MainWindow() {
	delete this->ui;
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

/**
 * @brief Parsing trinket name from string
 *
 * @param line
 * @param mode	1 -- leper[trinket_name] -> trinket_name
 * 				2 -- leper[trinket_name] -> leper
 * @return string
 */
string ParsingTrinket(string line, int mode) {
	string result;
	if (line.find("[") == string::npos)
		return line;
	if (mode == 1)
		for (unsigned int i = line.find("[") + 1; i < line.size() - 1; i++) // obtaining hero name
			result += line[i];
	else if (mode == 2)
		for (unsigned int i = 0; i < line.find("["); i++) // obtaining hero name
			result += line[i];
	return result;
}

/**
 * @brief Get the Trinkets
 *
 * @param lvl Player level
 * @param usedFighters Array of used fighters
 * @return QString*
 */
[[nodiscard]] QString *MainWindow::GetTrinkets(int lvl, QString *usedFighters) {
	vector<string> possibleTrinkets;
	QFile trinketList(":/trinkets/trinkets/list.txt");
	if (!trinketList.open(QIODevice::ReadOnly)) {
		std::terminate();
	}
	QTextStream in(&trinketList);
	string stopLine;
	// guessing stop line
	if (lvl >= 69)
		stopLine = "-----";
	else {
		stopLine = "--" + to_string(lvl + 1);
		lvl + 1 < 10 ? stopLine += "--" : stopLine += "-";
	}
	// obtaining possible trinkets for team
	while (true) {
		QString line        = in.readLine();
		string lineFromFile = line.toStdString();
		if (lineFromFile == stopLine)
			break;
		if (lineFromFile.find("--") != string::npos) // skipping level delimeter
			continue;
		if (lineFromFile.find("[") != string::npos) { // means there somewhere symbol '['
			for (int i = 0; i < 4; i++)
				if (QString::fromStdString(ParsingTrinket(lineFromFile, 1)) == usedFighters[i]) {
					possibleTrinkets.push_back(lineFromFile);
					break;
				}
		} else
			possibleTrinkets.push_back(lineFromFile);
	}
	QString *trinkets = new QString[8];
	// randomize
	while (true) {
		bool escape = true;
		for (int i = 0; i < 8; i++)
			if (trinkets[i] == "") {
				escape = false;
				break;
			}
		if (escape == true)
			return trinkets;
		int num = possibleTrinkets.size() - 1;
		num     = Random::Uniform::integral(0, num);
		if (possibleTrinkets[num] == ParsingTrinket(possibleTrinkets[num], 1)) {
			for (int i = 0; i < 8; i++)
				if (trinkets[i] == "" && (((i + 1) % 2 == 0 && possibleTrinkets[num] != trinkets[i - 1].toStdString()) || (i + 1) % 2 != 0)) {
					trinkets[i] = QString::fromStdString(possibleTrinkets[num]);
					break;
				}
		} else {
			for (int i = 0; i < 4; i++)
				if (QString::fromStdString(ParsingTrinket(possibleTrinkets[num], 1)) == usedFighters[i]) {
					if (trinkets[i * 2] != "" && trinkets[i * 2 + 1] != "")
						break;
					trinkets[i * 2] == "" ? trinkets[i * 2] = QString::fromStdString(ParsingTrinket(possibleTrinkets[num], 2)) : trinkets[i * 2 + 1] = QString::fromStdString(ParsingTrinket(possibleTrinkets[num], 2));
					break;
				}
		}
		possibleTrinkets.erase(possibleTrinkets.cbegin() + num);
	}
	return trinkets;
}

/**
 * @brief Get the Fighters
 *
 * @param numCommand 0 or 1
 * @return QString* or nullptr if error occurred
 */
[[nodiscard]] QString *MainWindow::GetFighters(int numCommand) {
	// opening file
	string fileName = numCommand == 0 ? "BCR_T1.txt" : "BCR_T2.txt";
	if (!filesystem::exists(fileName) && !recreate(fileName)) {
		QMessageBox::critical(this, "Cannot create file", "For some reason BCR cannot create file for team random settings");
		return nullptr;
	}

	vector<string> possibleHeroes = getPossibleHeroes(fileName);

	QString *result = new QString[4];
	// randomize first team
	// randomize heroes
	for (int i = 0; i < 4; i++) {
		int index   = possibleHeroes.size();     // variable for storing index that has hero skills
		int numHero = Random::Uniform::integral(0, index - 1); // variable for storing hero index
		result[i]   = QString::fromStdString(possibleHeroes[numHero]);
		possibleHeroes.erase(possibleHeroes.cbegin() + numHero);
	}
	return result;
}

/**
 * @brief Get already randomized QString of skills
 *
 * @param numCommand decide which random settings need to use
 * @param Fighters Already randomized fighters
 * @return QString* (format ex: "1234" "1234" "1234" "1234"")
 */
QString *MainWindow::GetSkills(int numCommand, QString *Fighters) {
	QString *skills = d_getPossibleSkills(numCommand, Fighters);
	QString *result = new QString[4]; // skills for each hero

	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 4; j++) {
			int getNum = Random::Uniform::integral(0, (int)skills[i].size() - 1);
			result[i] += skills[i][getNum];
			// delete symbol from QString
			skills[i].remove(getNum, 1);
		}

	delete[] skills;
	return result;
}
/**
 * @brief Generates layout with all heroes layouts (hero + skills + trinkets, 4 heroes, in general whole team)
 *
 * @param fighters Already randomized fighters
 * @param skillsArray Already randomized skills
 * @param trinkets Already randomized trinkets
 * @param radio3t If we have a lot of teams for placing in ui, function will shrink size of images
 * @param numCommand in case of right side (numCommand = 1) will mirror images and positioning
 * @return QVBoxLayout*
 */
QVBoxLayout *GenerateTeam(QString *fighters, QString *skillsArray, QString *trinkets, bool radio3t, int numCommand) {
	QVBoxLayout *result = new QVBoxLayout;

	const size_t heroImageSize       = radio3t ? 50 : 60;
	const size_t skillImageSize      = radio3t ? 40 : 60;
	const size_t trinketImageSize[2] = {radio3t ? (size_t)40 : 50, radio3t ? (size_t)70 : 100};
	const size_t positionImageSize   = radio3t ? 30 : 40;

	QHBoxLayout *remember; // needed for radio3t to remember previous team layout
	// generating whole team (4 heroes with ther trinkets and spells)
	for (size_t i = 0; i < 4; i++) {
		QHBoxLayout *team = new QHBoxLayout;
		// creating trinkets (equal for radio3t)
		QHBoxLayout *trinketsLayout = new QHBoxLayout;
		QLabel *trinket1            = new QLabel;
		QLabel *trinket2            = new QLabel;
		trinket1->setFixedSize(trinketImageSize[0], trinketImageSize[1]);
		trinket2->setFixedSize(trinketImageSize[0], trinketImageSize[1]);
		trinket1->setPixmap(QPixmap(":/trinkets/trinkets/" + trinkets[i * 2] + ".png").scaled(trinketImageSize[0], trinketImageSize[1], Qt::KeepAspectRatio));
		trinket2->setPixmap(QPixmap(":/trinkets/trinkets/" + trinkets[i * 2 + 1] + ".png").scaled(trinketImageSize[0], trinketImageSize[1], Qt::KeepAspectRatio));
		trinketsLayout->addWidget(trinket1);
		trinketsLayout->addWidget(trinket2);

		// creating hero (this part equal for radio3t)
		QLabel *hero = new QLabel;
		hero->setFixedSize(heroImageSize, heroImageSize);
		QPixmap heroImage(":/heroes/heroes+spells/" + fighters[i] + "/hero_" + fighters[i] + ".png");
		heroImage = heroImage.scaled(heroImageSize, heroImageSize, Qt::KeepAspectRatio);
		if (numCommand != 0) // if we place hero in right side, we rotate image
			heroImage = heroImage.transformed(QTransform().scale(-1, 1));
		hero->setPixmap(heroImage);

		// creating skills
		QLabel **skills = new QLabel *[4];
		// without for loop because of better speed
		skills[0] = new QLabel;
		skills[1] = new QLabel;
		skills[2] = new QLabel;
		skills[3] = new QLabel;
		if (radio3t) {                                 // Since we have 3 teams, we need to shrink size of images
			QVBoxLayout *skillsLayout = new QVBoxLayout; // and place them in vertical layout (for less space)
			for (size_t j = 0; j < 4; j += 2) {          // we make step 2 because we want to place 2 skills in one row (using horizontal layout)
				skills[j]->setFixedSize(skillImageSize, skillImageSize);
				skills[j + 1]->setFixedSize(skillImageSize, skillImageSize);

				if (fighters[i] == "abomination") { // if we have abomination, we place NA images
					QPixmap pixmap(":/banner/heroes+spells/NA.png");
					pixmap = pixmap.scaled(skillImageSize, skillImageSize, Qt::KeepAspectRatio);
					skills[j]->setPixmap(pixmap);
					skills[j + 1]->setPixmap(pixmap);
				} else {
					QPixmap pixmap(":/heroes/heroes+spells/" + fighters[i] + "/" + skillsArray[i][j] + ".png");
					QPixmap pixmap2(":/heroes/heroes+spells/" + fighters[i] + "/" + skillsArray[i][j + 1] + ".png");
					pixmap  = pixmap.scaled(skillImageSize, skillImageSize, Qt::KeepAspectRatio);
					pixmap2 = pixmap2.scaled(skillImageSize, skillImageSize, Qt::KeepAspectRatio);
					skills[j]->setPixmap(pixmap);
					skills[j + 1]->setPixmap(pixmap2);
				}
				// and then placing hero skills together
				QHBoxLayout *subSkillsLayout = new QHBoxLayout;
				subSkillsLayout->addWidget(skills[j]);
				subSkillsLayout->addWidget(skills[j + 1]);
				skillsLayout->addLayout(subSkillsLayout);
			}
			QVBoxLayout *heroLayout = new QVBoxLayout;
			heroLayout->addWidget(hero);
			QLabel *posText = new QLabel("Pos " + QString::number(i + 1));
			posText->setFixedSize(45, 15); // here hard coded numbers since it is only for radio3t
			heroLayout->addWidget(posText);

			team->addLayout(trinketsLayout);
			team->addLayout(heroLayout);
			team->addLayout(skillsLayout);

			if (i % 2 == 0)
				remember = team; // remember previous team
			else {
				// and then put them together in ui (for space saving)
				QHBoxLayout *connectTeams = new QHBoxLayout;
				connectTeams->addLayout(remember);
				connectTeams->addLayout(team);
				result->addLayout(connectTeams); // saving result
			}

			continue;
		}
		QHBoxLayout *skillsLayout = new QHBoxLayout; // since we have a lot of space we will use another type of layout
		for (size_t j = 0; j < 4; j++) {
			QLabel *skill = new QLabel;
			skill->setFixedSize(skillImageSize, skillImageSize);
			QPixmap pixmap;
			if (fighters[i] == "abomination") // still skipping abomination
				pixmap = QPixmap(":/banner/heroes+spells/NA.png");
			else
				pixmap = QPixmap(":/heroes/heroes+spells/" + fighters[i] + "/" + skillsArray[i][j] + ".png");
			pixmap = pixmap.scaled(skillImageSize, skillImageSize, Qt::KeepAspectRatio);
			skill->setPixmap(pixmap);
			skillsLayout->addWidget(skill);
		}

		QHBoxLayout *heroLayout       = new QHBoxLayout;
		QVBoxLayout *skillsHeroLayout = new QVBoxLayout;
		skillsHeroLayout->addLayout(heroLayout);
		skillsHeroLayout->addLayout(skillsLayout);

		if (numCommand == 0) { // if we place hero in left side, we place it normally
			heroLayout->addWidget(hero);
			for (size_t j = 0; j < 4; j++) {
				QLabel *pos = new QLabel;
				if (i == j) // if hero is in this position, then we need to show full circle
					pos->setPixmap(QPixmap(":/Position/circles/fullCircle.png").scaled(positionImageSize, positionImageSize, Qt::KeepAspectRatio));
				else
					pos->setPixmap(QPixmap(":/Position/circles/emptyCircle.png").scaled(positionImageSize, positionImageSize, Qt::KeepAspectRatio));
				heroLayout->addWidget(pos);
			}

			team->addLayout(trinketsLayout);
			team->addLayout(skillsHeroLayout);
			heroLayout->setAlignment(Qt::AlignLeft);
			team->addSpacerItem(new QSpacerItem(50 * (4 - i), 1));
		} else { // we need to make effect of mirror (right side)
			for (size_t j = 0; j < 4; j++) {
				QLabel *pos = new QLabel;
				if (3 - i == j) // since it is mirror, we need to change order of positions
					pos->setPixmap(QPixmap(":/Position/circles/fullCircle.png").scaled(positionImageSize, positionImageSize, Qt::KeepAspectRatio));
				else
					pos->setPixmap(QPixmap(":/Position/circles/emptyCircle.png").scaled(positionImageSize, positionImageSize, Qt::KeepAspectRatio));
				heroLayout->addWidget(pos);
			}
			heroLayout->addWidget(hero);
			team->addSpacerItem(new QSpacerItem(50 * (4 - i), 1));
			team->addLayout(skillsHeroLayout);
			team->addLayout(trinketsLayout);
			heroLayout->setAlignment(Qt::AlignRight);
		}
		result->addLayout(team); // saving result in layout
	}
	if (radio3t)
		// if 3t, then we need to add empty space between teams
		result->addWidget(new QLabel(""));
	return result;
}

void MainWindow::on_doRandom_clicked() {
	ClearLayout(this->leftSide);
	ClearLayout(this->rightSide);

	size_t needToGenerate = 0;
	if (this->radio3t->isChecked())
		needToGenerate = 6;
	else
		needToGenerate = 2;

	for (size_t i = 0; i < needToGenerate; i++) {
		int numCommand    = i % 2 == 0 ? 0 : 1;
		QString *fighters = GetFighters(numCommand);
		QString *skills   = GetSkills(numCommand, fighters);
		QString *trinkets = GetTrinkets(numCommand == 0 ? this->level[0]->value() : this->level[1]->value(), fighters);

		numCommand == 0 ? this->leftSide->addLayout(GenerateTeam(fighters, skills, trinkets, this->radio3t->isChecked(), numCommand)) : this->rightSide->addLayout(GenerateTeam(fighters, skills, trinkets, this->radio3t->isChecked(), numCommand));

		delete[] fighters;
		delete[] skills;
		delete[] trinkets;
	}
	// TODO: make do not repeat one replica two times in a row
	if (!this->muteAncestor->isChecked() && this->playVoice) {
		QMediaPlayer *player = new QMediaPlayer;
		QAudioOutput *output = new QAudioOutput;
		player->setAudioOutput(output);
		player->setSource(QUrl("qrc:/sounds/ancestor/" + QString::number(Random::Uniform::integral(1, 10)) + ".wav"));
		output->setVolume(0.4);
		player->play();
		this->playVoice = false;
	} else if (!this->muteAncestor->isChecked())
		this->playVoice = true;
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
	QPixmap screenshot    = this->grab(); // take screenshot
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setPixmap(screenshot);

	QDialog *dialog = new QDialog(this);

	// pasting screenshot into window
	size_t imageSize        = 700;
	QLabel *screenshotLabel = new QLabel(dialog);
	screenshotLabel->setPixmap(screenshot.scaled(imageSize, imageSize, Qt::KeepAspectRatio));
	QRect screenGeometry = screen()->geometry();

	// setting up window
	dialog->setStyleSheet("color: #FFFFFF;");
	dialog->setGeometry(screenGeometry.width(), screenGeometry.height(), screenshotLabel->pixmap().width(), screenshotLabel->pixmap().height());
	dialog->setWindowTitle("Saved to clipboard");
	dialog->setModal(true);

	QTimer::singleShot(1500, [=]() {
		dialog->accept();
	}); // Clean up dialog after1 seconds

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