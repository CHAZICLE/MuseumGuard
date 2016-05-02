#include <glm/gtc/matrix_transform.hpp>
#include "util/gl.h"
#include <iostream>
#include <string>
#include "render/shaders/ShaderUtils.hpp"
#include "gui/screens/MainMenu.hpp"
#include "util/Console.hpp"
#include "render/BasicShapes.hpp"
#include "input/Controls.hpp"
#include "render/RenderManager.hpp"
#include "util/DeltaTime.hpp"
#include "util/AssetManager.hpp"

#include "WindowScreenManager.hpp"

WindowScreenManager *WindowScreenManager::eventHandler = 0;
WindowScreenManager::WindowScreenManager() : ScreenManager()
{
	this->lastWindowWidthPx = 0;
	this->lastWindowHeightPx = 0;
	this->windowWidthPx = 0;
	this->windowHeightPx = 0;
	this->monitorWidthPx = 0;
	this->monitorHeightPx = 0;
	this->modeWidthPx = 0;
	this->modeHeightPx = 0;
	this->scale = 1;
	this->lastX = 0;
	this->lastY = 0;

	WindowScreenManager::eventHandler = this;
	glfwSetErrorCallback(WindowScreenManager::onError);
	// Initialise and create window
	if(!glfwInit())
	{
		std::cerr << "GLFW Init failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	/*glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/
	// Create window
	this->window = glfwCreateWindow(800, 600, "FYP", NULL, NULL);
	if(!this->window)
	{
		std::cerr << "Window creation failed" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(this->window);
	
#ifdef USE_GLEW
	// Setup GLEW
	glewExperimental = GL_TRUE;
	if(glewInit())
	{
		std::cerr << "GLEW initialization failed" << std::endl;
		exit(EXIT_FAILURE);
	}
#endif
	
	// Print info
	Console::println(CONSOLE_TAG_MAIN, "WindowScreenManager: [INIT] OpenGL version: "+std::string((const char *)glGetString(GL_VERSION)));
	Console::println(CONSOLE_TAG_MAIN, "WindowScreenManager: [INIT] GLSL version: "+std::string((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));
	Console::println(CONSOLE_TAG_MAIN, "WindowScreenManager: [INIT] Vendor: "+std::string((const char *)glGetString(GL_VENDOR)));
	Console::println(CONSOLE_TAG_MAIN, "WindowScreenManager: [INIT] Renderer: "+std::string((const char *)glGetString(GL_RENDERER)));
	
	// Event handlers
	glfwSetKeyCallback(this->window, WindowScreenManager::onKeyEvent);
	glfwSetCursorPosCallback(this->window, WindowScreenManager::onCursorPosEvent);
	glfwSetMouseButtonCallback(this->window, WindowScreenManager::onMouseButtonEvent);
	glfwSetScrollCallback(this->window, WindowScreenManager::onScrollEvent);
	
	shaders::compileShaders();
	BasicShapes::init();
	glEnableClientState(GL_VERTEX_ARRAY);
	this->openRootScreen(new MainMenu());

	util::AssetManager::getAssetManager()->init();
}
WindowScreenManager::~WindowScreenManager()
{
	
}
void WindowScreenManager::run()
{
	render::RenderManager renderManager;
	util::DeltaTime deltaTime(true, 60);
	while(!glfwWindowShouldClose(this->window))
	{
		// Get screen dimensions
		glfwGetFramebufferSize(this->window, &windowWidthPx, &windowHeightPx);
		if(windowWidthPx!=this->lastWindowWidthPx || windowHeightPx!=this->lastWindowHeightPx)
		{
			glViewport(0, 0, windowWidthPx, windowHeightPx);
			renderManager.setDimensionsPx(windowWidthPx, windowHeightPx);
			this->lastWindowWidthPx = windowWidthPx;
			this->lastWindowHeightPx = windowHeightPx;
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			if(monitor!=0)
			{
				glfwGetMonitorPhysicalSize(monitor, &monitorWidthPx, &monitorHeightPx);
				const GLFWvidmode *mode = glfwGetVideoMode(monitor);
				this->modeWidthPx = mode->width;
				this->modeHeightPx = mode->height;
				this->width = windowWidthPx*monitorWidthPx/modeWidthPx/scale;
				this->height = windowHeightPx*monitorHeightPx/modeHeightPx/scale;
				renderManager.setDimensionsMM(width, height);
				renderManager.P = glm::ortho(0.f, (float)this->width, 0.f, (float)this->height, 0.f, 1.f);
				renderManager.markPDirty();
				this->onScreenResize();
			}
		}
		
		// Calculate FPS
		deltaTime.postTime(glfwGetTime());
		
		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->render(&deltaTime, &renderManager);
		
		// Update frame buffer
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}
}
void WindowScreenManager::close()
{
	glfwSetWindowShouldClose(this->window, GL_TRUE);
}
void WindowScreenManager::onError(int error, const char *msg)
{
	std::cerr << msg << std::endl;
}
void WindowScreenManager::onKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	switch(action)
	{
		case GLFW_PRESS:
			action = CONTROL_KEYACTION_PRESS;
			break;
		case GLFW_RELEASE:
			action = CONTROL_KEYACTION_RELEASE;
			break;
		case GLFW_REPEAT:
			action = CONTROL_KEYACTION_REPEAT;
			break;
	}
	WindowScreenManager::eventHandler->onControlEvent(key, action);
}
void WindowScreenManager::onCursorPosEvent(GLFWwindow *window, double x, double y)
{
	y = WindowScreenManager::eventHandler->windowHeightPx-y;
	x = x*WindowScreenManager::eventHandler->monitorWidthPx/WindowScreenManager::eventHandler->modeWidthPx/WindowScreenManager::eventHandler->scale;
	y = y*WindowScreenManager::eventHandler->monitorHeightPx/WindowScreenManager::eventHandler->modeHeightPx/WindowScreenManager::eventHandler->scale;
	WindowScreenManager::eventHandler->onControlEvent(CONTROL_ACTION_MOUSE, x, y, WindowScreenManager::eventHandler->lastX-x, WindowScreenManager::eventHandler->lastY-y);
	WindowScreenManager::eventHandler->lastX = x;
	WindowScreenManager::eventHandler->lastY = y;
}
void WindowScreenManager::onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	switch(action)
	{
		case GLFW_PRESS:
			action = CONTROL_MOUSEBUTTONACTION_PRESS;
			break;
		case GLFW_RELEASE:
			action = CONTROL_MOUSEBUTTONACTION_RELEASE;;
			break;
		case GLFW_REPEAT:
			action = CONTROL_MOUSEBUTTONACTION_REPEAT;
			break;
	}
	WindowScreenManager::eventHandler->onControlEvent(button, action);
}
void WindowScreenManager::onScrollEvent(GLFWwindow* window, double dx, double dy)
{
	dx = dx*WindowScreenManager::eventHandler->monitorWidthPx/WindowScreenManager::eventHandler->modeWidthPx/WindowScreenManager::eventHandler->scale;
	dy = -dy*WindowScreenManager::eventHandler->monitorHeightPx/WindowScreenManager::eventHandler->modeHeightPx/WindowScreenManager::eventHandler->scale;
	WindowScreenManager::eventHandler->onControlEvent(CONTROL_ACTION_SCROLL, 0, 0, dx, dy);
}
void WindowScreenManager::onSurfaceScreenChanged(Screen *screen)
{
	if(screen->supportsCursor())
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	ScreenManager::onSurfaceScreenChanged(screen);
}
