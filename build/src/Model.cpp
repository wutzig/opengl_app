#include "header.h"

bool Model::operator==(const Model& other){
    return this->vertexArrayID == other.vertexArrayID;
}
Model::Model():
numQuads(0), vertices(nullptr), normals(nullptr), colors(nullptr){
    
}
Model::Model(const uint& numQuads, float *vertices, float *normals, float *colors):
    numQuads(numQuads), vertices(vertices), normals(normals), colors(colors){
    
    glCreateVertexArrays(1, &this->vertexArrayID);
    glBindVertexArray(this->vertexArrayID);

    uint buffer = 0;
    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * this->numQuads, this->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * this->numQuads, this->colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * this->numQuads, this->normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(NULL);
}

void Model::Bind(){
    glBindVertexArray(this->vertexArrayID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
}

void Model::Unbind(){
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glBindVertexArray(NULL);
}