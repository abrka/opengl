#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800*1.5;
const unsigned int SCR_HEIGHT = 600*1.5;
float mixAmount = 0.0;
bool RotationEnabled = true;




glm::vec3 CameraPosition{ 1.0f };
glm::vec3 CameraDirection{ 0.0f , 0.0f , -1.0f };
glm::vec3 CameraUpVector{ 0.0, 1.0,0.0 };

glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed{ 0.1f };

glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition + CameraDirection, CameraUpVector);
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);

glm::mat4 DirLightModelMat{ 1.0 };
glm::vec3 LightColor{ 1.0f };


glm::vec3 PointLightColor{ 1.0f, 0.1f, 0.1f};
glm::vec3 PointLightPosition{ 10.0f };


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f + 2.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 cubeRotationDirs[] = {
	glm::vec3(0.0f,  0.0f,  0.0f + 2.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	

	//opengl init
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	unsigned int indices[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35
	};



	unsigned int VAO;
	glGenVertexArrays(1, &VAO);


	unsigned int EBO;
	glGenBuffers(1, &EBO);


	unsigned int VBO;
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);




	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//tex coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GlShaderProgram LitObjectShader{ "shaders/LitObject.glsl", "shaders/Vertex.glsl" };
	GlShaderProgram LightShader{ "shaders/Light.glsl", "shaders/Vertex.glsl" };

	GlTexture Diffuse{ "containerdiff.png",0, GL_RGBA };
	GlTexture Specular{ "container_specular.png",1, GL_RGBA };
	GlTexture Emission{ "matrix.jpg",2, GL_RGB };

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);



		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_BLEND);// you enable blending function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	
		LitObjectShader.Bind();


		// update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(LitObjectShader.ID, "ourColor");
		glUniform4f(vertexColorLocation, greenValue, greenValue, 0.0f, 1.0f);

		glUniform1f(glGetUniformLocation(LitObjectShader.ID, "mixAmount"), mixAmount);

		view = glm::lookAt(CameraPosition, CameraPosition + CameraDirection, CameraUpVector);
		glUniformMatrix4fv(glGetUniformLocation(LitObjectShader.ID, "uView"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(LitObjectShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

		
		glm::vec3 DirLightDirection = glm::vec3{ DirLightModelMat[2].x,DirLightModelMat[2].y,DirLightModelMat[2].z };

		LitObjectShader.SetVec3("Light.direction", DirLightDirection);
		LitObjectShader.SetVec3("Light.color", LightColor);
		LitObjectShader.SetVec3("PointLight.color", PointLightColor);
		LitObjectShader.SetVec3("PointLight.position", PointLightPosition);
		LitObjectShader.SetFloat("PointLight.constant", 0.80f);
		LitObjectShader.SetFloat("PointLight.linear", 0.01f);
		LitObjectShader.SetFloat("PointLight.quadratic", 0.032f);
		LitObjectShader.SetVec3("uCameraPos", CameraPosition);
		LitObjectShader.SetFloat("Mat.shine",16.0);

		Diffuse.Activate();
		LitObjectShader.SetInt("Mat.color", Diffuse.TextureUnit);

		Specular.Activate();
		LitObjectShader.SetInt("Mat.specular", Specular.TextureUnit);

		Emission.Activate();
		LitObjectShader.SetInt("Mat.emission", Emission.TextureUnit);

		glBindVertexArray(VAO);

		for (size_t i = 0; i < 10; i++)
		{
			LitObjectShader.Bind();
			glm::mat4 model{ 1.0 };
			model = glm::translate(model, cubePositions[i]);


			model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.0f), glm::normalize(cubeRotationDirs[i]));
			glUniformMatrix4fv(glGetUniformLocation(LitObjectShader.ID, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}


		LightShader.Bind();
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ID, "uView"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ID, "uModel"), 1, GL_FALSE, glm::value_ptr(DirLightModelMat));
		glUniform3fv(glGetUniformLocation(LightShader.ID, "lightColor"), 1, glm::value_ptr(LightColor));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		LightShader.Bind();
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ID, "uView"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ID, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 PointLightModel{ 1.0f };
		PointLightModel = glm::translate(PointLightModel, PointLightPosition);
		glUniformMatrix4fv(glGetUniformLocation(LightShader.ID, "uModel"), 1, GL_FALSE, glm::value_ptr(PointLightModel));
		glUniform3fv(glGetUniformLocation(LightShader.ID, "lightColor"), 1, glm::value_ptr(PointLightColor));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, 0.1f, glm::vec3(0.0f, 1.0f,0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, -0.1f, glm::vec3(1.0f,0.0f, 0.0f));
	}
	

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		CameraPosition += cameraSpeed * CameraDirection;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		CameraPosition -= cameraSpeed * CameraDirection;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		CameraPosition -= glm::normalize(glm::cross(CameraDirection, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		CameraPosition += glm::normalize(glm::cross(CameraDirection, cameraUp)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		CameraPosition.y += cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		CameraPosition.y -= cameraSpeed;
	}
}

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	static bool firstMouse = true;
	static float lastX{};
	static float lastY{};
	static float yaw{};
	static float pitch{};

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	CameraDirection = glm::normalize(front);

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}