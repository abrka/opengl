#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include <iostream>
#include "Mesh.h"
#include "ModelLoader.h"
#include "Framebuffer.h"
#include "Renderbuffer.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
float mixAmount = 0.0;
bool RotationEnabled = true;


//glm::vec3 CameraPosition{ 1.0f };
//glm::vec3 CameraDirection{ 0.0f , 0.0f , -1.0f };
//glm::vec3 CameraUpVector{ 0.0, 1.0,0.0 };
//
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//
//
//glm::mat4 view = glm::lookAt(CameraPosition, CameraPosition + CameraDirection, CameraUpVector);
//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
Camera Cam{ SCR_WIDTH, SCR_HEIGHT };


float cameraSpeed{ 0.05f };

glm::mat4 DirLightModelMat{ 1.0 };
glm::vec3 LightColor{ 1.0f };


glm::vec3 PointLightColor{ 1.0f, 0.1f, 0.1f };
glm::vec3 PointLightPosition{ 100.0f };


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

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	else {
		std::cout << "couldnt enable opengl debug" << std::endl;
	}

	//opengl init
	std::vector<Vertex> vertices{
		// positions          // normals           // texture coords
		 Vertex{glm::vec3{ -0.5f, -0.5f, -0.5f}, glm::vec3{ 0.0f,  0.0f, -1.0f}, glm::vec2{  0.0f,  0.0f}},
		 Vertex{glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{  0.0f,  0.0f, -1.0f}, glm::vec2{ 1.0f,  0.0f}},
		 Vertex{glm::vec3{0.5f,  0.5f, -0.5f}, glm::vec3{  0.0f,  0.0f, -1.0f}, glm::vec2{  1.0f,  1.0f}},
		 Vertex{glm::vec3{0.5f,  0.5f, -0.5f}, glm::vec3{  0.0f,  0.0f, -1.0f}, glm::vec2{  1.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{ 0.0f,  0.0f, -1.0f}, glm::vec2{  0.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{  0.0f,  0.0f, -1.0f}, glm::vec2{ 0.0f,  0.0f}},

		Vertex{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{ 0.0f,  0.0f,  1.0f}, glm::vec2{ 0.0f,  0.0f}},
		 Vertex{glm::vec3{0.5f, -0.5f,  0.5f}, glm::vec3{  0.0f,  0.0f,  1.0f}, glm::vec2{  1.0f,  0.0f}},
		 Vertex{glm::vec3{0.5f,  0.5f,  0.5f}, glm::vec3{  0.0f,  0.0f,  1.0f}, glm::vec2{  1.0f,  1.0f}},
		 Vertex{glm::vec3{0.5f,  0.5f,  0.5f}, glm::vec3{  0.0f,  0.0f,  1.0f}, glm::vec2{  1.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{  0.0f,  0.0f,  1.0f}, glm::vec2{ 0.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{  0.0f,  0.0f,  1.0f}, glm::vec2{ 0.0f,  0.0f}},

		Vertex{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{ -1.0f,  0.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},
		Vertex{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{ -1.0f,  0.0f,  0.0f}, glm::vec2{  1.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{ -1.0f,  0.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{ -1.0f,  0.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{ -1.0f,  0.0f,  0.0f}, glm::vec2{ 0.0f,  0.0f}},
		Vertex{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{ -1.0f,  0.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},

		Vertex{glm::vec3{ 0.5f,  0.5f,  0.5f}, glm::vec3{  1.0f,  0.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},
		Vertex{glm::vec3{ 0.5f,  0.5f, -0.5f}, glm::vec3{  1.0f,  0.0f,  0.0f}, glm::vec2{  1.0f,  1.0f}},
		Vertex{glm::vec3{ 0.5f, -0.5f, -0.5f}, glm::vec3{  1.0f,  0.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}},
		 Vertex{glm::vec3{0.5f, -0.5f, -0.5f}, glm::vec3{  1.0f,  0.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}},
		Vertex{glm::vec3{ 0.5f, -0.5f,  0.5f}, glm::vec3{  1.0f,  0.0f,  0.0f}, glm::vec2{  0.0f,  0.0f}},
		 Vertex{glm::vec3{0.5f,  0.5f,  0.5f}, glm::vec3{  1.0f,  0.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},

		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{  0.0f, -1.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}},
		Vertex{glm::vec3{ 0.5f, -0.5f, -0.5f}, glm::vec3{  0.0f, -1.0f,  0.0f}, glm::vec2{  1.0f,  1.0f}},
		Vertex{glm::vec3{ 0.5f, -0.5f,  0.5f}, glm::vec3{ 0.0f, -1.0f,  0.0f}, glm::vec2{ 1.0f,  0.0f}},
		Vertex{glm::vec3{ 0.5f, -0.5f,  0.5f}, glm::vec3{  0.0f, -1.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f,  0.5f}, glm::vec3{  0.0f, -1.0f,  0.0f}, glm::vec2{  0.0f,  0.0f}},
		Vertex{glm::vec3{-0.5f, -0.5f, -0.5f}, glm::vec3{  0.0f, -1.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}},

		Vertex{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{  0.0f,  1.0f,  0.0f}, glm::vec2{ 0.0f,  1.0f}},
		Vertex{glm::vec3{ 0.5f,  0.5f, -0.5f}, glm::vec3{  0.0f,  1.0f,  0.0f}, glm::vec2{  1.0f,  1.0f}},
		 Vertex{glm::vec3{0.5f,  0.5f,  0.5f}, glm::vec3{ 0.0f,  1.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},
		Vertex{glm::vec3{ 0.5f,  0.5f,  0.5f}, glm::vec3{  0.0f,  1.0f,  0.0f}, glm::vec2{  1.0f,  0.0f}},
		Vertex{glm::vec3{-0.5f,  0.5f,  0.5f}, glm::vec3{  0.0f,  1.0f,  0.0f}, glm::vec2{  0.0f,  0.0f}},
		Vertex{glm::vec3{-0.5f,  0.5f, -0.5f}, glm::vec3{  0.0f,  1.0f,  0.0f}, glm::vec2{  0.0f,  1.0f}}
	};

	std::vector<unsigned int> indices = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35
	};

	std::vector<Vertex> QuadVertices{
		// positions   // texCoords
	   Vertex{ glm::vec3{-1.0f,  1.0f , 0.0f}, glm::vec3{}, glm::vec2{0.0f, 1.0f }},
	   Vertex{  glm::vec3{-1.0f, -1.0f, 0.0f},glm::vec3{},  glm::vec2{0.0f, 0.0f }},
	   Vertex{  glm::vec3{1.0f, -1.0f, 0.0f}, glm::vec3{}, glm::vec2{1.0f, 0.0f }},

	   Vertex{  glm::vec3{-1.0f,  1.0f, 0.0f},glm::vec3{}, glm::vec2{ 0.0f, 1.0f }},
	   Vertex{  glm::vec3{1.0f, -1.0f, 0.0f},glm::vec3{},  glm::vec2{1.0f, 0.0f }},
	   Vertex{  glm::vec3{1.0f,  1.0f, 0.0f},glm::vec3{},  glm::vec2{1.0f, 1.0f }}

	};

	std::vector<unsigned int> QuadIndices{
		0,1,2,3,4,5
	};

	//GlShaderProgram LitObjectShader{ "shaders/LitObject.glsl", "shaders/Vertex.glsl" };
	GlShaderProgram LightShader{ "shaders/Light.glsl", "shaders/Vertex.glsl" };
	GlShaderProgram SkyboxShader{ "shaders/SkyboxFrag.glsl", "shaders/SkyboxVert.glsl" };
	GlShaderProgram ScreenShader{ "shaders/ScreenFrag.glsl","shaders/ScreenVertex.glsl" };

	//GlTexture Gun{ "meshes/diffuse.png"};
	//GlTexture Specular{ "container_specular.png" };
	//GlTexture Emission{ "matrix.jpg" };
	std::shared_ptr<GlTexture> MP7Diffuse = AssetLoader::LoadTextureFromPath("meshes/mp7/cube_base_color.png");
	std::shared_ptr<GlTexture> MP7Specular = AssetLoader::LoadTextureFromPath("meshes/mp7/Image.png");
	std::shared_ptr<GlTexture> MP7Emission = AssetLoader::LoadTextureFromPath("meshes/mp7/cube_emission.png");
	std::shared_ptr<GlCubemapTexture> SkyboxTex = AssetLoader::LoadCubemapTextureFromPath(std::array<std::filesystem::path, 6>{ "textures/skybox/right.jpg", "textures/skybox/left.jpg", "textures/skybox/top.jpg", "textures/skybox/bottom.jpg", "textures/skybox/front.jpg", "textures/skybox/back.jpg" });

	GlMesh QuadMesh{ QuadVertices,QuadIndices };
	//GlModel ScreenQuadModel{ QuadMesh, ScreenShader };

	GlMesh CubeMesh{ vertices, indices };
	//GlModel SkyboxCubeModel{ CubeMesh, SkyboxShader };

	//GlMesh LightMesh{ vertices, indices };



	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("meshes/mp7/mp7.gltf", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	auto AssimpLoadedMesh = AssetLoader::LoadMeshFromAssimp(*scene->mMeshes[0]);
	//auto LitObjectShaderPtr = AssetLoader::LoadMaterialFromAssimp(*scene->mMaterials[0], "meshes/mp7", "shaders/Vertex.glsl", "shaders/LitObject.glsl");

	GlShaderProgram LitObjectShader{ "shaders/LitObject.glsl", "shaders/Vertex.glsl" };


	GlFramebuffer ScreenFBO{};
	GlTexture ScreenFBOTex{ GL_RGB, GL_RGB,SCR_WIDTH, SCR_HEIGHT, NULL, TextureSpec{false, GL_CLAMP_TO_EDGE} };
	GlRenderBuffer ScreenRBO{ GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT };
	ScreenFBO.AttachTexture(ScreenFBOTex, GL_COLOR_ATTACHMENT0);
	ScreenFBO.AttachRenderBuffer(ScreenRBO);
	ScreenFBO.CheckStatus();

	const unsigned int SHADOW_MAP_WIDTH = 1024;
	const unsigned int SHADOW_MAP_HEIGHT = 1024;
	GlTexture ShadowMapDepthTex{ GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT , SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, NULL, TextureSpec{false, GL_REPEAT, GL_FLOAT} };
	GlFramebuffer ShadowMapFBO{};
	ShadowMapFBO.AttachTexture(ShadowMapDepthTex, GL_DEPTH_ATTACHMENT);
	ShadowMapFBO.Bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);



		// render
		// ------

		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		ScreenFBO.Bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_BLEND);// you enable blending function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



		Cam.RecalculateViewMatrix();

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);

		SkyboxShader.Bind();
		SkyboxShader.SetMatrix4f("uView", glm::mat4{ glm::mat3{Cam.view} });
		SkyboxShader.SetMatrix4f("uProjection", Cam.projection);
		SkyboxTex->Bind();
		CubeMesh.Draw(SkyboxShader);

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);

		glm::vec3 DirLightDirection = glm::vec3{ DirLightModelMat[2].x,DirLightModelMat[2].y,DirLightModelMat[2].z };

		LitObjectShader.Bind();
		LitObjectShader.SetMatrix4f("uView", Cam.view);
		LitObjectShader.SetMatrix4f("uProjection", Cam.projection);
		LitObjectShader.SetVec3("Light.direction", DirLightDirection);
		LitObjectShader.SetVec3("Light.color", LightColor);
		LitObjectShader.SetVec3("PointLight.color", PointLightColor);
		LitObjectShader.SetVec3("PointLight.position", PointLightPosition);
		LitObjectShader.SetFloat("PointLight.constant", 0.80f);
		LitObjectShader.SetFloat("PointLight.linear", 0.01f);
		LitObjectShader.SetFloat("PointLight.quadratic", 0.032f);
		LitObjectShader.SetVec3("uCameraPos", Cam.Position);
		LitObjectShader.SetFloat("Mat.shine", 120.0);
		LitObjectShader.SetFloat("Mat.emissionStrength", 15.0);
		LitObjectShader.SetTexture("Mat.color", *MP7Diffuse, 0);
		LitObjectShader.SetTexture("Mat.specular", *MP7Specular, 1);
		LitObjectShader.SetTexture("Mat.emission", *MP7Emission, 2);
		LitObjectShader.SetCubemapTexture("skybox", *SkyboxTex, 4);
		glm::mat4 gunModelMatrix{ 1.0 };
		LitObjectShader.SetMatrix4f("uModel", gunModelMatrix);
		AssimpLoadedMesh->Draw(LitObjectShader);
		ScreenFBO.Unbind();
		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ScreenShader.Bind();
		ScreenShader.SetTexture("screenTexture", ScreenFBOTex, 0);
		QuadMesh.Draw(ScreenShader);
		ScreenShader.Unbind();

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

void DrawScene() {

}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, -0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		DirLightModelMat = glm::rotate(DirLightModelMat, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
	}


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Cam.Position += cameraSpeed * Cam.Direction;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Cam.Position -= cameraSpeed * Cam.Direction;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Cam.Position -= glm::normalize(glm::cross(Cam.Direction, Cam.UpVector)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Cam.Position += glm::normalize(glm::cross(Cam.Direction, Cam.UpVector)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		Cam.Position.y += cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		Cam.Position.y -= cameraSpeed;
	}
}

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	static bool firstMouse = true;
	static float lastX{};
	static float lastY{};
	static float yaw{ -90.0f };
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

	glm::vec3 front{};
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Cam.Direction = glm::normalize(front);

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}