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
#include "Model.h"
#include "Material.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
static bool CursorEnabled = false;

GlRendererContext RenderContext{ Camera{SCR_WIDTH, SCR_HEIGHT }, DirLight{}, PointLight{} };
float cameraSpeed{ 0.05f };

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
	glfwSetKeyCallback(window, key_callback);

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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

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
	std::shared_ptr<GlTexture> BakedLightmap = AssetLoader::LoadTextureFromPath("textures/baked.png");
	std::shared_ptr<GlTexture> BakedDiffuse = AssetLoader::LoadTextureFromPath("textures/diffuse.png");
	std::shared_ptr<GlCubemapTexture> SkyboxTex = AssetLoader::LoadCubemapTextureFromPath(std::array<std::filesystem::path, 6>{ "textures/skybox/right.jpg", "textures/skybox/left.jpg", "textures/skybox/top.jpg", "textures/skybox/bottom.jpg", "textures/skybox/front.jpg", "textures/skybox/back.jpg" });

	RenderContext.Skybox = AssetLoader::LoadCubemapTextureFromPath(std::array<std::filesystem::path, 6>{ "textures/skybox/right.jpg", "textures/skybox/left.jpg", "textures/skybox/top.jpg", "textures/skybox/bottom.jpg", "textures/skybox/front.jpg", "textures/skybox/back.jpg" });

	GlMesh QuadMesh{ QuadVertices,QuadIndices };
	//GlModel ScreenQuadModel{ QuadMesh, ScreenShader };

	GlMesh CubeMesh{ vertices, indices };
	//GlModel SkyboxCubeModel{ CubeMesh, SkyboxShader };

	//GlMesh LightMesh{ vertices, indices };



	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("meshes/sphere/untitled.gltf", aiProcess_Triangulate  | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	auto AssimpLoadedMesh = AssetLoader::LoadMeshFromAssimp(*scene->mMeshes[0]);

	std::shared_ptr<GlShaderProgram> LitObjectShaderPtr = std::make_shared<GlShaderProgram>("shaders/LitObject.glsl", "shaders/Vertex.glsl");

	std::shared_ptr<GlMaterial> TestMat = std::make_shared<GlMaterial>();
	TestMat->Shader = LitObjectShaderPtr;
	TestMat->DiffuseTex = BakedDiffuse;
	/*TestMat->SpecularTex = MP7Emission;
	TestMat->EmissionTex = MP7Emission;*/
	TestMat->LightmapTex = BakedLightmap;
	//TestMat->EmissionStrength = 13.0;

	GlModel TestModel{ AssimpLoadedMesh, TestMat };
	//TestModel.EulerRotation.x = glm::radians(90.0f);

//	GlModel TestModel2{ AssimpLoadedMesh, TestMat };

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

		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(); // Show demo window! :)

		// render
		// ------

		ImGui::Begin("My window");

		ImGui::End();

		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		ScreenFBO.Bind();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		glEnable(GL_BLEND);// you enable blending function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



		RenderContext.Cam.RecalculateViewMatrix();

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);

		SkyboxShader.Bind();
		SkyboxShader.SetMatrix4f("uView", glm::mat4{ glm::mat3{RenderContext.Cam.view} });
		SkyboxShader.SetMatrix4f("uProjection", RenderContext.Cam.projection);
		SkyboxTex->Bind();
		CubeMesh.Draw(SkyboxShader);

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);


		TestModel.Draw(RenderContext);
	//	TestModel2.Draw(RenderContext);

		ScreenFBO.Unbind();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ScreenShader.Bind();
		ScreenShader.SetTexture("screenTexture", ScreenFBOTex, 0);
		QuadMesh.Draw(ScreenShader);
		ScreenShader.Unbind();

		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// (Your code calls glfwSwapBuffers() etc.)
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();




	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void DrawScene() {

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{


	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		if (CursorEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			CursorEnabled = false;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			CursorEnabled = true;
		}
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	static glm::mat4 DirLightModelMat{ 1.0f };

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
	RenderContext.DirLightSource.Direction = glm::vec3{ DirLightModelMat[2].x, DirLightModelMat[2].y, DirLightModelMat[2].z };


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		RenderContext.Cam.Position += cameraSpeed * RenderContext.Cam.Direction;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		RenderContext.Cam.Position -= cameraSpeed * RenderContext.Cam.Direction;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		RenderContext.Cam.Position -= glm::normalize(glm::cross(RenderContext.Cam.Direction, RenderContext.Cam.UpVector)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		RenderContext.Cam.Position += glm::normalize(glm::cross(RenderContext.Cam.Direction, RenderContext.Cam.UpVector)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		RenderContext.Cam.Position.y += cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		RenderContext.Cam.Position.y -= cameraSpeed;
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
	RenderContext.Cam.Direction = glm::normalize(front);

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