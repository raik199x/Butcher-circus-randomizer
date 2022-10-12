#include "mainwindow.h"
#include "description.h"
#include "ui_mainwindow.h"

#include "Random.h"
#include <QDebug>
#include <string>
#include <fstream>
#include <vector>
#include <QFile>

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
    this->setFixedSize(QSize(1192, 665));

    this->setStyleSheet("color: #FFFFFF");
    this->ui->level1->setMaximum(75);
    this->ui->level2->setMaximum(75);


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
/*
 * mode == 1 -- trinket_name[leper] -> leper
 * mode == 2 -- trinket_name[leper] -> trinket_name
 */
string ParsingTrinket(string line, int mode){
    string result;
    if (line.find("[") > 100)
        return line;
    if(mode == 1)
        for(unsigned int i = line.find("[")+1; i < line.size()-1; i++) // obtaining hero name
            result += line[i];
    else if (mode == 2)
        for(unsigned int i = 0; i < line.find("["); i++) // obtaining hero name
            result += line[i];
    return result;
}

QString* GetTrinkets(int lvl, string usedFighters[4]){
    vector<string> possibleTrinkets;
    QFile trinketList(":/trinkets/trinkets/list.txt");
    //std::ifstream trinketList(file.toStdString());
    if (!trinketList.open(QIODevice::ReadOnly)){
        qDebug() << "ERROR";
        std::terminate();
    }
    QTextStream in(&trinketList);
    string stopLine;
    // guessing stop line
    if (lvl >= 69)
        stopLine = "-----";
    else{
        stopLine = "--" + to_string(lvl+1);
        lvl < 10 ? stopLine+="--" : stopLine+="-";
    }
    // obtaining possible trinkets for team
    while(true){
        QString line = in.readLine();
        string lineFromFile = line.toStdString();
        if(lineFromFile == stopLine)
            break;
        if(lineFromFile.find("--") < 100)  // skipping level delimeter
            continue;
        if(lineFromFile.find("[") < 100){ // means there somewhere symbol '['
            for(int i = 0; i < 4; i++)
                if(ParsingTrinket(lineFromFile,1) == usedFighters[i]){
                    possibleTrinkets.push_back(lineFromFile);
                    break;
                }
        } else
            possibleTrinkets.push_back(lineFromFile);
    }
    QString *trinkets = new QString[8];
    // randoming
    while(true){
        bool escape = true;
        for(int i = 0; i < 8; i++)
            if(trinkets[i] == ""){
                escape = false;
                break;
            }
        if(escape == true) return trinkets;
        int num = possibleTrinkets.size()-1;
        num = Random::get(0,num);
        if(possibleTrinkets[num] == ParsingTrinket(possibleTrinkets[num],1)){
            for(int i = 0; i < 8; i++)
                if(trinkets[i] == "" && (((i+1) % 2 == 0 && possibleTrinkets[num] != trinkets[i-1].toStdString()) || (i+1) % 2 != 0)){
                    trinkets[i] = QString::fromStdString(possibleTrinkets[num]);
                    break;
                }
        } else{
            for(int i = 0; i < 4; i++)
                if(ParsingTrinket(possibleTrinkets[num],1) == usedFighters[i]){
                    if(trinkets[i*2] != "" && trinkets[i*2+1] != "")
                        break;
                    trinkets[i*2] == "" ? trinkets[i*2] = QString::fromStdString(ParsingTrinket(possibleTrinkets[num],2)) : trinkets[i*2+1] = QString::fromStdString(ParsingTrinket(possibleTrinkets[num],2));
                    break;
                }
        }
        possibleTrinkets.erase(possibleTrinkets.cbegin()+num);
    }
    return trinkets;
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
    QString *trinkets = GetTrinkets(numCommand == 0 ? this->ui->level1->value() : this->ui->level2->value(), usedFighters); // trinket randomization
    //trinkets
    for(int i = 0, j = 0; i < 4;i++,j+=2){
         QString labelName = "t" + QString::number(i+(numCommand*4)+1) + "_1";
         string tempFP = "background-image: url(:/trinkets/trinkets/" + trinkets[j].toStdString() +".png)";
         QString filePath = QString::fromStdString(tempFP);
         QList<QLabel *>trinketName = this->findChildren<QLabel *>(labelName);
         trinketName[0]->setStyleSheet(filePath);

         labelName = "t" + QString::number(i+(numCommand*4)+1) + "_2";
         trinketName.clear();
         trinketName = this->findChildren<QLabel *>(labelName);
         tempFP = "background-image: url(:/trinkets/trinkets/" + trinkets[j+1].toStdString() +".png)";
         filePath = QString::fromStdString(tempFP);
         trinketName[0]->setStyleSheet(filePath);
    }
    delete [] trinkets;
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


void MainWindow::on_level1_valueChanged(int arg1)
{
    if(this->ui->sameTeamLevel->isChecked())
        this->ui->level2->setValue(arg1);
}


void MainWindow::on_level2_valueChanged(int arg1)
{
    if(this->ui->sameTeamLevel->isChecked())
        this->ui->level1->setValue(arg1);
}


void MainWindow::on_sameTeamLevel_clicked()
{
    if(this->ui->sameTeamLevel->isChecked())
        this->ui->level2->setValue(this->ui->level1->value());
}

