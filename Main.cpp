#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <algorithm>
#include <stb/stb_image.h>
#include <nfd/nfd.h>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Model.h"

// GLFW call back functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Renderer settings
unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;
const std::string TEXTURE_PATH = "Resources/";
float cameraFOV = 45.0f;
float cameraNearPlane = 0.1f;
float cameraFarPlane = 100.0f;
// Set up the camera
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

// OpenGL debug output context
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParameter)
{
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

// #define DEBUG_OUTPUT_ENABLED



// Vertices coordinates
Vertex vertices[] =
{//			COORDINATES						NORMALS							TEXTURE COORDINATES    
	Vertex{	glm::vec3(-1.0f, 0.0f,  1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f)},
	Vertex{	glm::vec3(-1.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)},
	Vertex{	glm::vec3(1.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)},
	Vertex{	glm::vec3(1.0f, 0.0f,  1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

glm::vec3 lightPositions[] = {
	glm::vec3(0.5, 0.5, 0.5),
	glm::vec3(0.5, 0.5, -0.5)
};


// GUI rendering functions
void showMainMenuBar(Model& model, std::string& currentModelPath);

int main()
{
	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG_OUTPUT_ENABLED
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	// Setup GLFW callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load all OpenGL function pointers with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Configurations for debug output context
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		std::cout << "OpenGL debug output is enabled!" << std::endl;
	}



	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		io.FontGlobalScale = 1.25f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");



	// Load textures for object
	std::string texturePath = "Resources/";
	Texture textures[] =
	{
		Texture((texturePath + "planks.png").c_str(), DIFFUSE, 0),
		Texture((texturePath + "planksSpec.png").c_str(), SPECULAR, 1)
	};

	// Load a model
	bool flipTexture = true;
	std::string currentModelPath = "Resources/deccer-cubes/SM_Deccer_Cubes_Textured.glb";
	Model currentModel(currentModelPath.c_str(), flipTexture);
	currentModel.Scale(glm::vec3(2.0f));



	// Setting up default shader and its objects to render
	Shader shaderProgram("default.vert", "default.frag");
	std::vector <Vertex> objVertices(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> objIndices(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> objTextures(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(objVertices, objIndices, objTextures);

	// Setting up the light shader and its object to render
	Shader lightShader("light.vert", "light.frag");
	std::vector <Vertex> lightVert(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightIdx(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVert, lightIdx);

	// Setting up depth shader for depth visualization
	Shader depthShader("default.vert", "depth.frag");

	// Setting up stencil outline shader
	Shader stencilOutlineShader("stencilOutline.vert", "stencilOutline.frag");
	stencilOutlineShader.Activate();
	stencilOutlineShader.SetFloat("outlining", 0.08f);



	// Initialize values of uniform variables in each shader
	glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPosition);

	glm::vec3 lightPosition(0.5f, 0.5f, 0.5f);
	DirLight dirLight(shaderProgram, 0.6f, 1.0f, 0.8f, glm::vec3(0.0f, 0.0f, 1.0f));



	// Assign values to the uniform variables
	shaderProgram.Activate();
	shaderProgram.SetMat4("model", objectModel);
	shaderProgram.SetMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(objectModel))));
	shaderProgram.SetFloat("shininess", 4.0f);



	// Enable the depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable stencil buffer
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	// GUI variables
	Shader* currentShader = &shaderProgram;
	glm::vec4 clearColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	bool showDemoWindow = false;
	bool lighting = true;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear viewport to a color
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		// Clear buffers to update the frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



		// **********************************************************
		// * IMGUI SECTION											*
		// **********************************************************
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Options", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Shader"))
			{
				if (ImGui::MenuItem("Default"))
				{
					currentShader = &shaderProgram;
				}
				if (ImGui::MenuItem("Depth"))
				{
					currentShader = &depthShader;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Other"))
			{
				ImGui::Checkbox("Demo Menu", &showDemoWindow);
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::SeparatorText("Model");
		if (ImGui::Button("Flip Texture"))
		{
			flipTexture = !flipTexture;
			currentModel = Model(currentModelPath.c_str(), flipTexture);
		}

		ImGui::SeparatorText("Environment");
		ImGui::ColorEdit3("Background color", glm::value_ptr(clearColor));
		if (ImGui::Checkbox("Lighting", &lighting))
		{
			shaderProgram.Activate();
			shaderProgram.SetBool("lighting", lighting);
		}

		ImGui::SeparatorText("Performance");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::End();

		if (showDemoWindow)
			ImGui::ShowDemoWindow(&showDemoWindow);
		showMainMenuBar(currentModel, currentModelPath);
		


		// **********************************************************
		// * SCENE DRAWING SECTION									*
		// **********************************************************
		currentModel.Draw(*currentShader, camera);



		// Render ImGui UIs
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		// Handel camera input
		if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
			camera.ProcessInputs(window);
		// Setting up matrices for 3D perspective
		camera.UpdateMatrix(cameraFOV, 0.1f, 100.0f);

		// Check and call events and swap the frame buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();



	// Clean up the objects and shader program
	shaderProgram.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// Resize viewport whenever the size of the window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.UpdateAspectRatio(width, height);
}

// Adjust FOV by scrolling
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraFOV -= (float)yoffset;
	if (cameraFOV < 1.0f)
		cameraFOV = 1.0f;
	if (cameraFOV > 45.0f)
		cameraFOV = 45.0f;
}



void showMainMenuBar(Model &model, std::string& currentModelPath)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) 
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

				if (result == NFD_OKAY)
				{
					std::replace(outPath, outPath + strlen(outPath), '\\', '/');
					currentModelPath = outPath;
					model = Model(outPath);
					free(outPath);
				}
				else if (result == NFD_CANCEL) {}
				else
				{
					std::cout << "ERROR::" << NFD_GetError() << std::endl;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}