#include "../include/mainwindow.h"
#include "../forms/ui_mainwindow.h"
#include "../include/heroselection.h"

#include "../include/random.h"
#include "../include/filemanip.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <fstream>
#include <qobject.h>
#include <string>
#include <vector>

/*!
 * \file mainwindow.cpp
 * \brief MainWindow Interface
 *
 * \author raik
 */

using namespace std;

MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	for (int i = 0; i < NUMBER_OF_HEROS; i++)
		heros[i] = "NULL";
	this->ui->statusbar->hide();
	this->setFixedSize(QSize(1192, 665));

	// setting colors
	this->setStyleSheet("color: #FFFFFF");
	this->ui->RandomSettings1->setStyleSheet("background-color: #3F3F3F");
	this->ui->RandomSettings2->setStyleSheet("background-color: #3F3F3F");
	this->ui->doRandom->setStyleSheet("background-color: #3F3F3F");
	this->ui->level1->setStyleSheet("background-color: #242424");
	this->ui->level2->setStyleSheet("background-color: #242424");

	this->ui->level1->setMaximum(75);
	this->ui->level2->setMaximum(75);

	for (int i = 0; i < NUMBER_OF_HEROS; i++) {
		QString block = "hero" + QString::number(i + 1);
		QList<QPushButton *> button = this->findChildren<QPushButton *>(block);
		button[0]->hide();
		for (int j = 0; j < 4; j++) {
			QString spell = "s" + QString::number(i + 1) + "_" + QString::number(j + 1);
			QList<QPushButton *> spellButton = this->findChildren<QPushButton *>(spell);
			spellButton[0]->hide();
		}
	}
}

MainWindow::~MainWindow() {
	delete ui;
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
QString *GetTrinkets(int lvl, string usedFighters[4]) {
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
		if (lineFromFile.find("--") < 100) // skipping level delimeter
			continue;
		if (lineFromFile.find("[") < 100) { // means there somewhere symbol '['
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
 * @return QString*
 */
QString *MainWindow::GetFighters(int numCommand) {
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

void MainWindow::Randomizing(int numCommand) {
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
}

void MainWindow::on_doRandom_clicked() {
	Randomizing(0);
	Randomizing(1);
}

void MainWindow::on_level1_valueChanged(int arg1) {
	if (this->ui->sameTeamLevel->isChecked())
		this->ui->level2->setValue(arg1);
}

void MainWindow::on_level2_valueChanged(int arg1) {
	if (this->ui->sameTeamLevel->isChecked())
		this->ui->level1->setValue(arg1);
}

void MainWindow::on_sameTeamLevel_clicked() {
	if (this->ui->sameTeamLevel->isChecked())
		this->ui->level2->setValue(this->ui->level1->value());
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
