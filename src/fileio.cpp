//
// Created by woranhun on 2020. 10. 17..
//

#include "fileio.h"

/*
 * saveFile structure:
 *
 *
 * section1: all the countries:
 *          id1
 *          total_production capacity, reserves
 * section2: all the districts with there parameter as CSV
 * section3: all the fields with there parameter as CSV
 *
 *
 *
 * Values that change: Field.infection, Field.vaccinationRate, Country.reserveVaccines, Country.totalProductionCapacity, District.clear
 * which translates to files: configuration -- created, and remains constant
 *                            Field         -- csv with id-s on top, with a lower level key of infections, vaccination
 *                            District      -- csv with districtID-s as header
 *                            Country       -- csv with id-s on top, with a lower level key of tot_prod_cap, reserves
 *
 * file configuration:
 *    section1: grid x-y to id codes:
 *          fieldID[0][0]   fieldID[1][0]   fieldID[2][0]   ...
 *          fieldID[0][1]   fieldID[1][1]   fieldID[2][1]   ...
 *          fieldID[0][2]   fieldID[1][2]   fieldID[2][2]   ...
 *               .               .               .        .
 *               .               .               .          .
 *               .               .               .            .
 *
 *    section2:
 *          districtID1: assignedFieldID1, assignedFieldID2, ....
 *          districtID2: assignedFieldID1, assignedFieldID2, ....
 *          .
 *          .
 *          .
 *
 *    section3:
 *          countryID1: assignedDistrictID1, assignedDistrictID1, ...
 *          countryID2: assignedDistrictID1, assignedDistrictID1, ...
 *          .
 *          .
 *          .
 *
 * file Fields:
 *      tickID: id1_vaccination, id1_infection, id2_vaccination, id2_infection, id3_vaccination, ...
 *      .
 *      .
 *      .
 *
 * file Districts:
 *      tickID: id1_clear, id2_clear, ...
 *      .
 *      .
 *      .
 *
 * file Countries:
 *      tickID: id1_reservedVaccines, id1_totalProdCap, id2_reservedVaccines, id2_totalProdCap, id3_reservedVaccines, ...
 *
 * */
void FileIO::saveConfiguration() {
    saveFileConfiguration << *grid << std::endl;
}

void FileIO::saveFieldsLastData(size_t tickID) {
    saveFileFieldData << tickID << ": ";
    for (int i = 0; i < grid->getX() * grid->getY(); ++i) {
        saveFileFieldData << grid->getFieldByID(i).getCurrentInfectionValue() << ", "
                          << grid->getFieldByID(i).getVaccinationRate() << ", ";
    }
    saveFileFieldData << std::endl;
}

void FileIO::saveDistrictsLastData(size_t tickID) {
    saveFileDistrictData << tickID << ": ";
    for (int i = 0; i < grid->getX() * grid->getY(); ++i) {
        saveFileDistrictData << grid->getDistrictByID(i).isClear() << ", ";
    }
    saveFileDistrictData << std::endl;
}

void FileIO::saveCountryLastData(size_t tickID) {
    saveFileCountryData << tickID<<": ";
    for (int i = 0; i < grid->getX() * grid->getY(); ++i) {
        saveFileCountryData << grid->getCountryByID(i).getReserveVaccines() << ", "
                            << grid->getCountryByID(i).getTotalProductionCapacity() << ", ";
    }
    saveFileCountryData << std::endl;
}

//WARNING: this might generate some problems if grid is not newly created
void FileIO::loadConfiguration() {
    //throw std::runtime_error("currently not implemented");
    size_t x = getXFromSaveFile();
    size_t y = getYFromSaveFile();
    std::string line;
    saveFileConfiguration.seekg(0, std::ios::beg);
    std::getline(saveFileConfiguration, line);
    std::getline(saveFileConfiguration, line);
    for (int i = 0; i < x; ++i) {
        std::getline(saveFileConfiguration, line);
        for (int j = 0; j < y; ++j) {
            size_t ID = std::stoi(line.substr(0, line.find(',')));
            grid->uploadGridWithFieldID(y, x, ID);
            line = line.substr(line.find(','));
        }
    }
    //creating Fields
    for (size_t ID = 0; ID < x * y; ++ID) {
        grid->addField(Field(ID, 0, 0, 0, 0, 0));
    }
    std::getline(saveFileConfiguration, line);
    std::stringstream ss;
    std::vector<size_t> fieldIDs = std::vector<size_t>();
    size_t FieldID;
    size_t districtID = 0;
    while (line.substr(0, 8) != "section3") {
        std::getline(saveFileConfiguration, line);
        ss << line;
        while (getline(ss, line, ',')) {
            std::stringstream(line) >> FieldID;
            fieldIDs.push_back(FieldID);
        }
        grid->addDistrict(District(districtID, fieldIDs, false));
        districtID++;
    }
    std::getline(saveFileConfiguration, line);
    std::vector<size_t> districtIDs = std::vector<size_t>();
    size_t CountryID = 0;
    while (!saveFileConfiguration.eof()) {
        std::getline(saveFileConfiguration, line);
        ss << line;
        while (getline(ss, line, ',')) {
            std::stringstream(line) >> districtID;
            fieldIDs.push_back(districtID);
        }
        grid->addCountry(Country(CountryID, districtIDs));
        CountryID++;
    }
}

void FileIO::loadFieldsLastData(size_t numberOfPastRecordToLoad, size_t x, size_t y) {
    for (int j = 0; j < x * y; ++j) {
        grid->getFieldByID(j).setNumberOfPastValues(numberOfPastRecordToLoad);
    }
    std::stringstream ss;
    std::string line;
    saveFileFieldData.seekg(0, std::ios::beg);
    std::getline(saveFileFieldData, line);
    size_t rows = 0;
    while (std::getline(saveFileFieldData, line)) rows++;
    saveFileFieldData.seekg(0, std::ios::beg);
    std::getline(saveFileFieldData, line);
    for (int i = 0; i < rows - numberOfPastRecordToLoad; ++i) {
        std::getline(saveFileFieldData, line);
    }
    //std::vector<int> pastInfections;
    int *vaccination;
    vaccination = new int[x * y];
    for (size_t i = rows - numberOfPastRecordToLoad; i < rows; ++i) {
        std::getline(saveFileFieldData, line);
        ss << line;
        size_t fieldID = 0;
        getline(ss, line, ':');
        while (getline(ss, line, ',')) {
            vaccination[fieldID] = std::stoi(line);
            getline(ss, line, ',');
            grid->getFieldByID(fieldID).updateInfection(std::stoi(line));
            fieldID++;
        }
    }
    for (int i = 0; i < x * y; ++i) {
        grid->getFieldByID(i).updateVaccination(vaccination[i]);
    }
    delete vaccination;
}

size_t FileIO::getYFromSaveFile() {
    saveFileConfiguration.seekg(0, std::ios::beg);
    std::string line;
    std::getline(saveFileConfiguration, line);
    std::getline(saveFileConfiguration, line);
    //size_t x = std::stod(line.substr(line.find(':'), line.find(' ') - 1));
    size_t y = std::stod(line.substr(line.rfind(' ', line.find('\n'))));
    return y;
}

size_t FileIO::getXFromSaveFile() {
    saveFileConfiguration.seekg(0, std::ios::beg);
    std::string line;
    std::getline(saveFileConfiguration, line);
    std::getline(saveFileConfiguration, line);
    size_t x = std::stod(line.substr(line.find(':'), line.find(' ') - 1));
    //size_t y = std::stod(line.substr(line.rfind(' ', line.find('\n'))));
    return x;

}

void FileIO::loadDistrictsLastData() {
    std::stringstream ss;
    std::string line;
    findLastLine(saveFileDistrictData);
    std::getline(saveFileDistrictData, line);
    ss << line;
    size_t districtID = 0;
    bool clear;
    while (getline(ss, line, ',')) {
        ss >> clear;
        grid->getDistrictByID(districtID).setClear(clear);
        districtID++;
    }
}

void FileIO::findLastLine(std::fstream &file) {
    //find last line by: https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file
    file.seekg(-1, std::ios_base::end);                // go to one spot before the EOF
    bool keepLooping = true;
    while (keepLooping) {
        char ch;
        file.get(ch);                            // Get current byte's data
        if ((int) file.tellg() <= 1) {             // If the data was at or before the 0th byte
            file.seekg(0);                       // The first line is the last line
            keepLooping = false;                // So stop there
        } else if (ch == '\n') {                   // If the data was a newline
            keepLooping = false;                // Stop at the current position.
        } else {                                  // If the data was neither a newline nor at the 0 byte
            file.seekg(-2, std::ios_base::cur);
            // Move to the front of that data, then to the front of the data before it
        }
    }
}

void FileIO::loadCountryLastData() {
    std::stringstream ss;
    std::string line;
    findLastLine(saveFileCountryData);
    std::getline(saveFileCountryData, line);
    ss << line;
    size_t countryID = 0;
    while (getline(ss, line, ',')) {
        grid->getCountryByID(countryID).setReserveVaccines(std::stoi(line));
        getline(ss, line, ',');
        grid->getCountryByID(countryID).setTotalProductionCapacity(std::stoi(line));
        countryID++;
    }
}

void FileIO::load(size_t numberOfPastRecordsToLoad, size_t x, size_t y) {
    loadConfiguration();
    loadFieldsLastData(numberOfPastRecordsToLoad,x,y);
    loadDistrictsLastData();
    loadCountryLastData();
}
