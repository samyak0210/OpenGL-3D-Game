#include "main.h"

#ifndef CUBE_H
#define CUBE_H


class Cube {
public:
    Cube() {}
    Cube(float x, float y,float z,float a,color_t color);
    glm::vec3 position;
    float rotation,side,angle;
    void draw(glm::mat4 VP);
    int score;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // CUBE_H
