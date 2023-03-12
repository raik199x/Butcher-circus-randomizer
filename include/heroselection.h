#pragma once

#include <QDialog>
#include <QPushButton>

/**
 * @file heroselection.h
 * @author raik
 * @brief header file for HeroSelection class
 *
 */

namespace Ui {
	class HeroSelection;
}

class HeroSelection : public QDialog {
	Q_OBJECT

public:
	explicit HeroSelection(QWidget* const parent = nullptr, uint8_t numTeam = 0);
	~HeroSelection(void);

private:
	QPushButton ***buttons;
	Ui::HeroSelection *ui;
	std::string fileName;
	int AccessibleHeroes;

	bool updateUiLine(const int line);

private slots:
	void ButtonClicked(void);
};
