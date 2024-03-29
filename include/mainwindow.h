#pragma once

#include "config.h"
#include <QHBoxLayout>
#include <QMainWindow>
#include <QRadioButton>
#include <QCheckBox>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include "random.h"

/**
 * @file mainwindow.h
 * @author raik
 * @brief header file for MainWindow class (Main ui)
 *
 */
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

	void on_level_valueChanged(int arg1);

	void on_sameTeamLevel_clicked();

	void on_RandomSettings1_clicked();

	void on_RandomSettings2_clicked();

	void on_screenShot_clicked();

	void on_radio_clicked();

	void on_muteAncestor_clicked();

private:
	bool playVoice;
	QWidget *ui;
	// Recreating mainwindow.ui
	QPushButton **RandomSettings;
	QPushButton *screenShot;
	QPushButton *doRandom;

	QCheckBox *sameTeamLevel;
	QCheckBox *muteAncestor;

	QRadioButton *radio1t;
	QRadioButton *radio3t;
	QSpinBox **level;

	QVBoxLayout *layout;
	
	QVBoxLayout *leftSide;
	QVBoxLayout *rightSide;

	void Randomizing(int numCommand);
	QString *GetFighters(int numCommand);
    QString *GetSkills(int numCommand, QString *Fighters);
	QString *GetTrinkets(int lvl, QString *usedFighters);
	void ClearLayout(QLayout *layout);

	//! \note Tricky prng
	Random::Tricky<uint>* prng;
};
