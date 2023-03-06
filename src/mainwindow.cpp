#include "../include/filemanip.h"
#include "../include/heroselection.h"
#include "../include/mainwindow.h"
#include "../include/random.h"

#include <QClipboard>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlayoutitem.h>
#include <qobject.h>
#include <QGuiApplication>
#include <QSpacerItem>

#include <fstream>
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
	this->ui = new QWidget(this);
	setCentralWidget(ui);

	// Setting up own made ui
	this->setFixedSize(QSize(1192, 665));
	this->ui->setStyleSheet("color: #FFFFFF");

	// allocating widgets
	this->RandomSettings = new QPushButton*[2];
	this->RandomSettings[0] = new QPushButton("Random Settings T1");
	this->RandomSettings[1] = new QPushButton("Random Settings T2");

	this->level = new QSpinBox*[2];
	this->level[0] = new QSpinBox();
	this->level[1] = new QSpinBox();

	this->doRandom = new QPushButton("Randomize");
	this->screenShot = new QPushButton("Screenshot");

	this->radio1t = new QRadioButton("1 Team");
	this->radio3t = new QRadioButton("3 Teams");

	this->sameTeamLevel = new QCheckBox("Same Team Level");
	this->muteAncestor = new QCheckBox("Mute Ancestor");

	this->leftSide = new QVBoxLayout();
	this->rightSide = new QVBoxLayout();

	// connecting signals
	connect(this->RandomSettings[0], SIGNAL(clicked()), this, SLOT(on_RandomSettings1_clicked()));
	connect(this->RandomSettings[1], SIGNAL(clicked()), this, SLOT(on_RandomSettings2_clicked()));
	connect(this->doRandom, SIGNAL(clicked()), this, SLOT(on_doRandom_clicked()));
	connect(this->screenShot, SIGNAL(clicked()), this, SLOT(on_screenShot_clicked()));
	connect(this->radio1t, SIGNAL(clicked()), this, SLOT(on_radio1t_clicked()));
	connect(this->radio3t, SIGNAL(clicked()), this, SLOT(on_radio3t_clicked()));
	connect(this->level[0], SIGNAL(valueChanged(int)), this, SLOT(on_level1_valueChanged(int)));
	connect(this->level[1], SIGNAL(valueChanged(int)), this, SLOT(on_level2_valueChanged(int)));
	connect(this->sameTeamLevel, SIGNAL(clicked()), this, SLOT(on_sameTeamLevel_clicked()));

	// Designing ui
	this->RandomSettings[0]->setStyleSheet("background-color: #3F3F3F");
	this->RandomSettings[0]->setFixedHeight(50);
	this->RandomSettings[1]->setStyleSheet("background-color: #3F3F3F");
	this->RandomSettings[1]->setFixedHeight(50);

	this->doRandom->setStyleSheet("background-color: #3F3F3F");
	this->doRandom->setFixedHeight(50);
	this->screenShot->setStyleSheet("background-color: #3F3F3F");
	this->radio1t->setChecked(true);

	this->level[0]->setStyleSheet("background-color: #242424");
	this->level[1]->setStyleSheet("background-color: #242424");
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
	topWindow->addSpacerItem(new QSpacerItem(400,1));

	QVBoxLayout *topCenterButtons = new QVBoxLayout();
	topCenterButtons->addWidget(this->doRandom);
	topCenterButtons->addWidget(this->screenShot);
	topWindow->addLayout(topCenterButtons);

	topWindow->addSpacerItem(new QSpacerItem(400,1));
	topWindow->addLayout(levelSetter2);
	topWindow->addWidget(this->RandomSettings[1]);

	QVBoxLayout *checkSettings = new QVBoxLayout();
	checkSettings->addWidget(this->sameTeamLevel);
	checkSettings->addWidget(this->muteAncestor);

	QVBoxLayout *radioSettings = new QVBoxLayout();
	radioSettings->addWidget(this->radio1t);
	radioSettings->addWidget(this->radio3t);

	QHBoxLayout *middle = new QHBoxLayout();
	middle->addSpacerItem(new QSpacerItem(500,1));
	middle->addLayout(checkSettings);
	middle->addLayout(radioSettings);
	middle->addSpacerItem(new QSpacerItem(500,1));

	QHBoxLayout *bottom = new QHBoxLayout();
	bottom->addLayout(this->leftSide);
	bottom->addLayout(this->rightSide);

	this->layout = new QVBoxLayout(this->ui);
	this->layout->addLayout(topWindow);
	this->layout->addLayout(middle);
	this->layout->addLayout(bottom);

	

	for (int i = 0; i < NUMBER_OF_HEROS; i++)
		heros[i] = "NULL";

	RefreshLayout();
}

MainWindow::~MainWindow() {
	delete this->ui;
}

void MainWindow::RefreshLayout(){
	// deleting previous elements
	// we can do this for both, since in there is equal amount of items in leftSide and rightSide
	/*for(size_t i = 0; i < this->leftSide->count(); i++){
		QLayoutItem* itemL = leftSide->takeAt(i);
		QLayoutItem* itemR = rightSide->takeAt(i);
		if (itemL != nullptr && itemR != nullptr) {
		    delete itemL->widget();  // delete the widget associated with the item
		    delete itemL;  // delete the item itself

			delete itemR->widget();
			delete itemR;
		}
	}*/
	//if(assets)
	//	delete assets;
	/*if(this->radio1t){
		assets = new QLabel*[16];

		for(int i = 0; i < 16; i++)
			assets[i]->setText(QString::number(i));

		QHBoxLayout *trinketsL = new QHBoxLayout();
		trinketsL->addWidget(assets[0]);
		trinketsL->addWidget(assets[1]);
		this->leftSide->addLayout(trinketsL);
		this->leftSide->addWidget(assets[2]);

		QHBoxLayout *skillsUpper = new QHBoxLayout(), *skillsLower = new QHBoxLayout();
		skillsUpper->addWidget(assets[3]);
		skillsUpper->addWidget(assets[4]);
		skillsLower->addWidget(assets[5]);
		skillsLower->addWidget(assets[6]);
		this->leftSide->addLayout(skillsUpper);
		this->leftSide->addLayout(skillsLower);
	}*/
}

/**
 * @brief Hide/Unhide assets of ui after changing radio button status
 *
 */
void MainWindow::SwitchedTeamRadioButton() {
	/*if (this->ui->radio1t->isChecked()) {
		for (size_t i = 0; i < 6; i++)
			for (size_t j = 0; j < 7; j++)
				assets3v3[i][j].hide();

		for (size_t i = 0; i < 2; i++)
			for (size_t j = 0; j < 7; j++)
				assets1v1[i][j].show();
	} else {
		for (size_t i = 0; i < 6; i++)
			for (size_t j = 0; j < 7; j++)
				assets3v3[i][j].show();

		for (size_t i = 0; i < 2; i++)
			for (size_t j = 0; j < 7; j++)
				assets1v1[i][j].hide();
	}*/
	return;
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
[[nodiscard]] QString *GetTrinkets(int lvl, string usedFighters[4]) {
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
		QString line = in.readLine();
		string lineFromFile = line.toStdString();
		if (lineFromFile == stopLine)
			break;
		if (lineFromFile.find("--") != string::npos) // skipping level delimeter
			continue;
		if (lineFromFile.find("[") != string::npos) { // means there somewhere symbol '['
			for (int i = 0; i < 4; i++)
				if (ParsingTrinket(lineFromFile, 1) == usedFighters[i]) {
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
		num = Random::get(0, num);
		if (possibleTrinkets[num] == ParsingTrinket(possibleTrinkets[num], 1)) {
			for (int i = 0; i < 8; i++)
				if (trinkets[i] == "" && (((i + 1) % 2 == 0 && possibleTrinkets[num] != trinkets[i - 1].toStdString()) || (i + 1) % 2 != 0)) {
					trinkets[i] = QString::fromStdString(possibleTrinkets[num]);
					break;
				}
		} else {
			for (int i = 0; i < 4; i++)
				if (ParsingTrinket(possibleTrinkets[num], 1) == usedFighters[i]) {
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
 * @param numCommand 1 or 2
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
		int index = possibleHeroes.size();			 // variable for storing index that has hero skills
		int numHero = Random::get(0, index - 1); // variable for storing hero index
		result[i] = QString::fromStdString(possibleHeroes[numHero]);
		possibleHeroes.erase(possibleHeroes.cbegin() + numHero);
	}
	return result;
}

/*
[[deprecated]] void MainWindow::Randomizing(int numCommand) {
	string usedFighters[4];
	int usedSpells[4][4]; // 0 for NA
	vector<string> possibleHeroes;
	vector<string> possibleSkills;
	// opening file
	string fileName;
	numCommand == 0 ? fileName = "BCR_T1.txt" : fileName = "BCR_T2.txt";
	if (!filesystem::exists(fileName) && !recreate(fileName)) {
		QMessageBox::critical(this, "Cannot create file", "For some reason BCR cannot create file for team random settings");
		return;
	}
	ifstream file(fileName);
	if (!file) {
		QMessageBox::critical(this, "Cannot open file", "For some reason BCR cannot open file for reading");
		return;
	}
	// analyzing which heroes and skills we can use for random
	for (int i = 0; i < NUMBER_OF_FIGHTERS; i++) {
		string line;
		file >> line;
		if (line[line.find(":") + 1] == '1') {
			possibleHeroes.push_back(fighters[i]); // saving hero
			string skills;
			for (unsigned int i = line.find(":") + 2; i < line.size(); i++)
				skills += line[i];
			possibleSkills.push_back(skills); // saving his skills
		}
	}
	// randomize first team
	// randomize heroes
	for (int i = 0; i < 4; i++) {
		int index = possibleHeroes.size();			 // variable for storing index that has hero skills
		int numHero = Random::get(0, index - 1); // variable for storing hero index
		index = numHero;												 // now we know where heroes spells located
		usedFighters[i] = possibleHeroes[numHero];
		// randomize spells
		if (usedFighters[i] == "abomination") {
			for (int j = 0; j < 4; j++)
				usedSpells[i][j] = 0;
		} else
			for (int j = 0; j < 4; j++) {
				int result = Random::get(1, 7);
				if (possibleSkills[index][result - 1] == '0') {
					j--;
					continue;
				}
				possibleSkills[index][result - 1] = '0';
				usedSpells[i][j] = result;
			}
		possibleSkills.erase(possibleSkills.cbegin() + index);
		possibleHeroes.erase(possibleHeroes.cbegin() + numHero);
	}
	for (int i = 0; i < 4; i++) {
		// heroes
		string temp = "background-image: url(:/heroes/heroes+spells/" + usedFighters[i] + "/hero_" + usedFighters[i] + ")";
		QString filePath = QString::fromStdString(temp), buttonName = "hero" + QString::number(i + (numCommand * 4) + 1);
		QList<QPushButton *> buttons = this->findChildren<QPushButton *>(buttonName);
		buttons[0]->setStyleSheet(filePath);
		buttons[0]->setToolTip(QString::fromStdString(usedFighters[i]));
		buttons[0]->show();
		heros[i + (numCommand * 4)] = QString::fromStdString(usedFighters[i]);
		// abilities
		for (int j = 0; j < 4; j++) {
			buttonName = "s" + QString::number(i + (numCommand * 4) + 1) + "_" + QString::number(j + 1);
			buttons = this->findChildren<QPushButton *>(buttonName);
			temp = "background-image: url(:/heroes/heroes+spells/" + usedFighters[i] + "/" + to_string(usedSpells[i][j]) + ".png)";
			filePath = QString::fromStdString(temp);
			if (usedSpells[i][j] != 0)
				buttons[0]->setStyleSheet(filePath);
			else
				buttons[0]->setStyleSheet("background-image: url(:/banner/heroes+spells/NA.png)");
			buttons[0]->show();
		}
	}
	QString *trinkets = GetTrinkets(numCommand == 0 ? this->ui->level1->value() : this->ui->level2->value(), usedFighters); // trinket randomization
	// trinkets
	for (int i = 0, j = 0; i < 4; i++, j += 2) {
		QString labelName = "t" + QString::number(i + (numCommand * 4) + 1) + "_1";
		string tempFP = "background-image: url(:/trinkets/trinkets/" + trinkets[j].toStdString() + ".png)";
		QString filePath = QString::fromStdString(tempFP);
		QList<QLabel *> trinketName = this->findChildren<QLabel *>(labelName);
		trinketName[0]->setStyleSheet(filePath);

		labelName = "t" + QString::number(i + (numCommand * 4) + 1) + "_2";
		trinketName.clear();
		trinketName = this->findChildren<QLabel *>(labelName);
		tempFP = "background-image: url(:/trinkets/trinkets/" + trinkets[j + 1].toStdString() + ".png)";
		filePath = QString::fromStdString(tempFP);
		trinketName[0]->setStyleSheet(filePath);
	}
	delete[] trinkets;
}*/

void MainWindow::on_doRandom_clicked() {
	// Randomizing(0);
	// Randomizing(1);
	
}

void MainWindow::on_level1_valueChanged(int arg1) {
	if (this->sameTeamLevel->isChecked())
		this->level[1]->setValue(arg1);
}

void MainWindow::on_level2_valueChanged(int arg1) {
	if (this->sameTeamLevel->isChecked())
		this->level[0]->setValue(arg1);
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
	QPixmap screenshot = this->grab();
	// Put the screenshot in the clipboard
	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setPixmap(screenshot);
}

void MainWindow::on_radio1t_clicked() {
	SwitchedTeamRadioButton();
}

void MainWindow::on_radio3t_clicked() {
	SwitchedTeamRadioButton();
}