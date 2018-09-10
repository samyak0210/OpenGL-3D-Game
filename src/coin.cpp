#include "coin.h"
#include "main.h"

Coin::Coin(float x, float y, float z,color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    float PI=3.14159265;
    float r=2.0;
    double ix=r,iy=0.0;
    int j=0,k=1,i;
    GLfloat vertex_buffer_data[3258];
    for(int i=1;i<=360;i++){
        vertex_buffer_data[j++]=0.0f;
        vertex_buffer_data[j++]=0.0f;
        vertex_buffer_data[j++]=0.0f;

        vertex_buffer_data[j++]=ix;
        vertex_buffer_data[j++]=iy;
        vertex_buffer_data[j++]=0.0f;

        ix=r*cos((double)k*PI/180);
        iy=r*sin((double)k*PI/180);
        k++;

        vertex_buffer_data[j++]=ix;
        vertex_buffer_data[j++]=iy;
        vertex_buffer_data[j++]=0.0f;
    }
    this->object = create3DObject(GL_TRIANGLES, 1080, vertex_buffer_data, color, GL_FILL);
}

void Coin::draw(glm::mat4 VP) {
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

bounding_box_t Coin::bounding_box() {
    float x = this->position.x,y=this->position.y,z = this->position.z;
    bounding_box_t bbox = { x,y,z, 2.0,2.0,2.0 };
    return bbox;
}
