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

    while(1){
        std::string line;
        file >> line;
        if(file.eof())
            return false;
        if(line.find(heroName) != std::string::npos){
            file.seekp(-line.size(), std::ios::cur); // moving back in file
            // finding pos of first
            size_t posStart = 0;
            while(line[posStart] != ':')
                posStart++;
            posStart++;
            if(what == 0 && AccessableHeroes == 4 && line[posStart+what] == '1')
                return -2;
            else if(what == 0 && line[posStart+what] == '1')
                returnCode = 0;
            else if(what == 0 && line[posStart+what] == '0')
                returnCode = 1;
            else
                returnCode = 3;
            int checkSpells = 0;
            for(unsigned int i = posStart+1; i < line.length(); i++)
                if(line[i] == '1') checkSpells++;
            if(checkSpells == 4 && line[posStart+what] == '1' && what != 0)
                return -3;
            line[posStart+what] == '0' ? line[posStart+what] = '1' : line[posStart+what] = '0';
            line += '\n';
            file << line;
            break;
        }
    }

    return returnCode;
}
