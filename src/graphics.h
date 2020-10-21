//
// Created by woranhun on 2020. 10. 17..
//

#ifndef VIRUS_GRAPHICS_H
#define VIRUS_GRAPHICS_H


class Graphics {
    int id;
public:
    Graphics(){
        id=0;
    }
    Graphics(const Graphics& g){
        *this= g;
    }
    Graphics& operator=(const Graphics& g){
        if(this != &g){
            id=g.id;
        }
        return *this;
    }
};


#endif //VIRUS_GRAPHICS_H
