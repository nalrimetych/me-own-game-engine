#include "LIGHT.h"

unsigned int Light::lsc = 0;

float vertices[] = {
    // positions
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

Light::Light(glm::vec3 pos,
		float constant, float linear, float quadratic,
		glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		Shader& shader)
{
	ind = lsc++;
	shader.setInt("nr_light", lsc);

	m_pos = pos;

	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;

	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;


	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//POSITION ATTRIBS
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);
}

void Light::draw(Shader& shader, Shader& secShader, glm::mat4 projection, glm::mat4 view) {
	m_pos.x = cos(glfwGetTime()) * 5.0f;
	m_pos.y = sin(glfwGetTime()) * 5.0f;

	glm::mat4 model = glm::mat4(1.0f);
	secShader.use();
	secShader.setMat4("projection", projection);
	secShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, m_pos);
	model = glm::scale(model, glm::vec3(0.2f));
	secShader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	shader.use();
	shader.setVec3(("pointLights[" + std::to_string(ind) + "].position"), m_pos);

	shader.setFloat(("pointLights[" + std::to_string(ind) + "].constant"), m_constant);
	shader.setFloat(("pointLights[" + std::to_string(ind) + "].linear"), m_linear);
	shader.setFloat(("pointLights[" + std::to_string(ind) + "].quadratic"), m_quadratic);

	shader.setVec3(("pointLights[" + std::to_string(ind) + "].ambient"), m_ambient);
	shader.setVec3(("pointLights[" + std::to_string(ind) + "].diffuse"), m_diffuse);
	shader.setVec3(("pointLights[" + std::to_string(ind) + "].specular"), m_specular);

}