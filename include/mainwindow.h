#pragma once

#include <QMainWindow>
#include "config.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_doRandom_clicked();

	void on_level1_valueChanged(int arg1);

	void on_level2_valueChanged(int arg1);

	void on_sameTeamLevel_clicked();

	void on_RandomSettings1_clicked();

	void on_RandomSettings2_clicked();

private:
	Ui::MainWindow *ui;
	QString heros[NUMBER_OF_HEROS];

	void Randomizing(int numCommand);
	QString *GetFighters(int numCommand);
	QString *GetSkills(int numCommand, QString *Fighters);
};
