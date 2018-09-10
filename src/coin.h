#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y,float z,color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // COIN_H
