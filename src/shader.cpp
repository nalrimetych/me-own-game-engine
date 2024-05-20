#include "SHADER.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
	std::string vertexCode;
	std::string fragmentCode;
    std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
    std::ifstream gShaderFile;

	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
    	vShaderFile.open(vertexPath);
    	fShaderFile.open(fragmentPath);
        gShaderFile.open(geometryPath);
    	std::stringstream vShaderStream, fShaderStream, gShaderStream;
    	vShaderStream << vShaderFile.rdbuf();
    	fShaderStream << fShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
    	vShaderFile.close();
    	fShaderFile.close();
        gShaderFile.close();
    	vertexCode   = vShaderStream.str();
    	fragmentCode = fShaderStream.str();
        geometryCode = gShaderStream.str();
    } catch(std::ifstream::failure &e) {
        std::cout << "BLYAT:: SHADERLAR:: FILE_NOT_READABLE_GOTIT?" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    unsigned int vertex, fragment, geometry;
    int success;
    char infolog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
    	glGetShaderInfoLog(vertex, 521, NULL, infolog);
    	std::cout<<"BLYAT::SHADER::VERTEX::COMPILATION_FAILANULSYA\n"<<infolog<<std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
    	glGetShaderInfoLog(fragment, 521, NULL, infolog);
    	std::cout<<"BLYAT::SHADER::FRAGMENT::COMPILATION_FAILANULSYA\n"<<infolog<<std::endl;
    }

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(geometry, 521, NULL, infolog);
        std::cout<<"BLYAT::SHADER::GEOMETRY::COMPILATION_FAILANULSYA\n"<<infolog<<std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glAttachShader(ID, geometry);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
    	glGetProgramInfoLog(ID, 512, NULL, infolog);
    	std::cout << "BLYAT::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);

}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure &e) {
        std::cout << "BLYAT:: SHADERLAR:: FILE_NOT_READABLE_GOTIT?" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infolog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 521, NULL, infolog);
        std::cout<<"BLYAT::SHADER::VERTEX::COMPILATION_FAILANULSYA\n"<<infolog<<std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 521, NULL, infolog);
        std::cout<<"BLYAT::SHADER::FRAGMENT::COMPILATION_FAILANULSYA\n"<<infolog<<std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infolog);
        std::cout << "BLYAT::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

Shader::~Shader() {
	glDeleteProgram(ID);
}


void Shader::use() {
	glUseProgram(ID);
}


void Shader::setBool(const std::string &name, bool val) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}
void Shader::setInt(const std::string &name, int val) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}
void Shader::setFloat(const std::string &name, float val) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}
void Shader::setMat4(const std::string &name, glm::mat4 val) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}
void Shader::setVec3(const std::string &name, glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}