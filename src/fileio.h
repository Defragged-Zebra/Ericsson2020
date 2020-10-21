//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_FILEIO_H
#define VIRUS_FILEIO_H

//#include <iostream>
#include <fstream>
#include <stdexcept>
#include "grid.h"

class FileIO {
    std::fstream saveFile;
    Grid *grid;
    void saveConfiguration();
public:
    FileIO(char *saveFilePath, Grid *grid = nullptr) {
        saveFile = std::fstream(saveFilePath, std::ios::in | std::ios::app);
        if (!saveFile.is_open()) { throw std::runtime_error("fileIO saveFile couldn't be opened"); }
        this->grid = grid;
    }

    void setGrid(Grid *grid) { this->grid = grid; }

    void changeSaveFile(char *newName) {
        saveFile.flush();
        saveFile.close();
        saveFile = std::fstream(newName, std::ios::in | std::ios::app);
        if (!saveFile.is_open()) { throw std::runtime_error("fileIO saveFile couldn't be opened"); }
    }

    void saveAll()const;

    void load();

    ~FileIO() {
        saveFile.flush();
        saveFile.close();
    }
};


#endif //VIRUS_FILEIO_H
