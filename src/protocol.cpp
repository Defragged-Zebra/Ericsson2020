//
// Created by woranhun on 2020. 10. 17..
//

#include "protocol.h"
#include "logic.h"

void Protocol::start() {
    std::string line;
    while (std::getline(is,line)){
        if(line!="."){
            Protocol::request(line);
        }
    }
}
void Protocol::request(std::string& line){
    std::string tmp;
    std::stringstream ss;
    ss<<line;
    int gameID,tickID,countryID;
    ss>>tmp>>gameID>>tickID>>countryID;
    std::cerr<<std::endl;
    for (int i = 1; i <= 4; ++i) {
        std::cerr<<grid->random.getFactor(i)<<" ";
    }
    std::cerr<<std::endl;
    Logic::simulateTO(gameID,tickID,countryID); //TODO: randomszámok innen jönnek

    Protocol::currentResult(gameID,tickID,countryID);
}
void Protocol::currentResult(int gameID,int tickID,int countryID) {
    os << "RES " << gameID <<" " << tickID <<" "<< countryID <<std::endl;
    for (size_t y = 0; y < grid->getY(); ++y) {
        for (size_t x = 0; x < grid->getX() ; ++x) {
            os<< grid->getFieldByID((*grid)[y][x]).getCurrentInfectionValue() <<" ";
        }
        os<<std::endl;
    }
    os << "." <<std::endl;
}
void Protocol::initAntiVirus() {
    init();
    initValues();
}
void Protocol::initValues() {
    std::string line;
    std::getline(is, line);
    Protocol::setStart(line);
    std::getline(is, line);
    Protocol::createGrid(line);
}
void Protocol::setStart(std::string& line){
    std::stringstream ss;
    size_t gameid,maxtickid,countriescount;
    std::string tmp;
    ss<<line;
    ss>> tmp >> gameid >> maxtickid >> countriescount;
}
void Protocol::createGrid(std::string& line){
    std::stringstream ss;
    ss<<line;
    std::string tmp;
    ss>> tmp;
    unsigned long factors[4] ={0};
    for (unsigned long & factor : factors) {
        ss>>factor;
    }

    std::getline(is, line);
    //iy==rows, ix==columns
    size_t iy, ix;
    std::stringstream ss2;
    ss2<<line;
    ss2>>tmp>>iy >> ix;
    grid = new Grid(iy,ix,factors);
    Logic::setGrid(grid);
    size_t fieldID=0;
    size_t storedValsCnt = grid->getY()+grid->getX();
    size_t numberOfDisticts=0;
    for (size_t y = 0; y < grid->getY(); ++y) {
        for (size_t x = 0; x < grid->getX(); ++x) {
            std::stringstream ss3;
            std::getline(is,line);
            ss3<<line;
            size_t district;
            int infRate, population,tmp2;
            ss3>>tmp>> tmp2 >> tmp2 >> district >> infRate >> population;
            if (district>numberOfDisticts){numberOfDisticts=district;}
            //TODO field id check
            grid->addField(Field(fieldID,district,infRate,0,population,storedValsCnt));
            grid->uploadGridWithFieldID(y,x,fieldID++);
        }
    }
    std::vector<size_t> tmp2 =std::vector<size_t>();
    for (size_t i = 0; i < numberOfDisticts; ++i) {
        grid->addDistrict(District(i,tmp2,false));
    }

}
void Protocol::sendDebugMsg(const std::string &msg) {
    ers << msg << std::endl;
}
void Protocol::init() {
    os << "START " << teamToken << std::endl << "." << std::endl;
}
void Protocol::init(int seed) {
    os << "START " << teamToken << " " << seed << std::endl << "." << std::endl;
}
