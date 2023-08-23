#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stb/stb_image.h>

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
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;
const std::string TEXTURE_PATH = "Resources/";
float cameraFOV = 45.0f;
// Set up the camera
Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 1.0f, 2.0f));

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
void showMainMenuBar();

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
	// Setup GLFW callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	Model testModel("Resources/deccer-cubes/SM_Deccer_Cubes_Textured.gltf");



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



	// Initialize values of uniform variables in each shader
	glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPosition);

	glm::vec3 lightPosition(0.5f, 0.5f, 0.5f);
	DirLight dirLight(shaderProgram, 0.6f, 1.0f, 0.8f, glm::vec3(0.0f, -0.5f, -1.0f));
	// PointLight pointLight(shaderProgram, 0.2f, 0.8f, 0.5f, lightPosition);
	// SpotLight spotlight1(shaderProgram, 0.0f, 1.0f, 1.0f, lightPositions[0], glm::vec3(0.0f, -1.0f, 0.0f));
	// SpotLight spotlight2(shaderProgram, 0.0f, 1.0f, 1.0f, lightPositions[1], glm::vec3(0.0f, -1.0f, 0.0f));



	// Assign values to the uniform variables
	shaderProgram.Activate();
	shaderProgram.SetMat4("model", objectModel);
	shaderProgram.SetMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(objectModel))));
	shaderProgram.SetFloat("shininess", 4.0f);



	// Enable the depth buffer
	glEnable(GL_DEPTH_TEST);


	// Test variables
	glm::vec4 clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	bool showDemoWindow;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear viewport to a color
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		// Clear buffers to update the frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		// Handel camera input
		camera.ProcessKeyboardInputs(window);
		if (!io.WantCaptureMouse)
			camera.ProcessMouseInputs(window);
		// Setting up matrices for 3D perspective
		camera.UpdateMatrix(cameraFOV, 0.1f, 200.0f);



		// floor.Draw(shaderProgram, camera);

		/*for (int i = 0; i < 2; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, lightPositions[i]);
			lightShader.Activate();
			lightShader.SetMat4("model", lightModel);
			light.Draw(lightShader, camera);
		}*/


		testModel.Draw(shaderProgram, camera);



		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			ImGui::Begin("Window");                 
			ImGui::ColorEdit3("clear color", glm::value_ptr(clearColor)); // Edit 3 floats representing a color
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::ShowDemoWindow(&showDemoWindow);
		showMainMenuBar();



		// Render ImGui UIs
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}



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
	std::cout << width << "x" << height << std::endl;
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



void showMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}