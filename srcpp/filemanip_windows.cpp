#include "filemanip.h"

bool recreate(std::string fileName, std::string *fighters){
    std::ofstream file(fileName);
    if(!file)
        return true;
    for(int i = 0; i < 18; i++){
        std::string line = fighters[i] + ":" + std::string(8,'1') + '\n';
        file << line;
    }
    file.close();
    return false;
}

int changeLine(std::string fileName, std::string heroName, int what, int AccessableHeroes){
    int returnCode;
    if(heroName == "abomination" && what != 0)
        return -4;
    std::fstream file(fileName);
    if(!file)
        return -1;

    std::string content[18]; unsigned int i = 0, pos;
    // obtaining list of heroes
    while(i < 18){
        getline(file,content[i++]);
        if(content[i-1].find(heroName) != std::string::npos)
            pos = i - 1;
    }
    unsigned int separator_pos = content[pos].find(":")+1; // needs for future
    if(what == 0 && AccessableHeroes == 4 && content[pos][separator_pos+what] == '1') // need to be at least 4 heroes
        return -2;
    else if(what == 0 && content[pos][separator_pos+what] == '1') // setted  0 (need to lower counter)
        returnCode = 0;
    else if(what == 0 && content[pos][separator_pos+what] == '0') // setted 1 (need to up counter)
        returnCode = 1;
    else
        returnCode = 3;                             // spell changed (but still need to check)

    // changing number + checking
    unsigned int checkSpells = 0;
    for(i = separator_pos; i < content[pos].length(); i++)
        if(content[pos][i] == '1') checkSpells++;
    if(checkSpells == 4 && content[pos][separator_pos+what] == '1' && what != 0)
        return -3;
    content[pos][separator_pos+what] == '0' ? content[pos][separator_pos+what] = '1' : content[pos][separator_pos+what] = '0';

    // writting changes to a file
    file.close();
    file.open(fileName);
    for(i = 0; i < 18; i++)
        file << content[i] << "\n";
    file.close();
    return returnCode;
}
