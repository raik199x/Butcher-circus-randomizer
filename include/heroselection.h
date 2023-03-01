#pragma once

#include <QDialog>
#include <QPushButton>

namespace Ui {
	class HeroSelection;
}

class HeroSelection : public QDialog {
	Q_OBJECT

public:
	explicit HeroSelection(QWidget *parent = nullptr, int numTeam = 0);
	~HeroSelection(void);

private:
	QPushButton ***buttons;
	Ui::HeroSelection *ui;
	std::string fileName;
	int AccessibleHeroes;

	bool updateUiLine(int line);

private slots:
	void ButtonClicked(void);
};
