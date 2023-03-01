#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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
	std::string fighters[18];
	QString hero[8];

	void Randoming(int numCommand);
	QString *GetFighters(int numCommand);
	QString *GetSkills(int numCommand, QString *Fighters);
};
#endif // MAINWINDOW_H
