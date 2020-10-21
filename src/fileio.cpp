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
    saveFileCountryData << ": ";
    for (int i = 0; i < grid->getX() * grid->getY(); ++i) {
        saveFileCountryData << grid->getCountryByID(i).getReserveVaccines() << ", "
                            << grid->getCountryByID(i).getTotalProductionCapacity() << ", ";
    }
    saveFileCountryData << std::endl;
}

