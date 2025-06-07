#include "Material.h"
#include "../glm/glm/ext/vector_float3.hpp"
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#include <cstdio>
#include <string>

Material::Material(){

}
bool Material::loadObjToTriangleArray(char* path)
{
    std::vector<GLuint>uvIndices;
    FILE * file = fopen(path, "r");
    if(file == NULL) {
        std::cerr<<"couldnt find .obj file"<<std::endl;
    }

    while(1) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                &vertexIndex[0], &uvIndex[0], &normalIndex[0], 
                &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                std::cerr<<"File can't be read by our simple parser :-( Try exporting with other options\n";
                fclose(file);
            }
            indices.push_back(vertexIndex[0]-1);
            indices.push_back(vertexIndex[1]-1);
            indices.push_back(vertexIndex[2]-1);
            
            uvIndices.push_back(uvIndex[0]-1);
            uvIndices.push_back(uvIndex[1]-1);
            uvIndices.push_back(uvIndex[2]-1);
            /*
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            */
        } else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    fclose(file);
    for(int i = 0; i<indices.size();i++){
        vertex_array.push_back(Vertex{
                        .position= vertices[indices[i]],
                        .texCoordinate = uvs[uvIndices[i]]
                        });
    }
    return true;
}

bool Material::loadTexture(char* path, GLuint shaderProgram, char* textureName){
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters (wrapping & filtering)
    // Load image with stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // OpenGL expects 0,0 at bottom left
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED_NV;
        else if (nrChannels == 3)
            format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
        else {
            std::cerr<<"color channel error"<<std::endl;
            return false;
        }


    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram,textureName), 0); // Texture unit 0
    }else {

        std::cerr<<"data not loaded\n";
        return false;}

    stbi_image_free(data);

    return true;
}
  void Material::loadElement(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices){
    this->indices = indices;
    this->vertices = vertices;
}

void Material::drawElementArray(std::vector<unsigned int>& elements, Shader s){
    glUseProgram(s.shaderProgram);
    glBindVertexArray(s.vao); 
    glBindBuffer(GL_ARRAY_BUFFER,s.vbo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,s.veo); 
    glDrawElements(GL_TRIANGLES,elements.size(),GL_UNSIGNED_INT,0);
}
