//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_PROTOCOL_H
#define VIRUS_PROTOCOL_H
#include <iostream>
#include "iface.h"


class Protocol : public Iface() {
    static std::string teamToken;
public:
    void init(){
        std::cout<< "START" << teamToken <<std::endl<<"."<< std::endl;
    }
    void init(int seed){
        std::cout<< "START" << teamToken << " " << seed  <<std::endl<<"."<< std::endl;
    }
    void sendDebugMsg( const std::string& msg){
        std::cerr << msg << std::endl;
    }
    void initValues(){
        std::string line;
        while(std::getline(std::cin), line){

        }
    }

};


#endif //VIRUS_PROTOCOL_H
