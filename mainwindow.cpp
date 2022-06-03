#include "mainwindow.h"
#include "description.h"
#include "ui_mainwindow.h"

#include "Random.h"
#include <QDebug>
#include <string>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = 0; i < 8; i++)
        hero[i] = "NULL";
    this->fighters[0] = "abomination";
    this->fighters[1] = "antiquarian";
    this->fighters[2] = "arbalest";
    this->fighters[3] = "bounty_hunter";
    this->fighters[4] = "crusader";
    this->fighters[5] = "flagellant";
    this->fighters[6] = "grave_robber";
    this->fighters[7] = "hellion";
    this->fighters[8] = "highwayman";
    this->fighters[9] = "houndmaster";
    this->fighters[10] = "jester";
    this->fighters[11] = "leper";
    this->fighters[12] = "man_at_arms";
    this->fighters[13] = "musketeer";
    this->fighters[14] = "occultist";
    this->fighters[15] = "plague_doctor";
    this->fighters[16] = "shieldbreaker";
    this->fighters[17] = "vestal";
    this->ui->statusbar->hide();
    this->setFixedSize(QSize(980, 550));
    this->ui->doRandom->setStyleSheet("color: #FFFFFF; background-color: #2F4F4F");
    this->ui->t1Flagellant->setStyleSheet("color: #FFFFFF");

    for(int i = 0; i < 8; i++){
        QString block = "hero" + QString::number(i+1);
        QList<QPushButton *> button = this->findChildren<QPushButton *>(block);
        button[0]->hide();
        for(int j = 0; j < 4; j++){
            QString spell = "s" + QString::number(i+1) + "_" + QString::number(j+1);
            QList<QPushButton *> spellButton = this->findChildren<QPushButton *>(spell);
            spellButton[0]->hide();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Randoming(int numCommand){
    string usedFighters[4];
    int usedSpells[4][4]; // 0 for NA
    bool flagellantEnterned = false, shieldbreakerEnterned = false, musketeerEnterned = false;
    //randoming first team
        //randoming heroes
    for(int i = 0; i < 4; i++){
        usedFighters[i] = this->fighters[Random::get(0,17)];
        //setting flag for future check
        if(usedFighters[i] == "shieldbreaker")
            shieldbreakerEnterned = true;
        else if(usedFighters[i] == "flagellant")
            flagellantEnterned = true;
        else if(usedFighters[i] == "musketeer")
            musketeerEnterned = true;

        //randoming spells
        if(usedFighters[i] == "abomination"){
            for(int j = 0; j < 4; j++)
                usedSpells[i][j] = 0;
        } else
            for(int j = 0; j < 4; j++){
                usedSpells[i][j] = Random::get(1,7);
                //also checking if such spells already taken
                for(int z = 0; z < j; z++)
                    if(usedSpells[i][z] == usedSpells[i][j]){
                        j--;
                        break;
                    }
            }
        //checking if we can paste randomed hero
        if(numCommand == 0){    //checking for first team
            if(this->ui->t1Shieldbreaker->isChecked() == false && shieldbreakerEnterned == true){
                shieldbreakerEnterned = false;
                i--;
                continue;
            } else if(this->ui->t1Musketeer->isChecked() == false && musketeerEnterned == true){
                musketeerEnterned = false;
                i--;
                continue;
            } else if(this->ui->t1Flagellant->isChecked() == false && flagellantEnterned == true){
                flagellantEnterned = false;
                i--;
                continue;
            }
        } else{     //checking for second team
            if(this->ui->t2Shieldbreaker->isChecked() == false && shieldbreakerEnterned == true){
                shieldbreakerEnterned = false;
                i--;
                continue;
            } else if(this->ui->t2Musketeer->isChecked() == false && musketeerEnterned == true){
                musketeerEnterned = false;
                i--;
                continue;
            } else if(this->ui->t2Flagellant->isChecked() == false && flagellantEnterned == true){
                flagellantEnterned = false;
                i--;
                continue;
            }
        }
        for(int j = 0; j < i; j++)
            if(usedFighters[j] == usedFighters[i]){
                i--;
                break;
            }
    }
    //now, when team info is ready, placing it in menu
    for(int i = 0; i < 4; i++){
        //heroes
        string temp = "background-image: url(:/heroes/heroes+spells/" + usedFighters[i] + "/hero_"+ usedFighters[i] +")";
        QString filePath = QString::fromStdString(temp), buttonName = "hero" + QString::number(i+(numCommand*4)+1);
        QList<QPushButton *> buttons = this->findChildren<QPushButton *>(buttonName);
        buttons[0]->setStyleSheet(filePath);
        buttons[0]->setToolTip( QString::fromStdString(usedFighters[i]));
        buttons[0]->show();
        hero[i+(numCommand*4)] = QString::fromStdString(usedFighters[i]);
        //abilities
        for(int j = 0; j < 4;j++){

            buttonName = "s" + QString::number(i+(numCommand*4)+1) + "_" + QString::number(j+1);
            buttons =  this->findChildren<QPushButton *>(buttonName);
            temp = "background-image: url(:/heroes/heroes+spells/" + usedFighters[i] + "/"+ to_string(usedSpells[i][j]) +".png)";
            filePath = QString::fromStdString(temp);
            if(usedSpells[i][j] != 0)
                buttons[0]->setStyleSheet(filePath);
            else
                buttons[0]->setStyleSheet("background-image: url(:/banner/heroes+spells/NA.png)");
            buttons[0]->show();
        }
    }
}

void MainWindow::on_doRandom_clicked()
{
    Randoming(0);
    Randoming(1);
}


void MainWindow::on_hero1_clicked()
{
    Description *window = new Description;
    window->setModal(true);
    window->exec();
}

