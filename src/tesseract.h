#ifndef TESSERACT_H
#define TESSERACT_H
#include "object4d.h"



class Tesseract : public Object4D{
    public:
        Tesseract();
        Tesseract(glm::vec4 position,  Hyperplane *hyperplane);

};

#endif