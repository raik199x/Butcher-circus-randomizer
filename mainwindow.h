#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_doRandom_clicked();
    void on_hero1_clicked();

private:
    Ui::MainWindow *ui;
    std::string fighters[18];
    QString hero[8];
    void Randoming(int numCommand);
};
#endif // MAINWINDOW_H
