#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SHADER.h"
#include "CAMERA.h"
#include "stb_image.h"
#include "MODEL.h"
#include "LIGHT.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);


const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 400, lastY = 300;
float yaw, pitch;
bool firstMouseMove = true;






int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  

    //window init
    GLFWwindow* window = glfwCreateWindow(800, 600, "me own game engine", NULL, NULL);
	if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window);


	//mouse stuff
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	//cursor stuff
	glfwSetScrollCallback(window, scroll_callback);
	
	yaw = -90.0f;



	//glad pointer init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);



	glViewport(0, 0, 800, 600); //window size setting
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //setting resising func

	Shader myShader("B:/game__dev/gl/first/prop/vertex.txt",
					"B:/game__dev/gl/first/prop/fragment.txt",
					"B:/game__dev/gl/first/prop/geometry.txt");
	Shader secShader("B:/game__dev/gl/first/prop/cubeSunVertex.txt",
					 "B:/game__dev/gl/first/prop/cubeSunFragment.txt");
	myShader.use();

	//myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//myShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    //myShader.setVec3("dirLight.diffuse", 0.7f, 0.7f, 0.7f);
    //myShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

    //myShader.setInt("nr_light", 0);
	
	Model bck("B:/game__dev/gl/first/prop/cliff/cliff_3_low.obj");



	//point lights
	Light firstLight(glm::vec3(-5.0f, 0.0f, 3.0f),
					 1.0f, 0.09f, 0.032f,
					 glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f),
					 myShader);







	while(!glfwWindowShouldClose(window)) // render loop
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);// rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //cleanin sum color and z-depth buffers

	    //movement delta time stuff
	    float currentFrame = glfwGetTime();
	    deltaTime = currentFrame - lastFrame;
	    lastFrame = currentFrame;


	    myShader.use();

		

	    myShader.setVec3("cameraPos", camera.position);

	    glm::mat4 projection = glm::perspective(glm::radians(camera.mouseZoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	    glm::mat4 view = camera.getViewMatrix();
	    myShader.setMat4("projection", projection);
	    myShader.setMat4("view", view);

	    


	    //model renderin here
	    glm::mat4 model = glm::mat4(1.0f);
	    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	    model = glm::scale(model, glm::vec3(0.1f));
	    myShader.setMat4("model", model);
	    bck.draw(myShader);


	    firstLight.draw(myShader, secShader, projection, view);
	    

	    

	    glfwSwapBuffers(window);
	    glfwPollEvents();    
	}


	glfwTerminate(); // terminate all allocated memory
    return 0;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    cout << width << '\t' << height << endl;
} // window resising func	

void mouse_callback(GLFWwindow* window, double xposIN, double yposIN)
{
	float xpos = static_cast<float>(xposIN);
	float ypos = static_cast<float>(yposIN);

	if(firstMouseMove) {
		lastX = xpos;
		lastY = ypos;
		firstMouseMove = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	
	camera.processMouseMovement(xoffset, yoffset);
}//mouse movin detection func

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll(static_cast<float>(yoffset));
}//scrollin detection func

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    //camera stuff
   	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
   		camera.processKeyboard(FORWARD, deltaTime);
   	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
   		camera.processKeyboard(BACKWARD, deltaTime);
   	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
   		camera.processKeyboard(LEFT, deltaTime);
   	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
   		camera.processKeyboard(RIGHT, deltaTime);

}// know ESC key is whether pressed or not


