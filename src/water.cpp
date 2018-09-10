#include "water.h"
#include "main.h"

Water::Water(float x, float y, float z,color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        //front
        -300.0f,-0.5f,-300.0f,
        -300.0f,-300.0f,-300.0f,
        300.0f, -300.0f, -300.0f,
         // triangle 1 : end
        300.0f, -300.0f,-300.0f,
        300.0f,-0.5f,-300.0f,
        -300.0f,-0.5f,-300.0f,
        //back
        -300.0f,-0.5f,300.0f,
        -300.0f,-300.0f,300.0f,
        300.0f, -300.0f,300.0f,

        300.0f, -300.0f,300.0f,
        300.0f,-0.5f,300.0f,
        -300.0f, -0.5f,300.0f,

        //left
        -300.0f, -0.5f,-300.0f,
        -300.0f,-300.0f,-300.0f,
        -300.0f,-300.0f, 300.0f,

        -300.0f, -0.5f, 300.0f,
        -300.0f,-300.0f, 300.0f,
        -300.0f, -0.5f,-300.0f,

        //right
        300.0f,-0.5f,-300.0f,
        300.0f,-300.0f,-300.0f,
        300.0f,-300.0f, 300.0f,

        300.0f,-300.0f, 300.0f,
        300.0f,-0.5f,-300.0f,
        300.0f,-0.5f,300.0f,

        //bottom
        -300.0f,-300.0f,-300.0f,
        -300.0f,-300.0f, 300.0f,
         300.0f,-300.0f,-300.0f,

         300.0f,-300.0f,-300.0f,
         300.0f,-300.0f, 300.0f,
        -300.0f,-300.0f,300.0f,

        //top
        -300.0f,-0.5f,-300.0f,
        -300.0f,-0.5f, 300.0f,
         300.0f,-0.5f,-300.0f,

         300.0f,-0.5f,-300.0f,
         300.0f,-0.5f, 300.0f,
        -300.0f,-0.5f,300.0f,
    };
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Water::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}