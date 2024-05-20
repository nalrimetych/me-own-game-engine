#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <vector>

#include "SHADER.h"

class Light {
public:
	Light(glm::vec3 pos,
		float constant, float linear, float quadratic,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		Shader& shader);

	void draw(Shader& shader, Shader& secShader, glm::mat4 projection, glm::mat4 view);
private:

	static unsigned int lsc; //light source's counter;
	unsigned int ind; 

	glm::vec3 m_pos;

	float m_constant;
	float m_linear;
	float m_quadratic;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	unsigned int VAO, VBO;

};



#endif