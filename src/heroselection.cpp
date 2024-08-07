#include <filesystem>

#include <QBoxLayout>
#include <QMessageBox>

#include "../forms/ui_heroselection.h"

#include "heroselection.h"
#include "filemanip.h"
#include "config.h"

/**
 * @brief Construct a new Hero Selection:: Hero Selection object
 * 
 * @param parent 
 * @param numTeam 
 */
HeroSelection::HeroSelection(QWidget* const parent, uint8_t numTeam) : QDialog(parent), ui(new Ui::HeroSelection) {
	ui->setupUi(this);

	this->setFixedSize(1065, 342);
	this->setWindowTitle("Random settings");
	this->setStyleSheet("background-color: #323232");

	this->fileName = "BCR_T" + std::to_string((uint16_t)numTeam + 1) + ".txt";
	// we should analyze how many heroes accessible for random
	if (!std::filesystem::exists(this->fileName)) {
		if (!recreate(this->fileName)) // if file does not exist and cannot be created
			std::terminate();
		this->AccessibleHeroes = NUMBER_OF_FIGHTERS;
	} else { // checking how many heroes enabled
		this->AccessibleHeroes = 0;
		std::ifstream file(this->fileName);
		if (!file) {
			QMessageBox::critical(this, "Cannot open file", "For some reason BCR cannot open file BCR_T(1,2,...)");
			std::terminate();
		}
		for (int i = 0; i < NUMBER_OF_FIGHTERS; i++) {
			std::string line;
			file >> line;
			if (line[line.find(":") + 1] == '1')
				this->AccessibleHeroes++;
			int spellsAvailable = 0; // also checking that hero can random 4 spells
			for (unsigned int j = line.find(":") + 2; j < line.size(); j++)
				if (line[j] == '1')
					spellsAvailable++;
			if (spellsAvailable < 4) {
				this->AccessibleHeroes = NUMBER_OF_FIGHTERS;
				QMessageBox::warning(this, "Random settings analyze", "One of heroes had less than 4 skills for randomizing, file will be recreated");
				recreate(this->fileName);
				break;
			}
		}
		if (this->AccessibleHeroes < 4) {
			QMessageBox::warning(this, "Random settings analyze", "Less than 4 heroes where set for randomizing, file will be recreated");
			recreate(this->fileName);
			this->AccessibleHeroes = NUMBER_OF_FIGHTERS;
		}
	}

	this->buttons = new QPushButton** [NUMBER_OF_FIGHTERS];
	for (int i = 0; i < NUMBER_OF_FIGHTERS; i++)
		this->buttons[i] = new QPushButton* [8]; // 7 spells + hero

	// initializing
	QWidget *wgtMain = new QWidget();
	QVBoxLayout *vboxMain = new QVBoxLayout(wgtMain);
	for (int i = 0; i < NUMBER_OF_FIGHTERS; i++) {
		QWidget *wgtSub = new QWidget();
		QHBoxLayout *hBoxSub = new QHBoxLayout(wgtSub);
		for (int j = 0; j < 8; j++) {
			this->buttons[i][j] = new QPushButton();
			buttons[i][j]->setFixedSize(QSize(75, 80));
			hBoxSub->addWidget(buttons[i][j]);
			connect(this->buttons[i][j], SIGNAL(clicked()), this, SLOT(ButtonClicked()));
		}
		vboxMain->addWidget(wgtSub);
	}
	// updating ui
	for (int i = 0; i < NUMBER_OF_FIGHTERS; i++)
		updateUiLine(i + 1);
	this->ui->heroes->setWidget(wgtMain);
}

bool HeroSelection::updateUiLine(const int line) {
	//! \todo Check  line's borders (for out of range values)
	std::ifstream file(this->fileName);
	if (!file)
		return false;
	// getting statistic line
	std::string lines;
	for (int i = 0; i < line; i++)
		getline(file, lines);
	file.close();

	// analysing and changing
	size_t pos = lines.find(":");
	pos++;
	std::string color;
	lines[pos] == '0' ? color = "Red;" : color = "Green;";
	std::string style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" + fighters[line - 1] + "/hero_" + fighters[line - 1] + ")"; // changing hero frame
	buttons[line - 1][0]->setStyleSheet(QString::fromStdString(style));
	pos++;
	for (unsigned int i = pos; i < pos + 7; i++) { // 7 since hero was checked before
		lines[i] == '0' ? color = "Red;" : color = "Green;";
		style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" + fighters[line - 1] + "/" + std::to_string(i - pos + 1) + ".png)";
		buttons[line - 1][i - pos + 1]->setStyleSheet(QString::fromStdString(style));
	}
	return true;
}

void HeroSelection::ButtonClicked(void) {
	using namespace Errors;
	QPushButton *button = (QPushButton *)sender();
	for (int x = 0; x < NUMBER_OF_FIGHTERS; x++)
		for (int c = 0; c < 8; c++)
			if (this->buttons[x][c] == button) {
				switch (changeLine(this->fileName, fighters[x], c, this->AccessibleHeroes)) {
				case ChangeLine::HeroRemoved:
					this->AccessibleHeroes--;
					break;
				case ChangeLine::HeroAdded:
					this->AccessibleHeroes++;
					break;
				case ChangeLine::NoFile:
					QMessageBox::critical(this, "File open error", "Could not open file BCR_T(1,2)");
					std::terminate();
					break;
				case ChangeLine::TooFewHeroes:
					QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 heroes for randomizing");
					break;
				case ChangeLine::TooFewSpells:
					QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 spells for hero");
					break;
				case ChangeLine::HeroForbidden:
					QMessageBox::warning(this, "Random settings analyze", "B.r.u.h.");
					break;
				case ChangeLine::SkillRemoved: default:
					break;
				}
				updateUiLine(x + 1);
				return;
			}
}

HeroSelection::~HeroSelection() {
	delete ui;
}
