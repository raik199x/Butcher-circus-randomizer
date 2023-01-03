#include "heroselection.h"
#include "qboxlayout.h"
#include "ui_heroselection.h"
#include "filemanip.h"
#include <filesystem>
#include <QMessageBox>

HeroSelection::HeroSelection(QWidget *parent, std::string *fighters, int numTeam) :
    QDialog(parent),
    ui(new Ui::HeroSelection)
{
    ui->setupUi(this);

    this->setFixedSize(1065,342);
    this->setWindowTitle("Random settings");
    this->setStyleSheet("background-color: #323232");

    for(int i = 0; i < 18; i++)
        this->fighters[i] = fighters[i];

    numTeam == 0 ? this->fileName = "BCR_T1.txt" : this->fileName = "BCR_T2.txt";
    // we should analyze how many heros accesable for random
    if(!std::filesystem::exists(this->fileName)){
        if(!recreate(this->fileName, fighters)) // if file does not exist and cannot be created
            std::terminate();
        this->AccessableHeroes = 18;
    } else{ // checking how many heroes enabled
        this->AccessableHeroes = 0;
        std::ifstream file(this->fileName);
        if(!file){
            QMessageBox::critical(this, "Cannot open file", "For some reason BCR cannot open file BCR_T(1,2)");
            std::terminate();
        }
        for(int i = 0; i < 18; i++){
            std::string line;
            file >> line;
            if(line[line.find(":")+1] == '1')
                this->AccessableHeroes++;
            int spellsAvailable = 0;                            // also checking that hero can random 4 spells
            for(unsigned int j = line.find(":")+2; j < line.size(); j++)
                if(line[j] == '1') spellsAvailable++;
            if(spellsAvailable < 4){
                this->AccessableHeroes = 18;
                QMessageBox::warning(this, "Random settings analyze", "One of heroes had less than 4 skills for randoming, file will be recreated");
                recreate(this->fileName,this->fighters);
                break;
            }
        }
        if(this->AccessableHeroes < 4){
            QMessageBox::warning(this, "Random settings analyze", "Less than 4 heroes where set for randoming, file will be recreated");
            recreate(this->fileName,this->fighters);
            this->AccessableHeroes = 18;
        }
    }

    this->buttons = new QPushButton**[18];
    for(int i = 0; i < 18; i++)
        this->buttons[i] = new QPushButton*[8];

    // initialazing
    QWidget *wgtMain = new QWidget();
    QVBoxLayout *vboxMain = new QVBoxLayout(wgtMain);
    for(int i = 0; i < 18;i++){
        QWidget *wgtSub = new QWidget();
        QHBoxLayout *hboxSub = new QHBoxLayout(wgtSub);
        for(int j = 0; j < 8;j++){
            this->buttons[i][j] = new QPushButton();
            buttons[i][j]->setFixedSize(QSize(75,80));
            hboxSub->addWidget(buttons[i][j]);
            connect(this->buttons[i][j], SIGNAL(clicked()),this,SLOT(ButtonClicked()));
        }
        vboxMain->addWidget(wgtSub);
    }
    // updating ui
    for(int i = 0; i < 18; i++)
        updateUiLine(i+1);
    this->ui->heroes->setWidget(wgtMain);
}

bool HeroSelection::updateUiLine(int line){
    std::ifstream file(this->fileName);
    if(!file)
        return false;
    // getting statistic line
    std::string lines;
    for(int i = 0; i < line; i++)
        getline(file,lines);
    file.close();

    // analyzing and changing
    size_t pos = lines.find(":"); pos++;
    std::string color;
    lines[pos] == '0' ? color = "Red;" : color = "Green;";
    std::string style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" + this->fighters[line-1] + "/hero_"+ this->fighters[line-1] +")"; // changing hero frame
    buttons[line-1][0]->setStyleSheet(QString::fromStdString(style));
    pos++;
    for(unsigned int i = pos; i < pos+7; i++){ // 7 since hero was checked before
        lines[i] == '0' ? color = "Red;" : color = "Green;";
        style = "background-color: " + color + " background-image: url(:/heroes/heroes+spells/" + this->fighters[line-1] + "/" + std::to_string(i-pos+1) +".png)";
        buttons[line-1][i-pos+1]->setStyleSheet(QString::fromStdString(style));
    }
    return true;
}

void HeroSelection::ButtonClicked(){
    QPushButton *button = (QPushButton*) sender();
    for(int x = 0; x < 18; x++)
        for(int c = 0; c < 8; c++)
            if(this->buttons[x][c] == button){
                switch(changeLine(this->fileName,this->fighters[x],c,this->AccessableHeroes)){
                    case 0: this->AccessableHeroes--; break;
                    case 1: this->AccessableHeroes++; break;
                    case 3: break;
                    case -1: QMessageBox::critical(this, "File open error", "Could not open file BCR_T(1,2)"); std::terminate(); break;
                    case -2: QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 heroes for randoming"); break;
                    case -3: QMessageBox::warning(this, "Random settings analyze", "You are trying to set less than 4 spells for hero"); break;
                    case -4: QMessageBox::warning(this, "Random settings analyze", "Bruh"); break;
                    default: break;
                }
                updateUiLine(x+1);
                return;
            }

}

HeroSelection::~HeroSelection(){
    delete ui;
}
