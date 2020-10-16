#include "../include/Window.h"
#include <iostream>
#include <chrono>
#include <ctime>

_ZRV_SOURCE

Window::Window(uint16_t w, uint16_t h, const char* title) :
	_ww(w), _wh(h)
{
	_frame_interval = 0.0f;

	glfwInit();

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	p_window = glfwCreateWindow(w, h, title, nullptr/* glfwGetPrimaryMonitor()*/, nullptr);
	if (p_window == nullptr)
	{
//		std::cout << "Error: window cannot be created. Maybe your hardware don't support OpenGl 3.0" << std::endl;
//		getchar();
		exit(-1);
	}
	glfwMakeContextCurrent(p_window);
	//	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
//		std::cout << "Error: context cannot be created" << std::endl;
//		getchar();
		exit(-1);
	}
	initEventHandler(p_window);
	glfwGetFramebufferSize(p_window, &_fw, &_fh);
	glViewport(0, 0, _fw, _fh);

	this->initGl();
}

int zrv::Window::exec()
{
	while (!glfwWindowShouldClose(p_window))
	{
		_cur_time = glfwGetTime();

		glfwPollEvents();
		this->paintGl();
		glfwSwapBuffers(p_window);

		_frame_interval = glfwGetTime() - _cur_time;
	}
	return 0;
}
