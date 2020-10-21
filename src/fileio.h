//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_FILEIO_H
#define VIRUS_FILEIO_H

//#include <iostream>
#include <fstream>
#include <stdexcept>
#include "grid.h"
#include "field.h"
//for making a directory
#include <sys/stat.h>


class FileIO {
    std::fstream saveFileConfiguration;
    std::fstream saveFileFieldData;
    std::fstream saveFileDistrictData;
    std::fstream saveFileCountryData;
    Grid *grid;

    void saveFieldsLastData(size_t tickID);

    void saveDistrictsLastData(size_t tickID);

    void saveCountryLastData(size_t tickID);

    void loadConfiguration();

    void loadFieldsLastData();

    void loadDistrictsLastData();

    void loadCountryLastData();

public:
    FileIO(char *saveFolderPath, Grid *grid = nullptr, const char *saveFileConfigurationDataName = nullptr,
           const char *saveFileFieldsDataName = nullptr, const char *saveFileDistrictsDataName = nullptr,
           const char *saveFileCountriesDataName = nullptr) {
        //try to create directory
        if (mkdir(saveFolderPath, 0777) == -1) { throw std::runtime_error("save directory couldn't be created"); }
        //open files, and give default name, if override name is not provided
        saveFileConfiguration = std::fstream(
                saveFileConfigurationDataName == nullptr ? "configuration.dat" : saveFileConfigurationDataName,
                std::ios::in | std::ios::app);
        saveFileFieldData = std::fstream(
                saveFileFieldsDataName == nullptr ? "fields.dat" : saveFileFieldsDataName,
                std::ios::in | std::ios::app);
        saveFileDistrictData = std::fstream(
                saveFileDistrictsDataName == nullptr ? "districts.dat" : saveFileDistrictsDataName,
                std::ios::in | std::ios::app);
        saveFileCountryData = std::fstream(
                saveFileCountriesDataName == nullptr ? "countries.dat" : saveFileCountriesDataName,
                std::ios::in | std::ios::app);
        //check for errors simultaneously in all 4 file rw
        if (!(saveFileConfiguration.is_open() || saveFileFieldData.is_open() || saveFileDistrictData.is_open() ||
              saveFileCountryData.is_open())) { throw std::runtime_error("fileIO saveFile couldn't be opened"); }
        this->grid = grid;
        //create headers
        saveFileFieldData
                << "tickID: id1_vaccination, id1_infection, id2_vaccination, id2_infection, id3_vaccination, ..."
                << std::endl;
        saveFileDistrictData << "tickID: id1_clear, id2_clear, ..." << std::endl;
        saveFileCountryData
                << "tickID: id1_reservedVaccines, id1_totalProdCap, id2_reservedVaccines, id2_totalProdCap, id3_reservedVaccines, ..."
                << std::endl;
    }

    void setGrid(Grid *grid) { this->grid = grid; }

    void saveConfiguration();

    void saveTickData(size_t tickID) {
        saveFieldsLastData(tickID);
        saveDistrictsLastData(tickID);
        saveCountryLastData(tickID);
    }

    void load();

    ~FileIO() {
        saveFileConfiguration.flush();
        saveFileConfiguration.close();
        saveFileFieldData.flush();
        saveFileFieldData.close();
        saveFileDistrictData.flush();
        saveFileDistrictData.close();
        saveFileCountryData.flush();
        saveFileCountryData.close();
    }
};


#endif //VIRUS_FILEIO_H
