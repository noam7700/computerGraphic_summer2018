﻿// ImGui - standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (Glad is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, GL3W, etc.)

#include <iostream>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>    // This example is using glad to access OpenGL functions. You may freely use any other OpenGL loader such as: glew, gl3w, glLoadGen, etc.
#include <GLFW/glfw3.h>
// Handling imgui+glfw
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// Scene, rendering and mesh loading
#include "Renderer.h"
#include "Scene.h"
#include "ImguiMenus.h"
#include "PrimMeshModel.h"
#include "InitShader.h" //for the function "string ReadShaderSource(const string& shaderFile);"


// Callback for the error state of glfw
static void GlfwErrorCallback(int error, const char* description);
// Setups the internal state of glfw, and intializing glad.
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
// Setup's the internal state of imgui.
ImGuiIO& SetupDearImgui(GLFWwindow* window);
// Takes care of all the opengl and glfw backend for rendering a new frame.
void StartFrame();
// Renders imgui. Takes care of screen resize, and finally renders the scene
void RenderFrame(GLFWwindow* window);
// Cleanup routines of all the systems used here.
void Cleanup(GLFWwindow* window);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main2(int argc, char **argv)
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int VBO,EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
int main()
{
    // Setup window
	int w = 1280, h = 720;
	GLFWwindow* window = SetupGlfwWindow(w,h,"Mesh Viewer");
	if (!window)
		return 1;
	// Setup renderer and scene
	
	//Renderer renderer = Renderer(w , h);
	Scene scene = Scene();
	Camera cam =Camera();
	glm::vec4 eye(0.0f, 0.0f, 0.0f, 0.0f); //left-down corner
	glm::vec4 at(0.0f, 0.0f, -1.0f, 0.0f); 
	glm::vec4 up(0.5f, 0.5f, 0.0f, 0.0f);
	int num = 0;
	int w2 = w, h2 = h;
	
	//openGL testing. draw simple box and so on...
	//once I'm able to do that the code should be deleted and move on to implement real code
	//in the scene (no use for renderer)

	/*********************************/


	
	//we use the file shaders, and send them to openGL with the function "InitShader"
	//openGL hello-triangle code --- InitShaders does all that hard work for us

	//shaders init & compilation...
	/*
	int  success;
	char infoLog[512];

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//check vshader compiling errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//check fshader compiling errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	*/

	//program init...
	/*
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	//after linking the shaders into the program object, we don't need the shaders anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	*/

	//do shaders init & compilation
	//do program init & linking
	//above is learn_openGL code for this. but it's the same (same openGL calls)
	GLuint shaderProgram = InitShader("vshader.glsl", "fshader.glsl"); 

	glEnable(GL_DEPTH_TEST); //z-buffer! :)


	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};
	/*****add prim object and draw him instead of our "hello triangle"*****/
	//scene.LoadPrim();
	//glm::vec4* helloPrim = scene.getModels()[scene.ActiveModel]->GetVertex();
	//GLint sizeVertices = scene.getModels()[scene.ActiveModel]->getVertexPosNum();

	//openGL init testing (some hello Triangle vao's & bao's for it)
	/*
	//vertex array object init...
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	
	
	//vertex buffer object init...
	unsigned int VBO[2];
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sizeVertices, &helloPrim[0], GL_STATIC_DRAW);


	//"specify how OpenGL should interpret the vertex data[i.e. attributes] before rendering..."  ~Learn_openGL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //position attributes
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sizeVertices, &helloPrim[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //position attributes
	glEnableVertexAttribArray(1);


	//unbind vbo & vao (so later calls calls won't accidentally modify this objects
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
	

	


	//only thing to do is useProgram and call draw function in the main while --below--



	/*********************************/




	
	//cam.LookAt(eye, at, up);

	//scene.LoadOBJModel("../Data/cow.obj");
	double a = 1, b = 270;
	scene.remove_cam(0);
	scene.load_cam(&cam);
	glm::vec4 avg ;
	scene.setcur_model(0);
	
	//scene.transformModel(cam.GetScaleTransform(a, a, a));


	//we normalize the mouse on the avereg of all the vertexes and it will be the center 
	//of all the rotations 
	//cam.LookAt(glm::vec4(0, 1, 2, 3), glm::vec4(0, 0, 0, 0), glm::vec4(1, 0, 0, 0));
    // Setup Dear ImGui binding
	ImGuiIO& io = SetupDearImgui(window);
	//scene.transformProjection(0, 1, 0, 1, 0, 1);
	glm::mat4x4 d = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	double xpos, ypos;
	int active = 1;
	float x, y, z;
	float x1=0, y1=0, z1=0;
	glfwGetWindowSize(window, &w, &h);
	glm::vec2 old_size = glm::vec2(1280,720);
	glm::vec2 size;

	
	//GLFWwindow* my_window = SetupGlfwWindow(w, h, "the window");
    // Main loop - the famous "Game Loop" in video games :)
	if(false)
	{
		//while (!glfwWindowShouldClose(window))
		/*
		while (!glfwWindowShouldClose(window))
		{

			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();
			// draw scene here



				//if (glfwGetKey(window, 'R') == GLFW_PRESS)
				//scene.transformProjection(1, 2, 1, 2, 1, 2);
				//resizing
			glfwGetWindowSize(window, &w, &h);
			if (scene.getModels().size() > 0)
			{
				size = glm::vec2(w, h);
				glfwGetCursorPos(window, &xpos, &ypos);
				ypos = h - ypos;
				avg = scene.GetVertexAvg(scene.ActiveModel);
				x = (avg.x);
				//x = (avg.x);
				y = (avg.y);
				z = avg.z;

				//update lookAt:
				if (glfwGetKey(window, 'C') == GLFW_PRESS)
				{
					glm::vec4 eye(0, 0, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f, 0.0f);
					scene.getCameras().at(scene.ActiveCamera)->LookAt(eye, avg, up);
				}


				if ((size.x != old_size.x || size.y != old_size.y)
					&& size.x != 0 && size.y != 0) //window resizing
				{

					scene.transformModel(cam.GetScaleTransform
					(old_size.x / size.x, old_size.x / size.x, 1));
					old_size.x = size.x;
					old_size.y = size.y;
				}



				if ((x != xpos || y != ypos) &&
					scene.getModels()[scene.ActiveModel]->folow_the_mouse)//follow the mouse
					scene.transformModel(cam.GetTranslateTransform((xpos / h - x)
						, (ypos / w - y), 0));
			}
			glfwGetWindowSize(window, &w, &h);
			scene.DrawScene(float(w), float(h)); //task3 - part2

			//scene.transformModel(cam.GetTranslateTransform(-b, -b, -b)*
			//cam.GetrotationTransform(1, 0)*cam.GetTranslateTransform(b, b, b) );
			// Start the ImGui frame
			StartFrame();
			// imgui stuff here
			DrawImguiMenus(io, &scene, window);
			// Rendering + user rendering - finishing the ImGui frame
			// go to function implementation to add your rendering calls.
			RenderFrame(window, &renderer);// --> go to line 137
		}
		*/
	}
	else
	{
		while (!glfwWindowShouldClose(window))
		{

			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			
			
			glUseProgram(shaderProgram);


			/** uniform testing **/
			/*
			float timeValue = glfwGetTime();
			float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
			int uniformColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
			glUniform4f(uniformColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
			*/

			if (scene.getModels().size() > 0)
			{
				/*
				MeshModel* activeM = scene.getModels().at(scene.ActiveModel);
				unsigned int sizeVertices = activeM->getVertexPosNum();
				activeM->bindVaoModel();
				glDrawArrays(GL_TRIANGLES, 0, sizeVertices);
				*/
				scene.DrawOpenGL(shaderProgram);
			}
			
			// Start the ImGui frame
			StartFrame(); 
			// imgui stuff here
			DrawImguiMenus(io, &scene, window); 
			// Rendering + user rendering - finishing the ImGui frame
			// go to function implementation to add your rendering calls.
			RenderFrame(window);// --> go to line 137 //*******************************************************************************

			

		}
	}
    // Cleanup
	Cleanup(window);
    
	
	
	
	return 0;
}

// Callback for the error state of glfw
static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Setups the internal state of glfw, and intializing glad.
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

// Setup's the internal state of imgui.
ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
	return io;
}

// Takes care of all the opengl and glfw backend for rendering a new frame.
void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

// Renders imgui. Takes care of screen resize, and finally renders the scene
void RenderFrame(GLFWwindow* window)
{
	// creates ImGui's vertex buffers and textures
	ImGui::Render();
	// getting current frame buffer size. Dont get confused with glfwWindowSize!!!
	// frame buffer is in pixels, screen size in different units for different systems.
	int displayW, displayH;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &displayW, &displayH);
	// Telling opengl to resize the framebuffer
	glViewport(0, 0, displayW, displayH);
	
	// put renderer code here
	// #######################################
	//renderer->printLine();

	//renderer->Viewport(displayW-100, displayH-100);
	//renderer->ClearColorBuffer(GetClearColor()); -----------------I removed this function because there's no renderer anymore!

	//*** I added instead the following lines in the main loop:

	
	// #######################################
	
	// Actual rendering of ImGui. ImGui only creates buffers and textures, 
	// which are sent to opengl for the actual rendering.
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}
