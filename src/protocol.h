//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_PROTOCOL_H
#define VIRUS_PROTOCOL_H


class Protocol {
public:
    Protocol(){
    }
    Protocol(const Protocol& p){
        *this= p;
    }
    Protocol& operator=(const Protocol& g){
        if(this != &g){
            //fields come here
        }
        return *this;
    }

};


#endif //VIRUS_PROTOCOL_H
