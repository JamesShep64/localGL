#include "GraphicsEngine.h"
#include "Material.h"
#include "ShaderStructs.h"
#include <iostream>
GraphicsEngine::~GraphicsEngine(){
    glDeleteProgram(landShader);
    glDeleteProgram(skyShader);
}
void GraphicsEngine::init(){
    landShader = this->initShader((char *)"LandFragmentShader.glsl",(char*)"NoTextureVertex.glsl");
    skyShader = this->initShader((char *)"SkyFragmentShader.glsl",(char *)"VertexShader.glsl");

    this->initLand(landShader);
    this->debugShaderProgram(landShader);
    this->initSky(skyShader);
    this->debugShaderProgram(skyShader);
}
void GraphicsEngine::initSky(GLuint& shaderProgram)
{
   glUseProgram(shaderProgram);
    GLuint vao, vbo, veo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenBuffers(1, &veo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
   GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texCoordinate");
    glVertexAttribPointer(posAttrib,3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (void*)0);
    glVertexAttribPointer(texAttrib,2, GL_FLOAT,GL_FALSE,sizeof(Vertex), (void*)(sizeof(glm::vec3))); 

    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(texAttrib);
    GLint modelUni = glGetUniformLocation(shaderProgram, "model");

    GLint perspectiveUni = glGetUniformLocation(shaderProgram, "perspective");

    GLint viewUni = glGetUniformLocation(shaderProgram, "view");
    Shader sky;

    sky.shaderProgram = shaderProgram;
    sky.vao = vao;
    sky.vbo = vbo;
    sky.modelUni = modelUni;
    sky.perspectiveUni = perspectiveUni;
    sky.viewUni = viewUni;
    sky.posAttrib = posAttrib;
    shaders[shaderProgram] = sky;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    GLenum err;
while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL error: " << std::hex << err << std::endl;
}

    }
void GraphicsEngine::setArrayToSky(GLuint& shaderProgram){
    this->setShader(shaderProgram);
    glVertexAttribPointer(shaders[shaderProgram].posAttrib,3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (void*)0);
    glVertexAttribPointer(shaders[shaderProgram].texAttrib,2, GL_FLOAT,GL_FALSE,sizeof(Vertex), (void*)(sizeof(glm::vec3)));

}
GLuint GraphicsEngine::initShader(char* fragName, char* vertexName){
    std::string vertexSourceStr = LoadShaderSource(vertexName);
    std::string fragmentSourceStr = LoadShaderSource(fragName);
    const char* vertexSource = vertexSourceStr.c_str();
    const char* fragmentSource = fragmentSourceStr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
void GraphicsEngine::initLand(GLuint& shaderProgram){
    glUseProgram(shaderProgram);
    GLuint vao, vbo, veo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenBuffers(1, &veo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib,3, GL_FLOAT,GL_FALSE,sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(posAttrib);
    GLint modelUni = glGetUniformLocation(shaderProgram, "model");

    GLint perspectiveUni = glGetUniformLocation(shaderProgram, "perspective");

    GLint viewUni = glGetUniformLocation(shaderProgram, "view");
    Shader sky;

    sky.shaderProgram = shaderProgram;
    sky.vao = vao;
    sky.vbo = vbo;
    sky.modelUni = modelUni;
    sky.perspectiveUni = perspectiveUni;
    sky.viewUni = viewUni;
    sky.posAttrib = posAttrib;
    shaders[shaderProgram] = sky;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    GLenum err;
while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL error: " << std::hex << err << std::endl;
}
}
void GraphicsEngine::setArrayToLand(GLuint& shaderProgram){
    this->setShader(shaderProgram);
    glEnableVertexAttribArray(shaders[shaderProgram].posAttrib);
    glVertexAttribPointer(shaders[shaderProgram].posAttrib,3, GL_FLOAT,GL_FALSE,sizeof(glm::vec3), (void*)0);
}
std::string GraphicsEngine::LoadShaderSource(const std::string& filePath)
{
    std::ifstream shaderFile(filePath);
    if (!shaderFile) {
        std::cerr << "Failed to open shader file: " << filePath << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    return buffer.str();
}
void GraphicsEngine::setModel(glm::mat4& m, GLuint& shaderProgram){
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(shaders[shaderProgram].modelUni, 1, GL_FALSE, glm::value_ptr(m));
}
void GraphicsEngine::setPerspective(glm::mat4& m, GLuint& shaderProgram){ 
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(shaders[shaderProgram].perspectiveUni, 1, GL_FALSE, glm::value_ptr(m));
}
void GraphicsEngine::setView(glm::mat4& m, GLuint& shaderProgram){
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(shaders[shaderProgram].viewUni, 1, GL_FALSE, glm::value_ptr(m));
}
void GraphicsEngine::setCamera(GLuint& shaderProgram, Camera& cam){
    this->setShader(shaderProgram);
    this->setPerspective(cam.perspective, shaderProgram);
    this->setView(cam.view, shaderProgram);
}
void GraphicsEngine::setShader(GLuint& shaderProgram){
    glUseProgram(shaderProgram);
    }
void GraphicsEngine::debugShaderProgram(GLuint program) {
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    std::cout << "Program " << program << " link status: " << linkStatus << std::endl;
    
    if (!linkStatus) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Link error: " << infoLog << std::endl;
        return;
    }
    
    // Get active uniforms
    GLint numUniforms;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
    std::cout << "Active uniforms: " << numUniforms << std::endl;
    
    for (int i = 0; i < numUniforms; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(program, i, sizeof(name), &length, &size, &type, name);
        GLint location = glGetUniformLocation(program, name);
        std::cout << "Uniform " << i << ": " << name << " (location: " << location << ", type: " << type << ")" << std::endl;
    }
}
