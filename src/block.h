#include "main.h"

#ifndef BLOCK_H
#define BLOCK_H


class Block {
public:
    Block() {}
    Block(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
