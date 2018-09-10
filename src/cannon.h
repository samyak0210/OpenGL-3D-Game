#include "main.h"

#ifndef CANNON_H
#define CANNON_H


class Cannon {
public:
    Cannon() {}
    Cannon(float x, float y,float z,color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // CANNON_H
