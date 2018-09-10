#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y,float z,color_t color);
    glm::vec3 position;
    float rotation,flagx,flagz;
    void draw(glm::mat4 VP);
    double speed_x,speed_y,speed_z;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BOAT_H
