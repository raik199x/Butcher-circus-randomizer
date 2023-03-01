#ifndef HEROSELECTION_H
#define HEROSELECTION_H

#include "qpushbutton.h"
#include <QDialog>

namespace Ui {
class HeroSelection;
}

class HeroSelection : public QDialog {
	Q_OBJECT

public:
	explicit HeroSelection(QWidget *parent = nullptr, std::string *fighters = nullptr, int numTeam = 0);
	~HeroSelection();

private:
	QPushButton ***buttons;
	Ui::HeroSelection *ui;
	std::string fighters[18];
	std::string fileName;
	int AccessableHeroes;

	bool updateUiLine(int line);
private slots:
	void ButtonClicked();
};

#endif // HEROSELECTION_H
