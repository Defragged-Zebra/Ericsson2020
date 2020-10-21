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
 *    section2:
 *          districtID: assignedFieldID1, assignedFieldID2, ....
 *    section3:
 *          countryID: assignedDistrictID1, assignedDistrictID1, ...
 *
 * file Field:
 *      TBD
 * file Districts:
 *      TBD
 * file Countries:
 *      TBD
 *
 * */
void FileIO::saveConfiguration() {
    saveFile<<*grid<<std::endl;
}
void FileIO::saveAll() const {

}
